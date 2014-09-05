#pragma once
#include "DialogueTree.h"
#include <map>

namespace gh
{
	class DialogueTreeManager
	{
	public:
		DialogueTreeManager();
		bool getDialogueTree( const std::string& name, DialogueTree*& outTree );
		void addDialogueTree( const std::string& name, DialogueTree* tree );
	private:
		std::map< std::string, DialogueTree* > m_dialogueMap;
	};

	DialogueTreeManager& getTheDialogueTreeManager();
}