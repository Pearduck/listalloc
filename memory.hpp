#pragma once
#include <memory>
#include "alloc.hpp"
namespace sus
{
    template<typename T>
    struct _deleter
    {
        void operator()(T *ptr) const
        {
            ptr->~T();
            listAllocator<T>().deallocate(ptr, 1);
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
        T *ptr = listAllocator<T>().allocate(1);
        ::new (static_cast<void *>(ptr)) T(std::forward<Args>(args)...);
        return sus::unique_ptr<T>(ptr);
    }
};
