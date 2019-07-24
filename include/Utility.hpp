#pragma once

namespace Utility
{
	/**
	 * Generates a Callable instance that invokes the given function.
	 * Implementation similar to P0477R0 (std::monostate_function<>).
	 */
	template<auto Callable>
	struct MonostateFunction
	{
		template<typename... Ts>
		constexpr decltype(auto) operator()(Ts&&... args) const noexcept(
			std::is_nothrow_invocable_v<decltype(Callable), Ts...>)
		{
			return std::invoke(Callable, std::forward<Ts>(args)...);
		}
	};
}
