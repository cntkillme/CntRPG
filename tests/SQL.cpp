#include "PrecompiledHeader.hpp"
#include <sstream>
#include "SQL/Database.hpp"
#include "SQL/Statement.hpp"
#include "SQL/Exception.hpp"

struct Vector3
{
	float x , y, z;

	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	: x(x)
	, y(y)
	, z(z) { }

	static std::string ToBlob(Vector3 vec)
	{
		std::stringstream blob;
		blob.write(reinterpret_cast<const char*>(&vec.x), sizeof(float));
		blob.write(reinterpret_cast<const char*>(&vec.y), sizeof(float));
		blob.write(reinterpret_cast<const char*>(&vec.z), sizeof(float));
		return blob.str();
	}

	static Vector3 FromBlob(std::string blob)
	{
		Vector3 vec;

		if (blob.size() != sizeof(float)*3)
			return { };

		std::stringstream chunk(blob);
		chunk.read(reinterpret_cast<char*>(&vec.x), sizeof(float));
		chunk.read(reinterpret_cast<char*>(&vec.y), sizeof(float));
		chunk.read(reinterpret_cast<char*>(&vec.z), sizeof(float));

		return vec;
	}
};

std::ostream& operator<<(std::ostream& os, const Vector3& vector3)
{
	os << "(";
	os << vector3.x << ", ";
	os << vector3.y << ", ";
	os << vector3.z << ")";

	return os;
}

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
		"cash REAL NOT NULL DEFAULT 0,"
		"pos BLOB"
		");").Execute();

	{
		SQL::Statement inserter(database, "INSERT INTO dab (name, cash, pos) VALUES (?, ?, ?);", true);

		for (int i = 0; i < 10; i++)
			inserter.BindMany(1,
					"Player" + std::to_string(i),
					i * 1000,
					SQL::Blob(Vector3(i, i, i)))
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
			auto [id, name, cash, pos] = statement.Results<long long, std::string, double, SQL::Blob<Vector3>>();

			// alternatively: auto id = statement.Result<long long>(0); and so on

			std::cout << "id = " << id.value() << ", ";
			std::cout << "name = " << name.value() << ", ";
			std::cout << "cash = " << cash.value() << ", ";
			std::cout << "pos = " << pos.value() << "\n";
		});
	}

	return 0;
}
