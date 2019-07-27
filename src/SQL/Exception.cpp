#include <sqlite3.h>
#include "sql/Exception.hpp"

namespace sql
{
	Exception::Exception(int errorCode) noexcept
	: mErrorCode(errorCode)
	, mErrorMessage(sqlite3_errstr(errorCode))
	{
	}

	Exception::Exception(int errorCode, std::string message) noexcept
	: mErrorCode(errorCode)
	, mErrorMessage(std::move(message))
	{
	}

	const char* Exception::what() const noexcept
	{
		return mErrorMessage.data();
	}

	int Exception::code() const noexcept
	{
		return mErrorCode;
	}
}
