#pragma once

#include "Deleter.hpp"

namespace SQL
{
	class Database;

	/**
	 * The Statement object encapsulates an SQLite3 statement.
	 */
	class Statement
	{
	public:
		/**
		 * Creates a statement of the given query.
		 * The persistent argument hints to SQLite3 that the statement will be retained for a long time.
		 */
		Statement(Database& database, std::string query, bool persistent = false);

		template<typename ParameterType, typename ValueType>
		void Bind(ParameterType&& parameter, ValueType&& value);
		
		// todo: step/execute
		// todo: operator bool (returns true if has more rows)

		/**
		 * Resets the statement and optionally clears all bindings.
		 */
		void Reset(bool clearBindings = true) noexcept;

		/**
		 * Implicit conversion to allow usage of this object directly by the C API.
		 */
		operator sqlite3_stmt*() noexcept;

	private:
		static sqlite3_stmt* Create(Database& database, std::string query, bool persistent);

		Database& database;

		std::unique_ptr<sqlite3_stmt, DeleterFactory<sqlite3_stmt, sqlite3_finalize>> handle;
	};
}

#include "Bridge.hpp"

namespace SQL
{
	template<typename ParameterType, typename ValueType>
	inline void Statement::Bind(ParameterType&& parameter, ValueType&& value)
	{
		SQL::Bind(*this, std::forward<ParameterType>(parameter), std::forward<ValueType>(value));
	}
}
