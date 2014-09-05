#include "ExpressionNode.h"
#include "MathematicExpression.h"
#include "VariableTableManager.h"

namespace gh
{


ExpressionNode::ExpressionNode()
{
}


ExpressionNode::ExpressionNode( OperationType opType, ExpressionNode* leftChild, ExpressionNode* rightChild, DataType operationResult )
	: m_nodeType( operation_type )
	, m_operationType( opType )
	, m_constantType( operationResult )
	, m_leftChild( leftChild )
	, m_rightChild( rightChild )
{
}


ExpressionNode::ExpressionNode( const std::string& variableName, const std::string& tableName )
	: m_nodeType( variable_type )
	, m_variableName( variableName )
	, m_tableName( tableName )
{
}


ExpressionNode::ExpressionNode( VariableBase* var, DataType constType )
	: m_nodeType( constant_type )
	, m_constant( var )
	, m_constantType( constType )
{
}


VariableTable* ExpressionNode::getTable()
{		
	return getTheVariableTableManager().getTable( m_tableName );
}


DataType ExpressionNode::getPreferredType()
{
	if( m_nodeType == operation_type )
	{
		if( m_constantType == NULL_DATA_TYPE )
		{
			if( m_leftChild != nullptr )
			{
				return m_leftChild->getPreferredType();
			}
			else
			{
				return m_rightChild->getPreferredType();
			}
		}
		else
		{
			return m_constantType;
		}
	}
	else if( m_nodeType == variable_type )
	{
		VariableTable* table = getTable();
		if( table != nullptr )
		{
			VariableBase* varBase = table->getVariable( m_variableName );
			if( varBase != nullptr )
			{
				return varBase->getDataType();
			}
			return NULL_DATA_TYPE;
		}
		return NULL_DATA_TYPE;
	}
	else
	{
		return m_constantType;
	}
}


bool ExpressionNode::evaluateAsBool()
{
	DataType prefferedType = getPreferredType();
	if( prefferedType == BOOL_DATA_TYPE )
	{
		return getValueAsBool();
	}
	else if( prefferedType == INT_DATA_TYPE )
	{
		int intValue = getValueAsInt();
		return intValue != 0;
	}
	else if( prefferedType == FLOAT_DATA_TYPE )
	{
		float floatValue = getValueAsFloat();
		return floatValue != 0.f;
	}
	else if( prefferedType == STRING_DATA_TYPE )
	{
		std::string stringValue = getValueAsString();
		return stringValue == "true";
	}
	return false;
}


int ExpressionNode::evaluateAsInt()
{
	DataType prefferedType = getPreferredType();
	if( prefferedType == INT_DATA_TYPE )
	{
		return getValueAsInt();
	}
	else if( prefferedType == BOOL_DATA_TYPE )
	{
		return (int)getValueAsBool();
	}
	else if( prefferedType == FLOAT_DATA_TYPE )
	{
		return (int)getValueAsFloat();
	}
	else if( prefferedType == STRING_DATA_TYPE )
	{
		return atoi( getValueAsString().c_str() );
	}
	return 0;
}


float ExpressionNode::evaluateAsFloat()
{
	DataType prefferedType = getPreferredType();
	if( prefferedType == FLOAT_DATA_TYPE )
	{
		return getValueAsFloat();
	}
	else if( prefferedType == BOOL_DATA_TYPE )
	{
		return (float)getValueAsBool();
	}
	else if( prefferedType == INT_DATA_TYPE )
	{
		return (float)getValueAsInt();
	}
	else if( prefferedType == STRING_DATA_TYPE );
	{
		return (float)atof( getValueAsString().c_str() );
	}

	return 0.f;
}


std::string ExpressionNode::evaluateAsString()
{
	DataType prefferedType = getPreferredType();
	if( prefferedType == STRING_DATA_TYPE )
	{
		return getValueAsString();
	}
	else if( prefferedType == BOOL_DATA_TYPE )
	{
		if( getValueAsBool() )
		{
			return "true";
		}
		return "false";
	}
	else if( prefferedType == INT_DATA_TYPE )
	{
		std::string returnString;
		convertToString( getValueAsInt(), returnString );
		return returnString;
	}
	else if( prefferedType == FLOAT_DATA_TYPE )
	{
		std::string returnString;
		convertToString( getValueAsFloat(), returnString );
		return returnString;
	}
	return "";
}


bool ExpressionNode::getValueAsBool()
{
	if( m_nodeType == constant_type )
	{
		Variable< bool >* boolVar = reinterpret_cast< Variable< bool >* >( m_constant );
		if( boolVar != nullptr )
		{
			return boolVar->m_value;
		}
	}
	else if ( m_nodeType == variable_type )
	{
		VariableTable* varTable = getTable();
		if( varTable != nullptr )
		{
			bool returnBool;
			if( varTable->getVariable( m_variableName, returnBool, BOOL_DATA_TYPE ) )
			{
				return returnBool;
			}
			
		}
	}
	else
	{
		if( m_operationType == equalEqual_operation )
		{
			if( m_leftChild != nullptr )
			{
				DataType childrenType = m_leftChild->getPreferredType();
				switch( childrenType )
				{
				case BOOL_DATA_TYPE:
					return evaluateEqualsEquals( m_leftChild->evaluateAsBool(), m_rightChild->evaluateAsBool() );
					break;
				case INT_DATA_TYPE:
					return evaluateEqualsEquals( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
					break;
				case FLOAT_DATA_TYPE:
					return evaluateEqualsEquals( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
					break;
				case STRING_DATA_TYPE:
					return evaluateEqualsEquals( m_leftChild->evaluateAsString(), m_rightChild->evaluateAsString() );
					break;
				}
			}
		}
		else if( m_operationType == greaterThanOrEqual_operation )
		{
			if( m_leftChild != nullptr )
			{
				DataType childrenType = m_leftChild->getPreferredType();
				switch( childrenType )
				{
				case BOOL_DATA_TYPE:
					return evaluateGreaterThanOrEquals( m_leftChild->evaluateAsBool(), m_rightChild->evaluateAsBool() );
					break;
				case INT_DATA_TYPE:
					return evaluateGreaterThanOrEquals( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
					break;
				case FLOAT_DATA_TYPE:
					return evaluateGreaterThanOrEquals( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
					break;
				case STRING_DATA_TYPE:
					return evaluateGreaterThanOrEquals( m_leftChild->evaluateAsString(), m_rightChild->evaluateAsString() );
					break;
				}
			}
		}
		else if( m_operationType == greaterThan_operation )
		{
			if( m_leftChild != nullptr )
			{
				DataType childrenType = m_leftChild->getPreferredType();
				switch( childrenType )
				{
				case BOOL_DATA_TYPE:
					return evaluateGreaterThan( m_leftChild->evaluateAsBool(), m_rightChild->evaluateAsBool() );
					break;
				case INT_DATA_TYPE:
					return evaluateGreaterThan( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
					break;
				case FLOAT_DATA_TYPE:
					return evaluateGreaterThan( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
					break;
				case STRING_DATA_TYPE:
					return evaluateGreaterThan( m_leftChild->evaluateAsString(), m_rightChild->evaluateAsString() );
					break;
				}
			}
		}
		else if( m_operationType == lessThan_operation )
		{
			if( m_leftChild != nullptr )
			{
				DataType childrenType = m_leftChild->getPreferredType();
				switch( childrenType )
				{
				case BOOL_DATA_TYPE:
					return evaluateLessThan( m_leftChild->evaluateAsBool(), m_rightChild->evaluateAsBool() );
					break;
				case INT_DATA_TYPE:
					return evaluateLessThan( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
					break;
				case FLOAT_DATA_TYPE:
					return evaluateLessThan( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
					break;
				case STRING_DATA_TYPE:
					return evaluateLessThan( m_leftChild->evaluateAsString(), m_rightChild->evaluateAsString() );
					break;
				}
			}
		}
		else if( m_operationType == lessThanOrEqual_operation )
		{
			if( m_leftChild != nullptr )
			{
				DataType childrenType = m_leftChild->getPreferredType();
				switch( childrenType )
				{
				case BOOL_DATA_TYPE:
					return evaluateLessThanOrEquals( m_leftChild->evaluateAsBool(), m_rightChild->evaluateAsBool() );
					break;
				case INT_DATA_TYPE:
					return evaluateLessThanOrEquals( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
					break;
				case FLOAT_DATA_TYPE:
					return evaluateLessThanOrEquals( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
					break;
				case STRING_DATA_TYPE:
					return evaluateLessThanOrEquals( m_leftChild->evaluateAsString(), m_rightChild->evaluateAsString() );
					break;
				}
			}
		}
		else if( m_operationType == and_operation )
		{
			return evaluateAnd( m_leftChild->evaluateAsBool(), m_rightChild->evaluateAsBool() );
		}
		else if( m_operationType == or_operation )
		{
			return evaluateOr( m_leftChild->evaluateAsBool(), m_rightChild->evaluateAsBool() );
		}
		else if( m_operationType == not_operation )
		{
			return evaluateNot( m_rightChild->evaluateAsBool() );
		}
	}

	return false;
}


int ExpressionNode::getValueAsInt()
{
	if( m_nodeType == constant_type )
	{
		Variable< int >* intVar = reinterpret_cast< Variable< int >* >( m_constant );
		if( intVar != nullptr )
		{
			return intVar->m_value;
		}
	}
	else if ( m_nodeType == variable_type )
	{
		VariableTable* varTable = getTable();
		if( varTable != nullptr )
		{
			int returnInt;
			if( varTable->getVariable( m_variableName, returnInt, INT_DATA_TYPE ) )
			{
				return returnInt;
			}
		}
	}
	else
	{
		if( m_operationType == addition_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateAddition( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
			}
		}
		else if( m_operationType == multiplication_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateMultiplication( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
			}
		}
		else if( m_operationType == subtraction_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateSubtraction( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
			}
		}
		else if( m_operationType == division_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateDivision( m_leftChild->evaluateAsInt(), m_rightChild->evaluateAsInt() );
			}
		}
	}
	return 0;
}


float ExpressionNode::getValueAsFloat()
{
	if( m_nodeType == constant_type )
	{
		Variable< float >* floatVar = reinterpret_cast< Variable< float >* >( m_constant );
		if( floatVar != nullptr )
		{
			return floatVar->m_value;
		}
	}
	else if ( m_nodeType == variable_type )
	{
		VariableTable* varTable = getTable();
		if( varTable != nullptr )
		{
			float returnFloat;
			if( varTable->getVariable( m_variableName, returnFloat, FLOAT_DATA_TYPE ) )
			{
				return returnFloat;
			}
		}
	}
	else
	{
		if( m_operationType == addition_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateAddition( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
			}
		}
		else if( m_operationType == multiplication_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateMultiplication( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
			}
		}
		else if( m_operationType == subtraction_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateSubtraction( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
			}
		}
		else if( m_operationType == division_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateDivision( m_leftChild->evaluateAsFloat(), m_rightChild->evaluateAsFloat() );
			}
		}
	}
	return 0.f;
}


std::string ExpressionNode::getValueAsString()
{
	if( m_nodeType == constant_type )
	{
		Variable< std::string >* stringVar = reinterpret_cast< Variable< std::string >* >( m_constant );
		if( stringVar != nullptr )
		{
			return stringVar->m_value;
		}
	}
	else if ( m_nodeType == variable_type )
	{
		VariableTable* varTable = getTable();
		if( varTable != nullptr )
		{
			std::string returnString;
			if( varTable->getVariable( m_variableName, returnString, STRING_DATA_TYPE ) )
			{
				return returnString;
			}
		}
	}
	else
	{
		if( m_operationType == addition_operation )
		{
			if( m_leftChild != nullptr )
			{
				return evaluateAddition( m_leftChild->evaluateAsString(), m_rightChild->evaluateAsString() );
			}
		}
	}
	return "";
}


}
