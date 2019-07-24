#pragma once

#include "Implementation/Bridge.hpp"
#include "Exception.hpp"
#include "Utility.hpp"

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
		Statement(Database& database, std::string_view query, bool persistent = false);

		/**
		 * Binds a value to the given bind parameter index (1-based).
		 */
		template<typename ValueType>
		Statement& Bind(int parameter, ValueType&& value)
		{
			if (parameter > 0 && parameter <= sqlite3_bind_parameter_count(handle.get()))
				Bridge<ValueType>::bind(handle.get(), parameter, std::forward<ValueType>(value));
			else
				throw SQL::Exception(SQLITE_MISMATCH, std::string("bad parameter index ") + std::to_string(parameter));

			return *this;
		}

		/**
		 * Binds a value to the given bind parameter name (1-based).
		 */
		template<typename ValueType>
		Statement& Bind(const char* name, ValueType&& value)
		{
			int index = sqlite3_bind_parameter_index(handle.get(), name);

			if (index > 0)
				Bridge<ValueType>::bind(handle.get(), index, std::forward<ValueType>(value));
			else
				throw SQL::Exception(SQLITE_MISMATCH, std::string("bad parameter name ") + name);

			return *this;
		}

		/**
		 * Binds multiple parameters at once starting from the given parameter index (1-based).
		 */
		template<typename... ValueTypes>
		Statement& BindMany(int parameter, ValueTypes&&... values)
		{
			BindManyImpl<ValueTypes...>(parameter, std::forward<ValueTypes>(values)...,
				std::make_index_sequence<sizeof...(ValueTypes)>());

			return *this;
		}

		/**
		 * Fetches the value stored in the given column (0-based).
		 */
		template<typename ValueType>
		std::optional<ValueType> Result(int column)
		{
			if (!hasRow)
				throw SQL::Exception(SQLITE_ERROR, "no row available");

			if (column >= 0 && column < sqlite3_column_count(handle.get()))
				return Bridge<ValueType>::result(handle.get(), column);
			else
				throw SQL::Exception(SQLITE_MISMATCH, std::string("bad column index ") + std::to_string(column));
		}

		/**
		 * Fetches multiple values starting from the given column (0-based).
		 */
		template<typename... ValueTypes>
		std::tuple<std::optional<ValueTypes>...> Results(int column = 0)
		{
			static_assert(sizeof...(ValueTypes) > 0, "expected at least 1 type");
			return ResultsImpl<ValueTypes...>(column,
				std::make_index_sequence<sizeof...(ValueTypes)>());
		}

		/**
		 * Continues to step until no more rows are available.
		 * Resets the statement at the end but does not clear bindings.
		 */
		void Execute()
		{
			Execute([](auto&&...) constexpr noexcept { });
		}

		/**
		 * Calls the user-given callback for each row.
		 * Use Utility::MonostateFunction if the callback is a function pointer.
		 * Resets the statement at the end but does not clear bindings.
		 */
		template<typename Callable>
		void Execute(Callable&& callback)
		{
			long row = 1;

			while (Step())
				std::forward<Callable>(callback)(*this, row++);

			Reset(false);
		}

		/**
		 * Fetch the next row, returns true if the row is available.
		 */
		bool Step();

		/**
		 * Resets the statement and optionally clears all bindings.
		 */
		void Reset(bool clearBindings = true) noexcept;

		/**
		 * Returns true if more rows are available.
		 */
		operator bool() const noexcept;

	private:
		static sqlite3_stmt* Create(Database& database, std::string_view query, bool persistent);

		Database& database;
		bool hasRow = false;
		std::unique_ptr<sqlite3_stmt, Utility::MonostateFunction<sqlite3_finalize>> handle;

		template<typename... ValueTypes, std::size_t... Indexes>
		void BindManyImpl(int parameter, ValueTypes&&... values, std::index_sequence<Indexes...>)
		{
			(... , Bind(parameter + Indexes, std::forward<ValueTypes>(values)));
		}

		template<typename... ValueTypes, std::size_t... Indexes>
		std::tuple<std::optional<ValueTypes>...> ResultsImpl(int column, std::index_sequence<Indexes...>)
		{
			return { Result<ValueTypes>(column + Indexes)... };
		}
	};
}
