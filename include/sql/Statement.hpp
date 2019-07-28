#pragma once

#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <memory>
#include <sqlite3.h>
#include <tuple>
#include "monostate_function.hpp"

namespace sql
{
	class Database;

	/**
	 * @brief SQLite3 statement wrapper.
	 *
	 * This class exposes a modern C++ interface to an SQLite3 statement.
	 * The UTF-16 encoding scheme is not supported.
	 * The underlying SQLite3 statement is finalized on destruction.
	 *
	 * A database must be specified before any binding or evaluation can take place.
	 */
	class Statement
	{
	private:
		/**
		 * @brief Describes the current state of the Statement.
		 *
		 * Statement progression is handled through a state machine with the following transititons:
		 *  SQL -> SQL on alteration of the SQL query.
		 *  Bind/Row/Done -> SQL on a call to clearBindings.
		 *  SQL/Bind -> Bind on a call to bind.
		 *  Row/Done -> Bind on a call to reset.
		 *  SQL/Bind/Row/Done -> Row on a call to step if a row of data is available.
		 *  SQL/Bind/Row/Done -> Done on a call to step/execute if no row of data is available.
		 * 
		 * The statement is in an "initialized" state if the sqlite3_stmt object has been created. This is
		 *  guaranteed when the statement's state is either Bind, Row, or Done.
		 */
		enum class State
		{
			SQL, ///< The SQL query is alterable.
			Bind, ///< The SQL query is fixed.
			Row, ///< A row of data is available.
			Done ///< The statement is fully evaluated.
		};

	public:
		friend std::ostream& operator<<(std::ostream&, const Statement& stmt);

	public:
		/**
		 * @brief Constructs a detached Statement object.
		 */
		explicit Statement() noexcept = default;

		/**
		 * @brief Constructs an attached Statement object.
		 * @param database the database that the statement will be attached to.
		 * @param query the query to initialize the statement with.
		 */
		explicit Statement(Database& database, std::string query = std::string()) noexcept;

		/**
		 * @brief Returns whether the statement is initialized.
		 * @return true if the statement is initialized, false otherwise.
		 */
		bool initialized() const noexcept;
		
		/**
		 * @brief Returns a reference to the associated database.
		 * @return The database.
		 * @throw Exception if no database is available.
		 */
		Database& database();
		
		/**
		 * @brief Returns a constant reference to the associated database.
		 * @return The database.
		 * @throw Exception if no database is available.
		 */
		const Database& database() const;

		/**
		 * @brief Clears the SQL query.
		 * @throw Exception if the statement is initialized.
		 */
		void clear();
		
		/**
		 * @brief Clears all bindings. Does nothing if the statement is not initialized.
		 */
		void clearBindings() noexcept;

		/**
		 * @brief Resets the statement. Does nothing if the statement is not initialized.
		 */
		void reset() noexcept;
		
		/**
		 * @brief Associates the statement with a database.
		 * @param database the database.
		 * @throw Exception if the statement is initialized.
		 */
		void attach(Database& database);

		/**
		 * @brief Disassociates the statement from the database.
		 * @throw Exception if the statement is initialized.
		 */
		void detach();

		/**
		 * @brief Returns the SQL query.
		 * @return The SQL query.
		 */
		const std::string& query() const noexcept;

		// invoke reset if state is done
		/**
		 * @brief Evaluate the query to the next row.
		 * 
		 */
		void next();

		// invoke reset if state is row or done
		void run();

		template<typename T>
		void bind(int index, T&& value);

		template<typename T>
		void bind(std::string_view parameter, T&& value);


		/**
		 * @brief Appends to the SQL query.
		 * @param sql SQL to append.
		 * @return The statement.
		 * @throws Exception if the statement is not reset.
		 */
		Statement& operator<<(std::string_view sql);

		template<typename T>
		std::optional<T> result(int column);

		template<typename... Ts>
		std::tuple<std::optional<Ts>...> results(int firstColumn = 0);

	private:
		void assertInitialized(bool expected, std::optional<std::string> errorMessage = std::nullopt) const;

	private:
		State mState = State::SQL;
		Database* mDatabase;
		std::string mQuery;
		std::unique_ptr<sqlite3_stmt, monostate_function<sqlite3_finalize>> mStmtHandle;
	};
}
