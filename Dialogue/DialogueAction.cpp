#include "DialogueAction.h"
#include "..\Utility\Utility.h"
#include "DialogueLocalVariableTableManager.h"
#include "..\Utility\VariableUtility.h"
#include "..\Parser\Variable.h"
#include "..\Parser\VariableTableManager.h"

namespace gh
{
	using namespace tinyxml2;


	DialogueAction::DialogueAction( XMLNode* actionNode )
		: m_expression( nullptr )
	{
		std::string tempVar = getStringXMLAttribute( actionNode, "variable", "" );
		int tableIndex = tempVar.find_first_of( '.' );
		if( tableIndex == std::string::npos )
		{
			m_variableName = tempVar;
			m_tableName = "local";
		}
		else
		{
			m_tableName = tempVar.substr( 1, tableIndex - 1 );
			m_variableName = "$" + tempVar.substr( tableIndex + 1 );
		}
		
		std::string expressionString = getStringXMLAttribute( actionNode, "expression", m_variableName );
		m_bIsPlayer = getBoolXMLAttribute( actionNode, "isPlayer", false );
		m_bIsNewVariable = getBoolXMLAttribute( actionNode, "isNewVariable", false );
		m_expression = new MathematicExpression( expressionString );
		std::string newVarType = getStringXMLAttribute( actionNode, "type", "string" );
		m_newVariableDataType = getDataTypeFromString( newVarType );
	}

	
	void DialogueAction::doAction()
	{
		VariableTable* varTable = nullptr;
		std::string localTableName = getTheDialogueLocalVariableTableManager().m_npcVariableTableName;
		if( m_tableName == "local" )
		{
			varTable = getTheVariableTableManager().getTable( localTableName );
		}
		else
		{
			varTable = getTheVariableTableManager().getTable( m_tableName );
		}

		VariableBase* var = varTable->getVariable( m_variableName );
		m_expression->setLocalVariableTable( localTableName );
		if( m_bIsNewVariable )
		{
			if( var == nullptr )
			{
				createNewVariable( varTable );
			}
			
		}
		else
		{
			setNewValue( var );
		}
		
	}


	void DialogueAction::setNewValue( VariableBase* var )
	{
		DataType type = var->getDataType();
		if( type == FLOAT_DATA_TYPE )
		{
			Variable<float>* floatVar = reinterpret_cast< Variable< float >* >( var );
			floatVar->m_value = m_expression->evaluateAsFloat();
		}
		else if( type == INT_DATA_TYPE )
		{
			Variable< int >* intVar = reinterpret_cast< Variable< int >* >( var );
			int v = m_expression->evaluateAsInt();
			intVar->m_value = v;
		}
		else if( type == STRING_DATA_TYPE )
		{
			Variable< std::string >* stringVar = reinterpret_cast< Variable< std::string >* >( var );
			stringVar->m_value = m_expression->evaluateAsString();
		}
		else if( type == BOOL_DATA_TYPE )
		{
			Variable< bool >* boolVar = reinterpret_cast< Variable< bool >* >( var );
			boolVar->m_value = m_expression->evaluateAsBool();
		}
	}


	void DialogueAction::createNewVariable( VariableTable* localTable )
	{
		if( m_newVariableDataType == BOOL_DATA_TYPE )
		{
			bool value = m_expression->evaluateAsBool();
			Variable< bool >* boolVar = new Variable< bool >( m_variableName, value, BOOL_DATA_TYPE );
			localTable->setVariable( boolVar );
		}
		else if( m_newVariableDataType == INT_DATA_TYPE )
		{
			int value = m_expression->evaluateAsInt();
			Variable< int >* intVar = new Variable< int >( m_variableName, value, INT_DATA_TYPE );
			localTable->setVariable( intVar );
		}
		else if( m_newVariableDataType == FLOAT_DATA_TYPE )
		{
			float value = m_expression->evaluateAsFloat();
			Variable< float >* floatVar = new Variable< float >( m_variableName, value, FLOAT_DATA_TYPE );
			localTable->setVariable( floatVar );
		}
		else if( m_newVariableDataType == STRING_DATA_TYPE )
		{
			std::string value = m_expression->evaluateAsString();
			Variable< std::string >* stringVar = new Variable< std::string >( m_variableName, value, STRING_DATA_TYPE );
			localTable->setVariable( stringVar );
		}

	}
}