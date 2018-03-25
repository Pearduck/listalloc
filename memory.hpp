#pragma once
#include <memory>
#include "alloc.hpp"
namespace sus
{
    template<typename T>
    using shared_ptr = std::shared_ptr<T>;
    template<typename T>
    using weak_ptr = std::weak_ptr<T>;
    template<typename T, typename... Args>
    sus::shared_ptr<T> make_shared(Args&&... args)
    {
        return std::allocate_shared<T>(listAllocator<T>(), std::forward<Args>(args)...);
    }
};
