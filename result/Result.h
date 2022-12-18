#ifndef SUGAR_RESULT_H
#define SUGAR_RESULT_H
#pragma once

#include <cstdint>
#include <utility>
#include <variant>
#include <stdexcept>
#include <optional>

namespace sugar {

    template<class T, class E>
    class Result {
    public:
        constexpr Result() {
            state = State::Uninit;
        }

        ~Result() {
            if (state == State::Ok) {
                val.~T();
            } else if (state == State::Err) {
                err.~E();
            }
        }

        constexpr static Result Ok(T val) {
            return Result(val);
        }

        constexpr static Result Err(E err) {
            return Result((std::byte) 0, err);
        }

    private:
        enum class State : uint8_t {
            Ok = 0, Err = 255, Uninit = 205
        };
        State state;
        union {
            T val;
            E err;
        };

        constexpr explicit Result(T val) : val(val) {
            state = State::Ok;
        }

        constexpr explicit Result(std::byte o, E err) : err(err) {
            state = State::Err;
        }

    private:
        constexpr void Copy(Result &other) {
            bool uninit = state == State::Uninit;
            state = other.state;
            if (state == State::Ok) {
                if (uninit) new(&val) T{other.val};
                else val = other.val;
            } else if (state == State::Err) {
                if (uninit) new(&err) E{other.err};
                else err = other.err;
            }
        }

        constexpr void Move(Result &&other) {
            bool uninit = state == State::Uninit;
            state = other.state;
            if (state == State::Ok) {
                if (uninit) new(&val) T;
                val = std::move(other.val);
            } else {
                if (uninit) new(&err) E;
                err = std::move(other.err);
            }
        }

    public:

        constexpr Result(Result &other) noexcept {
            Copy(other);
        }

        constexpr Result(Result &&other) noexcept {
            Move(std::forward<Result &&>(other));
        }

        constexpr Result &operator=(const Result &other) noexcept {
            Copy(other);
            return *this;
        }

        constexpr Result &operator=(Result &&other) noexcept {
            Move(std::forward<Result &&>(other));
            return *this;
        }

    public:

        [[nodiscard]]
        constexpr bool IsOk() const {
            return state == State::Ok;
        }

        [[nodiscard]]
        constexpr bool IsErr() const {
            return state == State::Err;
        }

        constexpr const T &UnwrapOk() const {
            if (IsOk()) {
                return val;
            } else {
                throw std::runtime_error( // NOLINT(hicpp-exception-baseclass)
                        "try to extract the result of err with ok");
            }
        }

        constexpr T &UnwrapOk() {
            if (IsOk()) {
                return val;
            } else {
                throw std::runtime_error( // NOLINT(hicpp-exception-baseclass)
                        "try to extract the result of err with ok");
            }
        }

        constexpr const E &UnwrapErr() const {
            if (IsErr()) {
                return err;
            } else {
                throw std::runtime_error( // NOLINT(hicpp-exception-baseclass)
                        "try to extract the result of ok with err");
            }
        }

        constexpr E &UnwrapErr() {
            if (IsErr()) {
                return err;
            } else {
                throw std::runtime_error( // NOLINT(hicpp-exception-baseclass)
                        "try to extract the result of ok with err");
            }
        }

        constexpr const T &UnsafeOk() const {
            return val;
        }

        constexpr T &UnsafeOk() {
            return val;
        }

        constexpr const E &UnsafeErr() const {
            return err;
        }

        constexpr E &UnsafeErr() {
            return err;
        }

        constexpr std::optional<const T &> TryOk() const {
            if (IsOk()) {
                return {val};
            } else {
                return std::nullopt;
            }
        }

        constexpr std::optional<T &> TryOk() {
            if (IsOk()) {
                return {val};
            } else {
                return std::nullopt;
            }
        }

        constexpr std::optional<const E &> TryErr() const {
            if (IsErr()) {
                return {err};
            } else {
                return std::nullopt;
            }
        }

