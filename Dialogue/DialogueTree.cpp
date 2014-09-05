#include "DialogueTree.h"
#include "..\Utility\Utility.h"
#include "..\Dialogue\DialogueLocalVariableTableManager.h"
#include "DialogueTreeManager.h"

using namespace tinyxml2;

namespace gh
{

DialogueTree::DialogueTree( const char* xmlDocumentName )
	: m_currentNode( nullptr )
{
	loadTree( xmlDocumentName );
}


DialogueTree::DialogueTree( const DialogueTree* tree )
	: m_rootNode( tree->m_rootNode )
	, m_currentNode( m_rootNode )
	, m_characterName( tree->m_characterName )
{
}


void DialogueTree::loadTree( const char* xmlDocumentName )
{
	XMLDocument xmlDocument;
	xmlDocument.LoadFile( xmlDocumentName );
	XMLElement* root = xmlDocument.FirstChildElement( "DialogueTree" )->ToElement();
	m_characterName = getStringXMLAttribute( root, "name", "none" );
	std::map< std::string, DialogueTreeNode* > nodeMap;

	for( auto dialogueNode = root->FirstChildElement( "Dialogue" ); dialogueNode; dialogueNode = dialogueNode->NextSiblingElement("Dialogue") )
	{
		DialogueTreeNode* treeNode = new DialogueTreeNode( dialogueNode );
		bool checkStartNode = getBoolXMLAttribute( dialogueNode, "start", false );
		if( checkStartNode )
		{
			m_rootNode = treeNode;
		}

		nodeMap[ treeNode->getNodeID() ] = treeNode;
	}

	setUpTree( m_rootNode, nodeMap);

	getTheDialogueTreeManager().addDialogueTree( m_characterName, this );
}


void DialogueTree::setUpTree( DialogueTreeNode* currentNode, std::map< std::string, DialogueTreeNode* >& nodeMap )
{
	auto found = nodeMap.find( currentNode->m_nodeID );
	if( found != nodeMap.end() )
	{
		for( auto responseIter = currentNode->m_responses.begin(); responseIter != currentNode->m_responses.end(); ++responseIter )
		{
			DialogueResponse* currentResponse = *responseIter;

			for( auto pathIter = currentResponse->m_possiblePaths.begin(); pathIter != currentResponse->m_possiblePaths.end(); ++pathIter )
			{
				DialogueTreeNode* childNode = nodeMap[ pathIter->m_nodeID ];
				if( childNode == nullptr )
				{
					reportError( "Node index \"" + pathIter->m_nodeID + "\" was not found." );
				}
				int newResponseIndex = currentNode->m_children.size();
				currentNode->m_children.push_back( childNode );
				pathIter->m_nodeIndex = newResponseIndex;
				setUpTree( childNode, nodeMap );
			}
		}
	}

}


void DialogueTree::getResponses( std::vector< std::string >& outResponses )
{
	m_currentNode->doActions();
	m_currentNode->fireEvents();
	for( auto responseIter = m_currentNode->m_responses.begin(); responseIter != m_currentNode->m_responses.end(); ++responseIter )
	{
		if( (*responseIter)->m_condition.evaluate() )
		{
			outResponses.push_back( (*responseIter )->m_text.evaluateAsString() );
		}
		
	}
}


void DialogueTree::setUpFirstNode()
{
	m_currentNode = m_rootNode;
}


void DialogueTree::restartTree()
{
	m_currentNode = m_rootNode;
}


void DialogueTree::startConversation( std::vector< std::string >& outOptions, std::string& outText, const std::string& npcTableName )
{
	getTheDialogueLocalVariableTableManager().setLocalVariableTable( npcTableName );
	m_currentNode = m_currentNode->nextNode( "" );
	getResponses( outOptions );
	outText = m_currentNode->m_text.evaluateAsString();
}


void DialogueTree::respond( int responseIndex, std::vector< std::string >& outOptions, std::string& outText )
{
	std::string response = outOptions[ responseIndex ];
	m_currentNode = m_currentNode->nextNode( response );
	outOptions.clear();
	getResponses( outOptions );
	outText = m_currentNode->m_text.evaluateAsString();
}


}