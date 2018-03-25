#pragma once
#include <stdexcept>
//#include <iostream>
namespace sus
{
    template<size_t size>
    struct Chunk
    {
        char memory[size];
        Chunk *next = nullptr;
    };
    template<size_t chunksize>
    class ListAlloc
    {
        Chunk<chunksize> *_head;
        //size_t _allocd = 0;
        //size_t _mallocd = 0;
    public:
        ListAlloc() noexcept:
            _head(nullptr)
        {}
        ListAlloc(ListAlloc const &) = delete;
        ListAlloc(ListAlloc &&) = delete;
        ListAlloc &operator=(ListAlloc const &) = delete;
        ListAlloc &operator=(ListAlloc &&) = delete;
        static ListAlloc &instance()
        {
            static ListAlloc alloc;
            return alloc;
        }
        ~ListAlloc()
        {
            if(_head)
            {
                while(_head->next)
                {
                    Chunk<chunksize> *next = _head->next;
                    delete _head;
                    _head = next;
                }
                delete _head;
            }
            //std::cout << "alloc: " << _allocd << ", " << _mallocd << " mallocs among them" << std::endl;
        }
        void *allocate(size_t n)
        {
            if(n > chunksize)
                throw std::bad_alloc();
            //_allocd++;
            if(_head == nullptr)
            {
                //_mallocd++;
                return static_cast<void *>(new Chunk<chunksize>());
            }
            else
            {
                Chunk<chunksize> *ret = _head;
                _head = _head->next;
                return static_cast<void *>(ret);
            }
        }
        void deallocate(void *ptr, size_t n) noexcept
        {
            if(n > chunksize)
                return;
            Chunk<chunksize> *next = _head;
            _head = static_cast<Chunk<chunksize> *>(ptr);
            _head->next = next;
        }
    };
    template<typename T, size_t chunksize>
    struct ListAllocator
    {
        using value_type = T;
        ListAllocator() noexcept
        {}
        template<typename U>
        struct rebind
        {
            using other = ListAllocator<U, chunksize>;
        };
        template<typename U>
        ListAllocator(ListAllocator<U, chunksize> const &a) noexcept
        {}
        T *allocate(size_t n)
        {
            return static_cast<T *>(ListAlloc<chunksize>::instance().allocate(n * sizeof(T)));
        }
        void deallocate(T *ptr, size_t n) noexcept
        {
            ListAlloc<chunksize>::instance().deallocate(ptr, n * sizeof(T));
        }
    };
    template<typename T, typename U, size_t s1, size_t s2>
    constexpr bool operator==(ListAllocator<T, s1> const &, ListAllocator<U, s2> const &)
    {
        return s1 == s2;
    }
    template<typename T, typename U, size_t s1, size_t s2>
    constexpr bool operator!=(ListAllocator<T, s1> const &, ListAllocator<U, s2> const &)
    {
        return s1 != s2;
    }
};
