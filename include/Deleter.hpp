#pragma once

#include "PrecompiledHeader.hpp"

/**
 * Generates a stateless Deleter type to be used for smart pointers which are to be destruced by a free function.
 */
template<typename T, auto Deleter>
struct DeleterFactory
{
	void operator()(T* ptr) noexcept(std::is_nothrow_invocable_v<decltype(Deleter), T*>)
	{
		Deleter(ptr);
	}
};
