#pragma once
#include "listalloc.hpp"
#include "segregator.hpp"
namespace sus
{
    template<typename T>
    using littleListAlloc = ListAllocator<T, 64>;
    template<typename T>
    using biggerListAlloc = ListAllocator<T, 256>;
    template<typename T>
    using listAllocator = Segregator<T, 64,
                              littleListAlloc<T>,
                              Segregator<T, 256,
                                  biggerListAlloc<T>,
                                  std::allocator<T>
                              >
                          >;
};
