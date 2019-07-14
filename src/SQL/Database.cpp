#include "Database.hpp"

// SQL::Database
namespace SQL
{
	Database::Database(Properties properties)
	{
		Connect(std::move(properties));
	}

	Database::Database(Database&& database) noexcept
	: handle(database.handle)
	{
		database.handle = nullptr;
	}

	Database::~Database() noexcept
	{
		Disconnect();
	}

	void Database::Connect(Properties properties)
	{
		if (handle == nullptr)
			throw SQLException(SQLITE_ERROR, "already connected");

		if (properties.filename == nullptr) {
			if (properties.memory)
				properties.filename = ":memory:";
			else
				throw SQLException(SQLITE_ERROR, "expected filename");
		}

		int err = sqlite3_open_v2(properties.filename, &handle, properties.GetFlags(), nullptr);

		if (err != SQLITE_OK)
			throw SQLException(err);
	}

	void Database::Disconnect() noexcept
	{
		sqlite3_close_v2(handle);
		handle = nullptr;
	}

	Database::operator bool() const noexcept
	{
		return handle != nullptr;
	}

	Database& Database::operator=(Database&& database) noexcept
	{
		handle = database.handle;
		database.handle = nullptr;
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
