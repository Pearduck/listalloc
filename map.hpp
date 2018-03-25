#pragma once
#include <map>
#include "alloc.hpp"
namespace sus
{
    template<typename T, typename U, typename less = std::less<T>>
    using map = std::map<T, U, less, listAllocator<std::pair<T, U>>>;
}
