#ifndef COMMON_ALIAS
#define COMMON_ALIAS

#include <string>
#include <vector>

template <class T>
using vec = std::vector<T>;

template <class T>
using mat = vec<vec<T>>;

using uint = unsigned int;
using str = std::string;

#endif