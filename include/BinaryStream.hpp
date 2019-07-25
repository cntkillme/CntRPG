#pragma once

#include "PrecompiledHeader.hpp"

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
