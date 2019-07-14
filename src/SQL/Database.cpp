#include "SQL/Database.hpp"

// SQL::Database
namespace SQL
{
	Database::Database(Properties properties)
	: handle(Create(std::move(properties)), sqlite3_close_v2)
	{
	}

	sqlite3* Database::Create(Properties properties)
	{
		if (properties.filename == nullptr) {
			if (properties.memory)
				properties.filename = ":memory:";
			else
				throw Exception(SQLITE_ERROR, "expected filename");
		}

		sqlite3* database;
		int err = sqlite3_open_v2(properties.filename, &database, properties.GetFlags(), nullptr);

		if (err != SQLITE_OK)
			throw Exception(err);

		return database;
	}
}

// SQL::Database::Properties
namespace SQL
{
	int Database::Properties::GetFlags() const noexcept
	{
		int flags = 0;

		flags |= immutable ? SQLITE_READONLY : SQLITE_OPEN_READWRITE;
		flags |= memory ? SQLITE_OPEN_MEMORY : 0;
		flags |= create ? SQLITE_OPEN_CREATE : 0;
		flags |= synchronized ? SQLITE_OPEN_FULLMUTEX : SQLITE_OPEN_NOMUTEX;
		flags |= shared ? SQLITE_OPEN_SHAREDCACHE : SQLITE_OPEN_PRIVATECACHE;

		return flags;
	}
}
