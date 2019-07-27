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
	 * @brief SQLite3 statement wrapped.
	 *
	 * This class exposes a modern C++ interface to an SQLite3 statement.
	 * The UTF-16 encoding scheme is not supported.
	 * The underlying SQLite3 statement is finalized on destruction.
	 *
	 * A database must be specified before the Bind, Row, and Done states. A call to attach is only valid in the
	 *  SQL state, else an exception will be thrown.
	 * Altering the SQL query is only valid in the SQL state, else an exception will be thrown.
	 * For information about state transitions, see Statement::State.
	 */
	class Statement
	{
	public:
		/**
		 * @brief Describes the current state of the Statement.
		 *
		 * Statement progression is handled through a state machine with the following transititons:
		 *  SQL -> SQL on alteration of the underlying SQL queryL.
		 *  Bind/Row/Done -> SQL on a call to clearBindings.
		 *  SQL/Bind -> Bind on a call to bind.
		 *  Row/Done -> Bind on a call to reset.
		 *  SQL/Bind/Row/Done -> Row on a call to step if a row of data is available.
		 *  SQL/Bind/Row/Done -> Done on a call to step/execute if no row of data is available.
		 */
		enum class State
		{
			SQL, ///< The underlying SQL query is alterable.
			Bind, ///< The .
			Row, ///< The statement has a row of data available.
			Done ///< The statement has been fully evaluated.
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
		 * @param query
		 */
		explicit Statement(Database& database, std::string query = std::string()) noexcept;

		void attach(Database& database);

		void detach() noexcept;

		Database& database();

		const Database& database() const;

		Statement& operator<<(std::string_view sql);

		// implicitly invoke reset if the state is Row or Done.
		void clearBindings() noexcept;

		void reset() noexcept;

		template<typename T>
		void bind(int index, T&& value);

		template<typename T>
		void bind(std::string_view parameter, T&& value);

		// invoke reset if state is done
		void next();

		// invoke reset if state is row or done
		void run();

		template<typename T>
		std::optional<T> result(int column);

		template<typename... Ts>
		std::tuple<std::optional<Ts>...> results(int firstColumn = 0);

	private:
		State mState = State::SQL;
		Database* mDatabase;
		std::string mQuery;
		std::unique_ptr<sqlite3_stmt, monostate_function<sqlite3_finalize>> mStmtHandle;
	}
}
