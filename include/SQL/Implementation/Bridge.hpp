#pragma once

#include "../Blob.hpp"

namespace SQL
{
	class Statement;

	namespace Implementation
	{
		/**
		 * Defines binding and fetching behavior through the use of:
		 *  Bind as `void(sqlite3_stmt* statement, int parameter, ValueType&& value)`
		 *  Result as `std::optional<ValueType>(sqlite3_stmt* statement, int column)`
		 */
		template<typename ValueType, typename = void>
		struct Bridge { };

		/**
		 * Handle reference types via elusion (Bridge<T&> -> Bridge<T>)
		 */
		template<typename ValueType>
		struct Bridge<ValueType, std::enable_if_t<std::is_reference_v<ValueType>>> :
			Bridge<std::remove_reference_t<ValueType>> { };

		/**
		 * Handle null via nullptr.
		 */
		template<>
		struct Bridge<std::nullptr_t>
		{
			static void Bind(sqlite3_stmt* statement, int parameter, std::nullptr_t)
			{
				sqlite3_bind_null(statement, parameter);
			}

			static std::optional<nullptr_t> Result(sqlite3_stmt* statement, int column)
			{
				if (sqlite3_column_type(statement, column) == SQLITE_NULL)
					return { nullptr };
				else
					return { };
			}
		};

		/**
		 * Handle bool via int.
		 */
		template<>
		struct Bridge<bool>
		{
			static void Bind(sqlite3_stmt* statement, int parameter, bool value)
			{
				sqlite3_bind_int(statement, parameter, static_cast<int>(value));
			}

			static std::optional<bool> Result(sqlite3_stmt* statement, int column)
			{
				if (sqlite3_column_type(statement, column) != SQLITE_INTEGER)
					return { };
				else
					return { sqlite3_column_int(statement, column) != 0 };
			}
		};

		/**
		 * Handle all integral types.
		 */
		template<typename Integral>
		struct Bridge<Integral, std::enable_if_t<std::is_integral_v<Integral>>>
		{
			static void Bind(sqlite3_stmt* statement, int parameter, Integral value)
			{
				if constexpr (sizeof(Integral) <= 4)
					sqlite3_bind_int(statement, parameter, static_cast<int>(value));
				else
					sqlite3_bind_int64(statement, parameter, static_cast<sqlite3_int64>(value));
			}

			static std::optional<Integral> Result(sqlite3_stmt* statement, int column)
			{
				int type = sqlite3_column_type(statement, column);

				if (type != SQLITE_INTEGER && type != SQLITE_FLOAT)
					return { };
				else {
					if constexpr (sizeof(Integral) <= 4)
						return { sqlite3_column_int(statement, column) };
					else
						return { sqlite3_column_int64(statement, column) };
				}
			}
		};

		/**
		 * Handle all real types.
		 */
		template<typename Real>
		struct Bridge<Real, std::enable_if_t<std::is_floating_point_v<Real>>>
		{
			static void Bind(sqlite3_stmt* statement, int parameter, Real value)
			{
				static_assert(sizeof(Real) <= sizeof(double), "floating-point type too large");
				sqlite3_bind_double(statement, parameter, value);
			}

			static std::optional<Real> Result(sqlite3_stmt* statement, int column)
			{
				int type = sqlite3_column_type(statement, column);

				if (type != SQLITE_INTEGER && type != SQLITE_FLOAT)
					return { };
				else
					return { sqlite3_column_double(statement, column) };
			}
		};

		/**
		 * Handles text.
		 */
		template<typename Text>
		struct Bridge<Text, std::enable_if_t<std::is_convertible_v<Text, std::string_view>>>
		{
			static void Bind(sqlite3_stmt* statement, int parameter, std::string_view value)
			{
				sqlite3_bind_text(statement, parameter, value.data(), value.size(), SQLITE_TRANSIENT);
			}

			static std::optional<std::string> Result(sqlite3_stmt* statement, int column)
			{
				int type = sqlite3_column_type(statement, column);

				if (type == SQLITE_NULL || type == SQLITE_BLOB)
					return { };
				else
					return { reinterpret_cast<const char*>(sqlite3_column_text(statement, column)) };
			}
		};

		/**
		 * Handles blob.
		 */
		template<typename Class>
		struct Bridge<Blob<Class>>
		{
			static void Bind(sqlite3_stmt* statement, int parameter, const Blob<Class>& value)
			{
				std::string blob = value.blob;
				sqlite3_bind_blob(statement, parameter, blob.data(), blob.size(), SQLITE_TRANSIENT);
			}

			static std::optional<Class> Result(sqlite3_stmt* statement, int column)
			{
				int type = sqlite3_column_type(statement, column);

				if (type != SQLITE_BLOB)
					return { };
				else
					return { Blob<Class>::Decode(
						std::string(
							reinterpret_cast<const char*>(sqlite3_column_blob(statement, column)),
							sqlite3_column_bytes(statement, column)
						)) };
			}
		};
	}
}
