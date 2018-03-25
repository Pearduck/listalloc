#pragma once
#include <set>
#include "alloc.hpp"
namespace sus
{
    template<typename T, typename less = std::less<T>>
    using set = std::set<T, less, listAllocator<T>>;
}
