#include "sql/Database.hpp"
/*
namespace sql
{
	Database::Database(const char* filename, int flags) noexcept
	{
		connect(filename, flags).unwrap();
	}

	result_code Database::connect(const char* filename, int flags) noexcept
	{
		if (*this) // already connected
			return error(SQLITE_MISUSE, "connection already established");

		sqlite3* dbHandle;
		int errorCode = sqlite3_open_v2(filename, &dbHandle, flags, nullptr);

		if (errorCode != SQLITE_OK)
			return error(errorCode, sqlite3_errstr(errorCode));

		m_dbHandle.reset(dbHandle);

		return result_code::ok();
	}

	void Database::disconnect() noexcept
	{
		sqlite3_close_v2(m_dbHandle.release());
	}


	result_code Database::execute(const char* query) noexcept
	{
		if (!*this) // not connected
			return error(SQLITE_MISUSE, "no connection established");

		char* sqliteErrorMessage;
		int errorCode = sqlite3_exec(m_dbHandle.get(), query, nullptr, nullptr, &sqliteErrorMessage);

		if (errorCode != SQLITE_OK) {
			std::string errorMessage(sqliteErrorMessage);
			sqlite3_free(sqliteErrorMessage);
			return error(errorCode, std::move(errorMessage));
		}

		return result_code::ok();
	}

	Database::operator bool() const noexcept
	{
		return m_dbHandle.get() != nullptr;
	}
}
*/
