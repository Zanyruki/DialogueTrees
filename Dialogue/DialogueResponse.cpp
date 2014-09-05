#include "DialogueResponse.h"
#include "DialogueTree.h"
#include "DialogueLocalVariableTableManager.h"

namespace gh
{
	DialogueResponse::DialogueResponse( const std::string& responseText, const std::string& conditionString )
		: m_text( responseText )
		, m_condition( conditionString )
	{
	}


	void DialogueResponse::addPossiblePath( const std::string& conditionString, const std::string& nextNodeID )
	{
		DialogueConditionalPath newPath( conditionString, nextNodeID );
		m_possiblePaths.push_back( newPath );
	}

	//TODO
	int DialogueResponse::getPath()
	{
		if( m_possiblePaths.size() == 0 )
		{
			return -1;
		}

		for( auto pathIter = m_possiblePaths.begin(); pathIter != m_possiblePaths.end(); ++pathIter )
		{
			(*pathIter).m_condition.setLocalVariableTable( getTheDialogueLocalVariableTableManager().m_npcVariableTableName );
			
			if( (*pathIter).m_condition.evaluate() )
			{
				return pathIter->m_nodeIndex;
			}
			
		}
		return 0;
	}
}