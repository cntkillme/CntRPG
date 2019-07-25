# CntRPG

## To-do
1. Finish SQL Bridge for BLOB
	- Due to type collisions between blob and text types (both should accept a const char* for example), a custom `SQL::Blob` type with an `explicit` constructor will be introduced. Overloading shall be done with through the `Bridge` interface of the outer type `Blob`.
	- Natively support: `Bridge<Blob<const T*>>` where `T` is convertible to `std::byte` (size expected as second constructor argument).
	- Natively support: `Bridge<Blob<std::string>>`.
	- Natively support: `Bridge<Blob<T>>` with static member functions:
		- `std::string T::ToBlob(const T&)` to encode (optional).
		- `T T::FromBlob(std::string blob)` to decode (optional).
2. SQLite Transactions
3. ...
