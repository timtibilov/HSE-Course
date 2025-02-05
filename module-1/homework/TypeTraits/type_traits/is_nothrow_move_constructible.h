#pragma once

#include <type_traits>

#include "is_copy_constructible.h"
#include "utility.h"

//<IsConstructible, is_reference, T, Args...>
template <bool, bool, typename T, typename... Args>
struct LibCppIsNoThrowConstructible;

// LibCppIsNoThrowConstructible - partial specializations

template <typename T, typename... Args>
struct LibCppIsNoThrowConstructible<true, false, T, Args...> {
    using type = std::integral_constant<bool, noexcept(T(declval<Args>()...))>;
};

template <typename T, typename... Args>
struct LibCppIsNoThrowConstructible<true, true, T, Args...> {
    using type = std::integral_constant<bool, noexcept(IsConstructibleHelper::implicitCast<T>(declval<Args>()...))>;
};

template <bool Ref, typename T, typename A>
struct LibCppIsNoThrowConstructible<false, Ref, T, A> {
    using type = std::false_type;
};

// LibCppIsNoThrowConstructible - partial specializations

template <typename T, typename... Args>
struct IsNoThrowConstructible : LibCppIsNoThrowConstructible<IsConstructible<T, Args...>::value, std::is_reference_v<T>, T, Args...>::type
{};

template <typename T, std::size_t N>
struct IsNoThrowConstructible<T[N]> : LibCppIsNoThrowConstructible<IsConstructible<T>::value, std::is_reference_v<T>, T>::type
{};

template <typename T>
struct IsNoThrowMoveConstructible : IsNoThrowConstructible<T, T&&> {};