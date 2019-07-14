#include <utility>
#include "SQL/Database.hpp"
#include "SQL/Statement.hpp"

// SQL::Statement
namespace SQL
{
	Statement::Statement(Database& database, std::string query, bool persistent)
	: database(database)
	, handle(Create(database, std::move(query), persistent), &sqlite3_finalize)
	{
	}

	sqlite3_stmt* Statement::Create(Database& database, std::string query, bool persistent)
	{
		sqlite3_stmt* stmt;
		int err = sqlite3_prepare_v3(database.handle.get(), query.data(), query.size(),
			persistent ? SQLITE_PREPARE_PERSISTENT : 0, &stmt, nullptr);

		if (err != SQLITE_OK)
			throw Exception(err);

		return stmt;
	}
}
