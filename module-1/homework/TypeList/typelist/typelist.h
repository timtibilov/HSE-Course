#pragma once

// Мне очень понравилась реализация TypeList, описанная здесь:
// https://habr.com/ru/post/220217/
template<typename Head, typename Tail>
struct TypeList;

struct NullType {
};