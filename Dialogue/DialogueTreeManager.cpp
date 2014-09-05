#include "DialogueTreeManager.h"

namespace gh
{
	DialogueTreeManager::DialogueTreeManager()
	{

	}

	bool DialogueTreeManager::getDialogueTree( const std::string& name, DialogueTree*& outTree )
	{
		auto foundTree = m_dialogueMap.find( name );
		if( foundTree != m_dialogueMap.end() )
		{
			outTree =  foundTree->second;
			outTree->setUpFirstNode();
			return true;
		}

		return false;
	}


	void DialogueTreeManager::addDialogueTree( const std::string& name, DialogueTree* tree )
	{
		m_dialogueMap[ name ] = tree;
	}


	DialogueTreeManager& getTheDialogueTreeManager()
	{
		static DialogueTreeManager theDialogueTreeManager;
		return theDialogueTreeManager;
	}
}