#pragma once

#include "typelist.h"

template<typename TList, typename TargetType>
struct EraseAll;

template<typename T>
struct EraseAll<NullType, T> {
    typedef NullType NewTypeList;
};

template<typename Head, typename Tail>
struct EraseAll<TypeList<Head, Tail>, Head> {
    typedef typename EraseAll<Tail, Head>::NewTypeList NewTypeList;
};

template<typename Head, typename Tail, typename T>
struct EraseAll<TypeList<Head, Tail>, T> {
    typedef TypeList<Head, typename EraseAll<Tail, T>::NewTypeList> NewTypeList;
};