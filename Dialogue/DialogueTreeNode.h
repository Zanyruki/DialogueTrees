#pragma once
#include <vector>
#include "DialogueResponse.h"
#include "..\Utility\tinyxml2.h"
#include "DialogueAction.h"

namespace gh
{
	class DialogueTreeNode
	{
	public:
		DialogueTreeNode();
		DialogueTreeNode( const DialogueTreeNode& node );
		DialogueTreeNode( tinyxml2::XMLNode* dialogueNode );
		const std::string& getNodeID();
		DialogueTreeNode* nextNode( const std::string& responseValue );
		void doActions();
		void fireEvents();
		
	private:
		//DialogueTreeNode* m_parent;
		std::string m_nodeID;
		MathematicExpression m_text;
		std::vector< std::string > m_eventNames;
		std::vector< std::string > m_eventStringArgs;
		std::vector< std::string > m_eventStringArgNames;
		std::vector< DialogueResponse* > m_responses;
		std::vector< DialogueAction* > m_actions;
		std::vector< DialogueTreeNode * > m_children;
		friend class DialogueTree;
	};
}