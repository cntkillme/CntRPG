#pragma once

#include <exception>
#include <string>

namespace sql
{
	/**
	 * @brief Exception class thrown on SQLite3 API errors or Database/Statement errors.
	 */
	class Exception : public std::exception
	{
	private:
		int mErrorCode;
		std::string mErrorMessage;

	public:
		/**
		 * @brief Constructs an exception given an SQLite3 error code.
		 * @param errorCode the error code.
		 * The error message generated is based on sqlite3_errstr.
		 */
		explicit Exception(int errorCode) noexcept;

		/**
		 * @brief Constructs an exception given an SQLite3 error code.
		 * @param errorCode the error code.
		 * @param message a custom error message.
		 */
		explicit Exception(int errorCode, std::string message) noexcept;

		/**
		 * @brief Returns the error message.
		 * @return The error message.
		 */
		const char* what() const noexcept override;

		/**
		 * @brief Returns the error code.
		 * @return The error code.
		 */
		int code() const noexcept;
	};
}
