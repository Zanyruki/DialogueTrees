#pragma once
#include <string>
#include "ExpressionNode.h"
#include "VariableTable.h"
#include <map>

namespace gh
{
	class MathematicExpression
	{
	public:
		MathematicExpression( const std::string& expression );

		void setLocalVariableTable( const std::string& localTableName );
		bool evaluateAsBool();
		int evaluateAsInt();
		float evaluateAsFloat();
		std::string evaluateAsString();


		ExpressionNode* m_rootNode;

	private:
		ExpressionNode* parseExpressionString( const std::string& expression );
		ExpressionNode* parseExpressionConstantOrVariable( const std::string& dataString );
		bool isOperation( char character );
		static void initializePriorityMap();
		static bool s_bIsPriorityMapSet;
		static std::map< std::string, int > s_operationPriority;
		static std::map< std::string, OperationType > s_operationTypeMap;
		static std::map< OperationType, DataType > MathematicExpression::s_operationReternPreference;
		static int* s_operationHashMap;

	};
}