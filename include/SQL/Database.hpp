#pragma once

#include <string>
#include <sqlite3.h>
#include <memory>
#include "Exception.hpp"

namespace SQL
{
	/**
	 * The Database object encapsulates an SQLite3 database connection.
	 */
	class Database
	{
	public:
		class Properties;

		/**
		 * Connects to a database using the given properties.
		 */
		Database(Properties properties);

		/**
		 * Database objects are not copyable.
		 */
		Database(const Database&) = delete;

		/**
		 * Move constructor.
		 */
		Database(Database&& database) noexcept  = default;

		/**
		 * Database objects are not copyable.
		 */
		Database& operator=(const Database&) = delete;

		/**
		 * Move assignment operator.
		 */
		Database& operator=(Database&& database) noexcept = default;

	private:
		friend class Statement;

		std::unique_ptr<sqlite3, decltype(&sqlite3_close_v2)> handle;

		static sqlite3* Create(Properties properties);

	public:
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
