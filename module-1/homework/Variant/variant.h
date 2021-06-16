#include <cstdlib>
#include <utility>

#pragma once

namespace task {

template<typename From, typename To>
auto check_implicitly_convertible(int) -> decltype(void(std::declval<void(&)(To)>()(std::declval<From>())), std::true_type {});

template<typename, typename>
auto check_implicitly_convertible(...) -> std::false_type;

template<typename From, typename To>
inline constexpr bool is_implicitly_convertible_v = decltype(check_implicitly_convertible<From, To>(0))::value;

template<std::size_t Index, typename... Types>
union Union;

template<std::size_t Index>
union  Union<Index> {};

struct Helper {
    template<typename U>
    static auto&& get(U&& u, std::in_place_index_t<0>) {
        return std::forward<U>(u).head;
    }

    template<typename U, std::size_t Idx>
    static auto&& get(U&& u, std::in_place_index_t<Idx>) {
        return get(std::forward<U>(u).tail, std::in_place_index<Idx - 1>);
    }

    template<typename U, std::size_t UnionIdx, typename Head, typename... Tail>
    static void set(U&& value, std::in_place_index_t<0>, Union<UnionIdx, Head, Tail...>& u) {
        if (is_implicitly_convertible_v<U, Head>)
            u.head = value;
        else if (std::is_same_v<Head, U>)
            u.head = value;
    }

    template<std::size_t Idx, typename U, std::size_t UnionIdx, typename Head, typename... Tail>
    static void set(U&& value, std::in_place_index_t<Idx>, Union<UnionIdx, Head, Tail...>& u) {
        set(std::forward<U>(value), std::in_place_index<Idx - 1>, u.tail);
    }
};

template<std::size_t Index, typename T, typename... Types>
union Union<Index, T, Types...> {
public:
    using value_type = T;
    friend struct Helper;

    value_type head;
    Union<Index + 1, Types...> tail;

    Union() {
        if (!std::is_trivially_constructible_v<value_type>)
            head = T();
    }

    ~Union() {
        if (!std::is_trivially_destructible_v<value_type>)
            head.~T();
    }
};

template <typename... Types>
class Variant;

template <size_t Idx, typename T>
struct VariantAlternative;

template <size_t Idx, typename T>
using variant_alternative_t = typename VariantAlternative<Idx, T>::type;

template<typename Head, typename... Tail>
struct TypeList{
    using head = Head;
    using tail =  TypeList<Tail...>;
};

template<std::size_t Idx, class TList> struct TypeAt;

template<typename Head, typename... Tail>
struct TypeAt<0, TypeList<Head, Tail...>> {
    using type = Head;
};

template<std::size_t Idx, typename Head, typename... Tail>
struct TypeAt<Idx, TypeList<Head, Tail...>> {
    using type = typename TypeAt<Idx - 1, TypeList<Tail...>>::type;
};

template<std::size_t Size>
constexpr std::size_t findTypePos(std::size_t pos, const bool (&founded)[Size], const bool (&conv)[Size]) {
    return (pos == Size) ? -1 :
           ((founded[pos] || conv[pos]) ? pos : findTypePos(pos + 1, founded, conv));
}

template<std::size_t Size>
constexpr std::size_t findTypePos(std::size_t pos, const bool (&founded)[Size]) {
    return (pos == Size) ? -1 :
           ((founded[pos]) ? pos : findTypePos(pos + 1, founded));
}

template<typename Target, typename... Types>
struct Finder {
    constexpr static bool founded[sizeof...(Types)] = {std::is_same_v<Target, Types>...};
    constexpr static bool conv[sizeof...(Types)] = {is_implicitly_convertible_v<Target, Types>...};
    constexpr static std::size_t value = findTypePos(0, founded);
    constexpr static std::size_t assignment = value != -1 ? value : findTypePos(0, founded, conv);
};

template <size_t Idx, typename... Types>
struct VariantAlternative<Idx, Variant<Types...>> {
    using type = typename TypeAt<Idx, TypeList<Types...>>::type;
};

template <typename... Types>
class Variant {
public:
    // Special member functions
    constexpr Variant() noexcept {}

    template<typename Target, typename... FTypes>
    static auto&& Get(Variant<FTypes...>& v) {
        return Helper::get(std::forward<Variant<FTypes...>>(v).union_,
                           std::in_place_index<Finder<Target, FTypes...>::value>);
    }

    template <typename T>
    Variant& operator=(T&& t) noexcept {
        constexpr std::size_t value = Finder<T, Types...>::assignment;
        static_assert(value != -1);
        Helper::set(std::forward<T>(t), std::in_place_index<value>, union_);
        return *this;
    }

private:
    Union<0, Types...> union_;
};

// Non-member functions
template <size_t I, typename... Types>
constexpr const variant_alternative_t<I, Variant<Types...>>& Get(Variant<Types...>& v) {
    return Variant<Types...>::template Get<typename TypeAt<I, TypeList<Types...>>::type>(std::forward<decltype(v)>(v));
}

template <size_t I, typename... Types>
constexpr variant_alternative_t<I, Variant<Types...>>&& Get(Variant<Types...>&& v) {
    return Variant<Types...>::template Get<typename TypeAt<I, TypeList<Types...>>::type>(std::move(v));
}

template <typename T, typename... Types>
constexpr const T& Get(Variant<Types...>& v) {
    return Variant<Types...>::template Get<T>(v);
}

template <typename T, typename... Types>
constexpr T&& Get(Variant<Types...>&& v) {
    return Variant<Types...>::template Get<T>(std::move(v));
}

};  // namespace task