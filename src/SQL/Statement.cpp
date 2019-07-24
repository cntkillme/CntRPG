#include "PrecompiledHeader.hpp"
#include "SQL/Database.hpp"
#include "SQL/Statement.hpp"

// SQL::Statement
namespace SQL
{
	Statement::Statement(Database& database, std::string_view query, bool persistent)
	: database(database)
	, handle(Create(database, std::move(query), persistent))
	{
	}

	bool Statement::Step()
	{
		int err = sqlite3_step(handle.get());

		hasRow = false;

		if (err == SQLITE_ROW)
			hasRow = true;
		else if (err == SQLITE_DONE)
			return false;
		else
			throw SQL::Exception(err);

		return true;
	}

	void Statement::Reset(bool clearBindings) noexcept
	{
		hasRow = false;

		if (clearBindings)
			sqlite3_clear_bindings(handle.get());
		sqlite3_reset(handle.get());
	}

	sqlite3_stmt* Statement::Create(Database& database, std::string_view query, bool persistent)
	{
		sqlite3_stmt* stmt;
		int err = sqlite3_prepare_v3(database.handle.get(), query.data(), query.size(),
			persistent ? SQLITE_PREPARE_PERSISTENT : 0, &stmt, nullptr);

		if (err != SQLITE_OK)
			throw Exception(err);

		return stmt;
	}

	Statement::operator bool() const noexcept
	{
		return hasRow;
	}
}
