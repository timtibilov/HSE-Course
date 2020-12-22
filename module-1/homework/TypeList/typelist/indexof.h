#pragma once

#include "typelist.h"

typedef long long idx;

template<typename TList, typename TargetType>
struct IndexOf;

template<typename T>
struct IndexOf<NullType, T> {
    static constexpr idx pos = -1;
};

template<typename Head, typename Tail>
struct IndexOf<TypeList<Head, Tail>, Head> {
    static constexpr idx pos = 0;
};

template<typename Head, typename Tail, typename T>
struct IndexOf<TypeList<Head, Tail>, T> {
    static constexpr idx pos = IndexOf<Tail, T>::pos == -1 ? -1 : 1 + IndexOf<Tail, T>::pos;
};