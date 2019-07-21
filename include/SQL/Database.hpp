#pragma once

#include "Deleter.hpp"

namespace SQL
{
	/**
	 * The Database object encapsulates an SQLite3 database connection.
	 */
	class Database
	{
	public:
		/**
		 * Specifies connection details, allows aggregate initialization.
		 */
		struct Properties
		{
			std::string filename = "";
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

		/**
		 * Connects to a database using the given properties.
		 * Throws SQL::Exception with the SQLite3 error code if connection failed.
		 */
		Database(Properties properties);

		/**
		 * Implicit conversion to allow usage of this object directly by the C API.
		 */
		operator sqlite3*() noexcept;

	private:
		friend class Statement;

		static sqlite3* Create(Properties properties);

		std::unique_ptr<sqlite3, DeleterFactory<sqlite3, sqlite3_close_v2>> handle;
	};
}
