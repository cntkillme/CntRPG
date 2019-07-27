#pragma once

#include <functional>
#include <type_traits>
#include <utility>

/**
 * @brief Generates a Callable class that invokes the given function.
 */
template<auto Callable>
struct monostate_function
{
	/**
	 * @brief Invokes the function.
	 * @tparam Args... the argument types.
	 * @param args... the arguments.
	 * @return What the function returns.
	 * @throws What the function throws.
	 */
	template<typename... Args>
	constexpr decltype(auto) operator()(Args&&... args) const noexcept(
		std::is_nothrow_invocable_v<decltype(Callable), Args...>)
	{
		return std::invoke(Callable, std::forward<Args>(args)...);
	}
};
