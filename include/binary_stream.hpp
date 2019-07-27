#pragma once

#include <sstream>
#include <type_traits>
#include <utility>

class BinaryStream
{
private:
	std::stringstream stream;

public:
	BinaryStream() noexcept(std::is_nothrow_default_constructible_v<std::stringstream>) { };

	template<typename T>
	BinaryStream& operator<<(T&& value)
	{
	}
};
