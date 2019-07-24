#include "PrecompiledHeader.hpp"
#include "SQL/Database.hpp"
#include "SQL/Statement.hpp"
#include "SQL/Exception.hpp"

int main(int argc, char** argv)
{
	SQL::Database database({
		.filename = "memdb",
		.immutable = false,
		.memory = true,
		.create = true
	});

	SQL::Statement(database, "CREATE TABLE dab (" \
		"ID INTEGER PRIMARY KEY," \
		"name TEXT NOT NULL DEFAULT \"\"," \
		"cash REAL NOT NULL DEFAULT 0"
		");").Execute();

	{
		SQL::Statement inserter(database, "INSERT INTO dab (name, cash) VALUES (?, ?);", true);

		for (int i = 0; i < 10; i++)
			inserter
				.BindMany(1, "Player" + std::to_string(i), i * 1000)
				.Execute();
	}

	{
		SQL::Statement fetch(database, "SELECT * FROM dab");

		{
			fetch.Step();

			auto columns = fetch.GetColumns();
			std::cout << "Columns:\n";

			for (auto&& [v, k] : columns)
				std::cout << k << " at index " << v << "\n";

			fetch.Reset();
		}

		std::cout << "\n";

		fetch.Execute([](SQL::Statement& statement, long row) {
			auto [id, name, cash] = statement.Results<long long, std::string, double>();

			// alternatively: auto id = statement.Result<long long>(0); and so on

			std::cout << "id = " << id.value() << ", ";
			std::cout << "name = " << name.value() << ", ";
			std::cout << "cash = " << cash.value() << "\n";
		});
	}

	return 0;
}
