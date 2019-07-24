#pragma once

namespace SQL
{
	class Statement;

	/**
	 * Defines binding and fetching behavior through the use of:
	 *  bind as `void(sqlite3_stmt*, int parameter, ValueType&& value)`
	 *  result as `std::optional<ValueType>(sqlite3_stmt*, int column)`
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
		static void bind(sqlite3_stmt* stmt, int parameter, std::nullptr_t)
		{
			sqlite3_bind_null(stmt, parameter);
		}

		static std::optional<nullptr_t> result(sqlite3_stmt* stmt, int column)
		{
			if (sqlite3_column_type(stmt, column) == SQLITE_NULL)
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
		static void bind(sqlite3_stmt* stmt, int parameter, bool value)
		{
			sqlite3_bind_int(stmt, parameter, static_cast<int>(value));
		}

		static std::optional<bool> result(sqlite3_stmt* stmt, int column)
		{
			if (sqlite3_column_type(stmt, column) != SQLITE_INTEGER)
				return { };
			else
				return { sqlite3_column_int(stmt, column) != 0 };
		}
	};

	/**
	 * Handle all integral types.
	 */
	template<typename Integral>
	struct Bridge<Integral, std::enable_if_t<std::is_integral_v<Integral>>>
	{
		static void bind(sqlite3_stmt* stmt, int parameter, Integral value)
		{
			if constexpr (sizeof(Integral) <= 4)
				sqlite3_bind_int(stmt, parameter, static_cast<int>(value));
			else
				sqlite3_bind_int64(stmt, parameter, static_cast<sqlite3_int64>(value));
		}

		static std::optional<Integral> result(sqlite3_stmt* stmt, int column)
		{
			int type = sqlite3_column_type(stmt, column);

			if (type != SQLITE_INTEGER && type != SQLITE_FLOAT)
				return { };
			else {
				if constexpr (sizeof(Integral) <= 4)
					return { sqlite3_column_int(stmt, column) };
				else
					return { sqlite3_column_int64(stmt, column) };
			}
		}
	};

	/**
	 * Handle all real types.
	 */
	template<typename Real>
	struct Bridge<Real, std::enable_if_t<std::is_floating_point_v<Real>>>
	{
		static void bind(sqlite3_stmt* stmt, int parameter, Real value)
		{
			static_assert(sizeof(Real) <= sizeof(double), "floating-point type too large");
			sqlite3_bind_double(stmt, parameter, value);
		}

		static std::optional<Real> result(sqlite3_stmt* stmt, int column)
		{
			int type = sqlite3_column_type(stmt, column);

			if (type != SQLITE_INTEGER && type != SQLITE_FLOAT)
				return { };
			else
				return { sqlite3_column_double(stmt, column) };
		}
	};

	/**
	 * Handles text.
	 */
	template<typename Text>
	struct Bridge<Text, std::enable_if_t<std::is_convertible_v<Text, std::string_view>>>
	{
		static void bind(sqlite3_stmt* stmt, int parameter, std::string_view value)
		{
			sqlite3_bind_text(stmt, parameter, value.data(), value.size(), SQLITE_TRANSIENT);
		}

		static std::optional<std::string> result(sqlite3_stmt* stmt, int column)
		{
			int type = sqlite3_column_type(stmt, column);

			if (type == SQLITE_NULL || type == SQLITE_BLOB)
				return { };
			else
				return { reinterpret_cast<const char*>(sqlite3_column_text(stmt, column)) };
		}
	};
}
