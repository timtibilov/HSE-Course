#pragma once

#include "typelist.h"

template<typename TList, typename OldType, typename NewType> 
struct Replace;

template<typename OldT, typename NewT>
struct Replace<NullType, OldT, NewT> {
    typedef NullType NewTypeList;
};

template<typename OldT, typename NewT, typename Tail>
struct Replace<TypeList<OldT, Tail>, OldT, NewT> {
    typedef TypeList<NewT, Tail> NewTypeList;
};

template<typename Head, typename Tail, typename OldT, typename NewT>
struct Replace<TypeList<Head, Tail>, OldT, NewT> {
    typedef TypeList<Head, typename Replace<Tail, OldT, NewT>::NewTypeList> NewTypeList;
};