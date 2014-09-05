#include "DialogueTreeNode.h"
#include "..\Utility\tinyxml2.h"
#include "..\Utility\Utility.h"
#include "..\EventSystem\EventSystem.h"
#include "..\EventSystem\VariableNamedProperties.h"

using namespace tinyxml2;

namespace gh
{


DialogueTreeNode::DialogueTreeNode()
	: m_nodeID( "" )
	, m_text("")
{}


const std::string& DialogueTreeNode::getNodeID()
{
	return m_nodeID;
}


DialogueTreeNode::DialogueTreeNode( const DialogueTreeNode& node )
	: m_text( m_text )
{
	m_nodeID = node.m_nodeID;
	m_responses = node.m_responses;
	m_children = node.m_children;
}


DialogueTreeNode::DialogueTreeNode( tinyxml2::XMLNode* dialogueNode )
	: m_text("")
{
	m_nodeID = getStringXMLAttribute( dialogueNode, "nodeID", "" );
	XMLNode* textNode = dialogueNode->FirstChildElement( "Text" );
	if( textNode )
	{
		m_text = MathematicExpression( getStringXMLAttribute( textNode, "value", "NO DIALOGUE" ) );
	}

	//get actions
	for( auto actionNode = dialogueNode->FirstChildElement( "Action" ); actionNode; actionNode = actionNode->NextSiblingElement("Action") )
	{
		DialogueAction* action = new DialogueAction( actionNode );
		m_actions.push_back( action );
	}

	//get events
	for( auto eventNode = dialogueNode->FirstChildElement( "Event" ); eventNode; eventNode = eventNode->NextSiblingElement("Event") )
	{
		std::string temp = getStringXMLAttribute( eventNode, "eventName", "" );
		m_eventNames.push_back( temp );
		m_eventStringArgs.push_back( getStringXMLAttribute( eventNode, "eventArg", "" ) );
		std::string eventArgName = getStringXMLAttribute( eventNode, "eventArgName", "" );
		m_eventStringArgNames.push_back( eventArgName );
	}

	//get responses
	for( auto responseNode = dialogueNode->FirstChildElement( "Response" ); responseNode; responseNode = responseNode->NextSiblingElement("Response") )
	{
		std::string responseText = getStringXMLAttribute( responseNode, "value", "NO RESPONSE" );
		std::string conditionString = getStringXMLAttribute( responseNode, "condition", "true" );
		DialogueResponse* response = new DialogueResponse( responseText, conditionString );
		for( auto nextNodeNode = responseNode->FirstChildElement( "NextNode" ); nextNodeNode; nextNodeNode = nextNodeNode->NextSiblingElement("NextNode") )
		{
			std::string condition = getStringXMLAttribute( nextNodeNode, "condition", "true" );
			std::string treeNodeID = getStringXMLAttribute( nextNodeNode, "nodeID", "" );
			response->addPossiblePath( condition, treeNodeID );
		}

		m_responses.push_back( response );
	}
}


DialogueTreeNode* DialogueTreeNode::nextNode( const std::string& responseValue )
{
	DialogueResponse* response = nullptr;
	for( auto responseIter = m_responses.begin(); responseIter != m_responses.end(); ++responseIter )
	{
		if( strcmp( (*responseIter)->m_text.evaluateAsString().c_str(), responseValue.c_str() ) == 0 )
		{
			response = *responseIter;
		}
	}
	 
	int nodeIndex = response->getPath();
	if( nodeIndex >= 0 )
	{
		return m_children[ nodeIndex ];
	}

	return nullptr;
}


void DialogueTreeNode::doActions()
{
	for( auto actionIter = m_actions.begin(); actionIter != m_actions.end(); ++actionIter )
	{
		(*actionIter)->doAction();
	}
}


void DialogueTreeNode::fireEvents()
{
	for( int index = 0; index < (int)m_eventNames.size(); ++index )
	{
		VariableNamedProperties vnp;
		vnp.SetProperty( m_eventStringArgNames[ index ], m_eventStringArgs[ index ] );
		std::string temp = m_eventNames[ index ];
		FireEvent( temp, vnp );
	}
}


}