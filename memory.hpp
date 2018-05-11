#pragma once
#include <memory>
#include "alloc.hpp"
namespace sus
{
    template<typename T>
    struct _deleter
    {
        using traits = std::allocator_traits<listAllocator<T>>;
        void operator()(T *ptr) const
        {
            traits::destroy(ptr);
            traits::deallocate(ptr, 1);
        }
    };
    template<typename T>
    using unique_ptr = std::unique_ptr<T, _deleter<T>>;
    template<typename T>
    using shared_ptr = std::shared_ptr<T>;
    template<typename T>
    using weak_ptr = std::weak_ptr<T>;
    template<typename T, typename... Args>
    sus::shared_ptr<T> make_shared(Args &&... args)
    {
        return std::allocate_shared<T>(listAllocator<T>(), std::forward<Args>(args)...);
    }
    template<typename T, typename... Args>
    sus::unique_ptr<T> make_unique(Args &&... args)
    {
        using traits = std::allocator_traits<listAllocator<T>>;
        T *ptr = traits::allocate(1);
        traits::construct(ptr, std::forward<Args>(args)...);
        return sus::unique_ptr<T>(ptr);
    }
};
