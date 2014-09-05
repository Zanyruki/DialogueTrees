#pragma once
#include <string>

namespace gh
{
	enum DataType
	{
		BOOL_DATA_TYPE,
		INT_DATA_TYPE,
		FLOAT_DATA_TYPE,
		STRING_DATA_TYPE,
		RANDOMRANGE_DATA_TYPE,
		RANDOMCHOICE_DATA_TYPE,
		NULL_DATA_TYPE
	};

	

	class VariableBase
	{
	public:
		inline DataType getDataType()
		{
			return m_dataType;
		}

		DataType m_dataType;
		std::string m_variableName;
	};
}