# CntRPG

## To-do
1. Finish Bridge for BLOB
	- Due to type collisions between blob and text types (both should accept a const char* for example), a custom `SQL::Blob` type with an `explicit` constructor will be introduced. Overloading shall be done with through the `Bridge` interface of the outer type `Blob`.
	- Natively support: `Bridge<Blob<std::string_view>>` as raw buffer input.
	- Natively support: `Bridge<Blob<T>>` where `T` is a `POD type`.
	- Natively support: `Bridge<Blob<T>>` with static member functions:
		- `std::string T::Blobify(const T&)`
		- `T T::Objectify(std::string blob)`
