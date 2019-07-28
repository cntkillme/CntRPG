#pragma once

#include <memory>
#include <sqlite3.h>
#include "monostate_function.hpp"

namespace sql
{
	/**
	 * @brief SQLite3 connection wrapper.
	 *
	 * This class exposes a modern C++ interface to an SQLite3 database.
	 * A user-specified VFS (virtual filesystem) is not supported.
	 * The UTF-16 encoding scheme is not supported.
	 * The underlying SQLite3 connection is closed on destruction.
	 */
	class Database
	{
	public:
		/// Database connection flag to open an immutable database.
		static constexpr int READONLY = SQLITE_OPEN_READONLY;

		/// Database connection flag to open a mutable database.
		static constexpr int READWRITE = SQLITE_OPEN_READWRITE;

		/// Database connection flag to create the database file if it does not exist.
		static constexpr int CREATE = SQLITE_OPEN_CREATE;

		/// Database connection flag to allow URI interpretation.
		static constexpr int URI = SQLITE_OPEN_URI;

		/// Database connection flag to open a transient, in-memory database.
		static constexpr int MEMORY = SQLITE_OPEN_MEMORY;

		/// Database connection flag to open the connection in multi-thread threading mode.
		/// Ignored if single-thread threading mode is set.
		static constexpr int NOMUTEX = SQLITE_OPEN_NOMUTEX;

		/// Database connection flag to open the connection in serialized threading mode.
		/// Ignored if single-thread threading mode is set.
		static constexpr int FULLMUTEX = SQLITE_OPEN_FULLMUTEX;

		/// Database connection flag to enable shared cache mode.
		static constexpr int SHAREDCACHE = SQLITE_OPEN_SHAREDCACHE;

		/// Database connection flag to disable shared cache mode.
		static constexpr int PRIVATECACHE = SQLITE_OPEN_PRIVATECACHE;

		/// The default flags used if none is specified.
		static constexpr int DEFAULT = READWRITE | URI;

	public:
		/**
		 * @brief Constructs an unconnected Database object.
		 */
		explicit Database() noexcept = default;

		/**
		 * @brief Constructs a connected Database object, terminates if a connection cannot be made.
		 * @sa connect(const char*, int)
		 */
		explicit Database(const char* filename, int flags = DEFAULT) noexcept;

		/**
		 * @brief Opens an SQLite3 connection.
		 * @param filename database filename (UTF-8).
		 * @param flags database open flags.
		 * @return The result code.
		 * @throw Exception with SQLITE_MISUSE if a connection is already established.
		 * @throw Exception with the error code if connection failed.
		 */
		void connect(const char* filename, int flags = DEFAULT);

		/**
		 * @brief Closes the SQLite3 connection.
		 */
		void disconnect() noexcept;

		/**
		 * @brief Executes semicolon-sepeareted SQL statements in a single call. For more control, use SQL::Statement.
		 * @param query SQL statements to be executed.
		 * @returns The result code.
		 * @throw Exception with SQLITE_MISUSE if no connection is established.
		 * @throw Exception with the error code if execution failed.
		 */
		void execute(const char* query);

		/**
		 * @brief Returns true if a connection is established, false otherwise.
		 */
		operator bool() const noexcept;

	private:
		std::unique_ptr<sqlite3, monostate_function<sqlite3_close_v2>> mDbHandle;
	};
}
