#pragma once

#include <string>
#include <sqlite3.h>
#include "Exception.hpp"

namespace SQL
{
	/**
	 * The Database object encapsulates an SQLite3 database connection.
	 */
	class Database
	{
		class Properties;

	public:

		/**
		 * Creates a disconnected Database object.
		 */
		Database() noexcept = default;

		/**
		 * Creates a connected Database object.
		 * Equivilent to calling Connect(properties) on a disconnected Database.
		 */
		Database(Properties properties);

		/**
		 * Database objects are not copyable.
		 */
		Database(const Database&) = delete;

		/**
		 * Move constructor.
		 */
		Database(Database&& database) noexcept;

		/**
		 * Disconnects from the connected database.
		 * Equivilent to calling Disconnect().
		 */
		~Database() noexcept;

		/**
		 * Connects to an SQLite3 database with the given properties.
		 * Throws SQL::SQLException exception if the connection failed.
		 */
		void Connect(Properties properties);

		/**
		 * Disconnects from the connected database.
		 */
		void Disconnect() noexcept;

		/**
		 * Returns true if there is a connection to a database.
		 */
		operator bool() const noexcept;

		/**
		 * Database objects are not copyable.
		 */
		Database& operator=(const Database& database) = delete;

		/**
		 * Move assignment operator.
		 */
		Database& operator=(Database&& database) noexcept;

	private:
		sqlite3* handle = nullptr;

		/**
		 * Specifies connection details, allows aggregate initialization.
		 * The filename should live until the object is no longer needed.
		 */
		struct Properties
		{
			const char* filename = "";
			bool immutable = false;
			bool memory = false;
			bool create = false;
			bool synchronized = false;
			bool shared = false;

			/**
			 * Encodes properties into flags to be used in sqlite3_open_v2.
			 */
			int GetFlags() const noexcept;
		};
	};
}
