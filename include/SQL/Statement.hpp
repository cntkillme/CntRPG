#pragma once

#include <memory>
#include <string>
#include <sqlite3.h>
#include "Exception.hpp"

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

		/**
		 * Statement objects are not copyable.
		 */
		Statement(const Statement&) = delete;

		/**
		 * Move constructor.
		 */
		Statement(Statement&& statement) noexcept = default;

		// todo: Bind(int, T&&)
		// todo: Bind(const char*, T&&)
		// todo: execute, step, results
		// todo: clear bindings, reset
		// todo: operator bool (returns true if has more rows)

		/**
		 * Statement objects are not copyable.
		 */
		Statement& operator=(const Statement&) = delete;

		/**
		 * Move assignment operator.
		 */
		Statement& operator=(Statement&& statement) noexcept = default;

	private:
		Database& database;

		std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> handle;

		static sqlite3_stmt* Create(Database& database, std::string query, bool persistent);
	};
}
