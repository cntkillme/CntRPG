#pragma once

namespace SQL
{
	class Statement;

	template<typename ValueType>
	struct Bridge { };

	template<typename ValueType>
	void Bind(Statement& statement, int parameter, const ValueType& value)
	{
		if (parameter > 0 && parameter < sqlite3_bind_parameter_count(statement))
			Bridge<ValueType>::bind(statement, parameter, value);
		else
			throw SQL::Exception(SQLITE_MISMATCH, std::string("bad parameter index ") + std::to_string(parameter));
	}

	template<typename ValueType>
	void Bind(Statement& statement, const char* name, const ValueType& value)
	{
		int index = sqlite3_bind_parameter_index(statement, name);
		
		if (index > 0)
			Bridge<ValueType>::bind(statement, index, value);
		else
			throw SQL::Exception(SQLITE_MISMATCH, std::string("bad parameter name ") + name);
	}

	template<typename ValueType>
	std::optional<ValueType> Result(Statement& statement, int column)
	{
		if (column >= 0 && column < sqlite3_column_count(statement))
			return Bridge<ValueType>::result(statement, column);
		else
			throw SQL::Exception(SQLITE_MISMATCH, std::string("bad column index ") + std::to_string(column));
	}
}