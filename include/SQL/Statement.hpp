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

		// todo: Bind(int, T&&)
		// todo: Bind(const char*, T&&)
		// todo: step/execute
		// todo: operator bool (returns true if has more rows)

		/**
		 * Resets the statement and optionally clears all bindings.
		 */
		void Reset(bool clearBindings = true) noexcept;

	private:
		static sqlite3_stmt* Create(Database& database, std::string query, bool persistent);

		Database& database;

		std::unique_ptr<sqlite3_stmt, DeleterFactory<sqlite3_stmt, sqlite3_finalize>> handle;
	};
}
