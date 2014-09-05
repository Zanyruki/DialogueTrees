#pragma once
#include "..\Parser\VariableTable.h"

namespace gh
{
	class DialogueLocalVariableTableManager
	{
	public:
		DialogueLocalVariableTableManager();
		void setLocalVariableTable( const std::string& npcTableName );
		std::string m_npcVariableTableName;
	private:
		
	};

	DialogueLocalVariableTableManager& getTheDialogueLocalVariableTableManager();
}