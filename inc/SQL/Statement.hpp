#pragma once

#include <string>
#include <string_view>
#include <sstream>
#include <sqlite3.h>
#include "Exception.hpp"

namespace SQL
{
	class Database;

	class Statement
	{
	private:
		Database& database;
		sqlite3_stmt* handle;
		std::string query;

	public:
		Statement(Database& database) noexcept;

		Statement(Database& database, std::string query) noexcept;

		~Statement();

		// bind

		// results

		const std::string& GetQuery() const noexcept;

		void Clear() noexcept;

		operator bool() const noexcept;

		Statement& operator<<(std::string_view query);
	};

	std::ofstream& operator<<(std::ofstream& stream, const Statement& statement)
	{
		stream << statement.GetQuery();
		return stream;
	}
}