        constexpr std::optional<E &> TryErr() {
            if (IsErr()) {
                return {err};
            } else {
                return std::nullopt;
            }
        }

    public:

        template<class F>
        constexpr auto map(F f) const -> Result<decltype(f(val)), E> {
            if (IsOk()) {
                auto r = f(val);
                return Result<decltype(r), E>::Ok(r);
            } else {
                return *this;
            }
        };

        template<class F>
        constexpr auto mapErr(F f) const -> Result<T, decltype(f())> {
            if (IsErr()) {
                auto r = f(err);
                return Result<T, decltype(f())>::Err(r);
            } else {
                return *this;
            }
        };
    };

    template<class E>
    class Result<void, E> {
    public:
        constexpr explicit Result() {
            state = State::Ok;
        }

        ~Result() {
            if (state == State::Err) {
                err.~E();
            }
        }

        constexpr static Result Ok() {
            return Result();
        }

        constexpr static Result Err(E err) {
            return Result(err);
        }

    private:
        enum class State : uint8_t {
            Ok = 0, Err = 255
        };
        State state;
        E err;

        constexpr explicit Result(E err) : err(err) {
            state = State::Err;
        }

    private:
        constexpr void Copy(Result &other) {
            bool is_ok = state == State::Ok;
            state = other.state;
            if (state == State::Err) {
                if (is_ok) new(&err) E{other.err};
                else err = other.err;
            }
        }

        constexpr void Move(Result &&other) {
            bool is_ok = state == State::Ok;
            state = other.state;
            if (state == State::Err) {
                if (is_ok) new(&err) E;
                err = std::move(other.err);
            }
        }

    public:

        constexpr Result(Result &other) noexcept {
            Copy(other);
        }

        constexpr Result(Result &&other) noexcept {
            Move(std::forward<Result &&>(other));
        }

        constexpr Result &operator=(const Result &other) noexcept {
            Copy(other);
            return *this;
        }

        constexpr Result &operator=(Result &&other) noexcept {
            Move(std::forward<Result &&>(other));
            return *this;
        }

    public:

        [[nodiscard]]
        constexpr bool IsOk() const {
            return state == State::Ok;
        }

        [[nodiscard]]
        constexpr bool IsErr() const {
            return state == State::Err;
        }

        constexpr void UnwrapOk() const {
            if (IsOk()) {
                return;
            } else {
                throw std::runtime_error( // NOLINT(hicpp-exception-baseclass)
                        "try to extract the result of err with ok");
            }
        }

        constexpr E &UnwrapErr() const {
            if (IsErr()) {
                return err;
            } else {
                throw std::runtime_error( // NOLINT(hicpp-exception-baseclass)
                        "try to extract the result of ok with err");
            }
        }

        constexpr void UnsafeOk() const {}

        constexpr const E &UnsafeErr() const {
            return err;
        }

        constexpr E &UnsafeErr() {
            return err;
        }

        [[nodiscard]]
        constexpr bool TryOk() const {
            if (IsOk()) {
                return true;
            } else {
                return false;
            }
        }

        constexpr std::optional<const E &> TryErr() const {
            if (IsErr()) {
                return {err};
            } else {
                return std::nullopt;
            }
        }

        constexpr std::optional<E &> TryErr() {
            if (IsErr()) {
                return {err};
            } else {
                return std::nullopt;
            }
        }

    public:

        template<class F>
        constexpr auto map(F f) const -> Result<decltype(f()), E> {
            if (IsOk()) {
                auto r = f();
                return Result<decltype(r), E>::Ok(r);
            } else {
                return *this;
            }
        };

        template<class F>
        constexpr auto mapErr(F f) const -> Result<void, decltype(f())> {
            if (IsErr()) {
                auto r = f(err);
                return Result<void, decltype(f())>::Err(r);
            } else {
                return *this;
            }
        };
    };

} // sugar

#endif //SUGAR_RESULT_H
