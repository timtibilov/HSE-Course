#pragma once

#include <type_traits>
#include <utility>

#include "utility.h"

template <typename T>
add_rvalue_reference_t<T> declval() noexcept;

template <typename T, typename... Args>
struct LibCppIsConstructible;

template <typename Derived, typename Base>
struct IsInvalidBaseToDerivedCast {
    using base = uncvref_t<Base>;
    using derived = uncvref_t<Derived>;

    static std::integral_constant<bool,
            !std::is_same_v<base, derived> &&
            std::is_base_of_v<base, derived> &&
            !LibCppIsConstructible<derived, base>::type::value> value;
};

template <typename To, typename From>
struct IsInvalidLvalueToRvalueCast : std::false_type {
};

template <typename RefTo, typename RefFrom>
struct IsInvalidLvalueToRvalueCast<RefTo&&, RefFrom&> {
    using from = uncvref_t<RefFrom>;
    using to = uncvref_t<RefTo>;

    static std::integral_constant<bool,
            std::is_same_v<from, to> &&
            std::is_base_of_v<to, from>> value;
};

struct IsConstructibleHelper {
    template <typename To, typename From>
    static To implicitCast(From);

    template <typename To, typename From, typename = decltype(implicitCast<To>(declval<From>()))>
    static std::true_type cast(long long);
    template <typename To, typename From, typename = decltype(static_cast<To>(declval<From>()))>
    static std::integral_constant<bool,
            !IsInvalidBaseToDerivedCast<To, From>::value &&
            !IsInvalidLvalueToRvalueCast<To, From>::value> cast(int);
    template <typename, typename>
    static std::false_type cast(...);

    template <typename T, typename ...Args, typename = decltype(T(declval<Args>()...))>
    static std::true_type nary(int);
    template <typename ..., typename...>
    static std::false_type nary(...);

    template <typename T, typename A, typename = decltype(::new T(declval<A>()))>
    static std::is_destructible<T> unary(int);
    template <typename, typename>
    static std::false_type unary(...);
};

template <typename T, typename... Args>
struct LibCppIsConstructible {
    using type = decltype(IsConstructibleHelper::nary<T, Args...>(0));
};

template <typename T, typename A>
struct LibCppIsConstructible<T, A> {
    using type = decltype(IsConstructibleHelper::unary<T, A>(0));
};

template <typename T, typename A>
struct LibCppIsConstructible<T&, A> {
    using type = decltype(IsConstructibleHelper::cast<T&, A>(0));
};

template <typename T, typename A>
struct LibCppIsConstructible<T&&, A> {
    using type = decltype(IsConstructibleHelper::cast<T&&, A>(0));
};

// LibCppIsConstructible - partial specializations

template <typename T, typename... Args>
struct IsConstructible : LibCppIsConstructible<T, Args...>::type {
};

template <typename T>
struct IsCopyConstructible : LibCppIsConstructible<T, const T&>::type {
};