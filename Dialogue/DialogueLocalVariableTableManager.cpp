#include "DialogueLocalVariableTableManager.h"

namespace gh
{
	DialogueLocalVariableTableManager::DialogueLocalVariableTableManager()
		: m_npcVariableTableName( "" )
	{

	}


	void DialogueLocalVariableTableManager::setLocalVariableTable( const std::string& npcTableName )
	{
		m_npcVariableTableName = npcTableName;
	}


	DialogueLocalVariableTableManager& getTheDialogueLocalVariableTableManager()
	{
		static DialogueLocalVariableTableManager theLocalVarTableManager;
		return theLocalVarTableManager;
	}
}