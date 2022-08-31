#pragma once

#include <utility>

namespace sugar {

    struct [[maybe_unused]] Let {
        template<typename T, typename F>
        requires ::std::invocable<F, T>
        constexpr auto operator()(T &&v, F f) const noexcept {
            return f(::std::forward<T>(v));
        }

        using is_transparent [[maybe_unused]] = int;
    };

    [[maybe_unused]] Let let;

    struct [[maybe_unused]] Also {
        template<typename T, typename F>
        requires ::std::invocable<F, T>
        constexpr auto &&operator()(T &&v, F f) const noexcept {
            f(::std::forward<T>(v));
            return ::std::forward<T>(v);
        }

        using is_transparent [[maybe_unused]] = int;
    };

    [[maybe_unused]] Also also;
}
