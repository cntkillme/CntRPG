# CntRPG

## Building
A compiler with C++17 support is required (recommended: gcc-7 or later).

## To-Do
1. Design and Implement `sql::Exception`.
1. Design `sql::Statement` (bind/result method placeholders).
2. Implement `sql::Database` and `sql::Statement`.
3. Implement `sql::Value`, finish implementing `sql::Statement` bind/result methods. 
4. ...

## sql::Value<T> Interface
```cpp
namespace sql
{
	using null = std::monostate;

	// primary Value struct
	template<typename ValueType, typename = void>
	struct Value;

	// remove reference from type
	template<typename ValueType>
	struct Value<ValueType&, void> : Value<ValueType, void> { };
	
	// deduction guide: handle Value()
	template<>
	Value() -> Value<null>;

	// deduction guide: handle Value(ValueType&&)
	template<typename ValueType>
	Value(ValueType&&) -> Value<ValueType>;

	// null type
	template<>
	struct Value<null,  void> { /* tbd */ };
	
	// integer type
	template<typename T>
	struct Value<T, std::enable_if_t<std::is_integral_v<T>>> { /* tbd */ };
	
	// real type
	template<typename T>
	struct Value<T, std::enable_if_t<std::is_floating_point_v<T>>> { /* tbd */ };
	
	// text type for const char*
	template<>
	struct Value<const char*, void> { /* tbd */ };

	// text type for std::string
	template<typename T>
	struct Value<T, std::enable_if_t<std::is_constructible_v<std::string, T>>> { /* tbd */ };
	
	// blob type for buffer (const void*, int)
	template<>
	struct Value<const void*, void> { /* tbd */ };
}
```
