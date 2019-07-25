#pragma once

namespace SQL
{
	/**
	 * Defines encoding and decoding blob types through the use of:
	 *  Decode as `static T(std::stringstream)`
	 *
	 * The blob is generated on construction.
	 */
	template<typename ValueType, typename = void>
	struct Blob { };

	/**
	 * Deduction guide to allow Blob(T) to select Blob<T>() and not Blob<T, void>().
	 */
	template<typename ValueType>
	Blob(ValueType) -> Blob<ValueType>;

	template<typename Type, typename = void>
	struct StripBlob
	{
		using type = Type;
	};

	template<typename Type>
	struct StripBlob<Blob<Type>>
	{
		using type = Type;
	};

	/**
	 * Handle reference types via elusion (Bridge<T&> -> Bridge<T>)
	 */
	template<typename ValueType>
	struct Blob<ValueType, std::enable_if_t<std::is_reference_v<ValueType>>> { };

	/**
	 * Handles const T* where T can be converted to a char.
	 */
	template<typename Type>
	struct Blob<const Type*, std::enable_if_t<std::is_convertible_v<Type, char>>>
	{
		std::string blob;

		Blob(const Type* buffer, std::size_t size) noexcept
		: blob(buffer, size) { };

		static std::string Decode(std::string blob)
		{
			return blob;
		}
	};

	template<typename Class>
	struct Blob<Class, std::void_t<
		std::enable_if_t<std::is_invocable_r_v<std::string, decltype(Class::ToBlob), Class>>,
		std::enable_if_t<std::is_invocable_r_v<Class, decltype(Class::FromBlob), std::string>>>>
	{
		std::string blob;

		Blob(Class object)
		: blob(std::move(Class::ToBlob(std::move(object)))) { }

		static Class Decode(std::string blob)
		{
			return Class::FromBlob(std::move(blob));
		}
	};
}
