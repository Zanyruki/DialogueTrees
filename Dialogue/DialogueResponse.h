#pragma once
#include <vector>
#include <string>
#include "..\Parser\GHCondition.h"
namespace gh
{

	struct DialogueConditionalPath
	{
	public:
		DialogueConditionalPath( const std::string condition, const std::string nextNodeID )
			: m_condition( condition )
			, m_nodeID( nextNodeID )
		{
			
		}

		GHCondition m_condition;
		std::string m_nodeID;
		int m_nodeIndex;
	};

	class DialogueResponse
	{
	public:
		DialogueResponse( const std::string& responseText, const std::string& conditionString );
		void addPossiblePath( const std::string& conditionString, const std::string& nextNodeID );
		int getPath();

		MathematicExpression m_text;
		GHCondition m_condition;
		std::vector< DialogueConditionalPath > m_possiblePaths;

	};
}