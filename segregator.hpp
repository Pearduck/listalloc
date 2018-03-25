#pragma once
#include <typeinfo>
namespace sus
{
    template<typename T, size_t threshold, typename SmallAlloc, typename BigAlloc>
    struct Segregator : private SmallAlloc, private BigAlloc
    {
        using value_type = T;
        Segregator() noexcept
        {}
        template<typename U>
        struct rebind
        {   
            using otherSmall = typename SmallAlloc::template rebind<U>::other;
            using otherBig = typename BigAlloc::template rebind<U>::other;
            using other = Segregator<U, threshold, otherSmall, otherBig>;
        };
        template<typename U, typename sAlloc, typename bAlloc>
        Segregator(Segregator<U, threshold, sAlloc, bAlloc> const &) noexcept
        {}
        T *allocate(size_t n)
        {   
            if(n * sizeof(T) <= threshold)
                return SmallAlloc::allocate(n);
            else
                return BigAlloc::allocate(n);
        }
        void deallocate(T *ptr, size_t n) noexcept
        {   
            if(n * sizeof(T) <= threshold)
                SmallAlloc::deallocate(ptr, n);
            else
                BigAlloc::deallocate(ptr, n);
        }
    };
    template<typename T, typename U, size_t s1, size_t s2, typename Alloc1, typename Alloc2, typename Alloc3, typename Alloc4>
    constexpr bool operator==(Segregator<T, s1, Alloc1, Alloc2> const &, Segregator<U, s2, Alloc3, Alloc4> const &)
    {
        return s1 == s2 && typeid(Alloc1) == typeid(Alloc3) && typeid(Alloc2) == typeid(Alloc4);
    }
    template<typename T, typename U, size_t s1, size_t s2, typename Alloc1, typename Alloc2, typename Alloc3, typename Alloc4>
    constexpr bool operator!=(Segregator<T, s1, Alloc1, Alloc2> const &, Segregator<U, s2, Alloc3, Alloc4> const &)
    {
        return s1 != s2 || typeid(Alloc1) != typeid(Alloc3) || typeid(Alloc2) != typeid(Alloc4);
    }
};
