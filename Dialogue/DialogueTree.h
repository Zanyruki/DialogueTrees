#pragma once
#define WIN32_LEAN_AND_MEAN
#include "..\Utility\tinyxml2.h"
#include "DialogueTreeNode.h"
#include "..\Parser\VariableTable.h"
#include <string>
#include <map>

using namespace tinyxml2;

namespace gh
{
	class DialogueTree
	{
	public:
		DialogueTree( const char* xmlDocumentName );
		DialogueTree( const DialogueTree* tree );
		void loadTree( const char* xmlDocumentName );
		void setUpFirstNode();
		void restartTree();
		void startConversation( std::vector< std::string >& outOptions, std::string& outText, const std::string& npcTableName );
		void respond( int responseIndex, std::vector< std::string >& outOptions, std::string& outText );

		std::string m_characterName;
		//VariableTable* m_playerTable;
		//VariableTable* m_npcTable;
	private:
		void setUpTree( DialogueTreeNode* currentNode, std::map< std::string, DialogueTreeNode* >& nodeMap );
		void getResponses( std::vector< std::string >& outResponses );

		DialogueTreeNode* m_rootNode;
		DialogueTreeNode* m_currentNode;
		
		
	};
}