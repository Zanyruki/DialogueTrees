#pragma 
#include "..\Utility\tinyxml2.h"
#include "..\Parser\MathematicExpression.h"
#include <string>


namespace gh
{
	using namespace tinyxml2;

	class DialogueAction
	{
	public:
		DialogueAction( XMLNode* actionNode );
		void doAction();

		 bool m_bIsPlayer;
		 bool m_bIsNewVariable;
		 std::string m_variableName;
		 std::string m_tableName;
		 MathematicExpression* m_expression;
		 DataType m_newVariableDataType;
	private:
		void createNewVariable( VariableTable* localTable );
		void setNewValue( VariableBase* var );
	};
}