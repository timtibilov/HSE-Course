#pragma once

#include <type_traits>
#include <utility>

template <typename T>
struct Uncvref {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template <typename T>
using uncvref_t = typename Uncvref<T>::type;

template <typename T>
struct AddConst {
    using type = const T;
};

template <typename T>
using add_const_t = typename AddConst<T>::type;

template <typename T>
struct AddLvalueReference {
    using type = std::remove_reference_t<T>&;
};

template <typename T>
struct AddRvalueReference {
    using type = std::remove_reference_t<T>&&;
};

template <typename T>
using add_lvalue_reference_t = typename AddLvalueReference<T>::type;

template <typename T>
using add_rvalue_reference_t = typename AddRvalueReference<T>::type;