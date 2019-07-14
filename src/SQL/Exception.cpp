#include <sqlite3.h>
#include "SQL/Exception.hpp"

// SQL::Exception
namespace SQL
{
	Exception::Exception(int errorCode) noexcept
	: errorCode(errorCode)
	, message(sqlite3_errstr(errorCode))
	{
	}

	Exception::Exception(int errorCode, std::string message) noexcept
	: errorCode(errorCode)
	, message(std::move(message))
	{
	}

	const char* Exception::what() const noexcept
	{
		return message.data();
	}

	int Exception::code() const noexcept
	{
		return errorCode;
	}
}
