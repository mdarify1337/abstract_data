#pragma once

#include <memory>
#include <cstddef>
#include <stdexcept>
#include <iterator>
#include <algorithm> 

namespace ft
{
    template <typename T, class Alloc = std::allocator<T>>
    class deque
    {
    public:
        typedef T value_type;
        typedef Alloc allocator_type;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

    private:
        static const size_type BLOCK_SIZE = 64;

        pointer *map;                       
        size_type map_size;                 
        size_type start_block, start_index; 
        size_type end_block, end_index;   
        size_type sz;
        allocator_type alloc;

        pointer allocate_block()
        {
            return alloc.allocate(BLOCK_SIZE);
        }

        void deallocate_block(pointer p)
        {
            alloc.deallocate(p, BLOCK_SIZE);
        }

        void clear_storage()
        {
            if (map)
            {
                if (sz > 0)
                {
                    iterator it = begin();
                    for (; it != end(); ++it)
                        alloc.destroy(&(*it));
                }
                for (size_type b = 0; b < map_size; ++b)
                {
                    if (map[b])
                        deallocate_block(map[b]);
                }
                delete[] map;
            }
            map = NULL;
            map_size = 0;
            sz = 0;
        }

        void grow_map()
        {
            size_type new_size = (map_size ? map_size * 2 : 8);
            pointer *new_map = new pointer[new_size];
            for (size_type i = 0; i < new_size; ++i)
                new_map[i] = NULL;

            size_type offset = (new_size - map_size) / 2;
            for (size_type i = 0; i < map_size; ++i)
                new_map[offset + i] = map[i];

            delete[] map;
            map = new_map;
            start_block += offset;
            end_block += offset;
            map_size = new_size;
        }

    public:
        class iterator
        {
        public:
            typedef std::random_access_iterator_tag iterator_category;
            typedef T value_type;
            typedef std::ptrdiff_t difference_type;
            typedef T *pointer;
            typedef T &reference;

        private:
            pointer *block_ptr; 
            size_type index;    

        public:
            iterator() : block_ptr(NULL), index(0) {}
            iterator(pointer *bp, size_type i) : block_ptr(bp), index(i) {}
            iterator(const iterator &other) : block_ptr(other.block_ptr), index(other.index) {}
            iterator &operator=(const iterator &other)
            {
                block_ptr = other.block_ptr;
                index = other.index;
                return *this;
            }

            reference operator*() const { return (*block_ptr)[index]; }
            pointer operator->() const { return &(*block_ptr)[index]; }

            iterator &operator++()
            {
                ++index;
                if (index == BLOCK_SIZE)
                {
                    ++block_ptr;
                    index = 0;
                }
                return *this;
            }
            iterator operator++(int)
            {
                iterator tmp(*this);
                ++(*this);
                return tmp;
            }

            iterator &operator--()
            {
                if (index == 0)
                {
                    --block_ptr;
                    index = BLOCK_SIZE - 1;
                }
                else
                    --index;
                return *this;
            }
            iterator operator--(int)
            {
                iterator tmp(*this);
                --(*this);
                return tmp;
            }

            bool operator==(const iterator &other) const
            {
                return block_ptr == other.block_ptr && index == other.index;
            }
            bool operator!=(const iterator &other) const
            {
                return !(*this == other);
            }
        };

        class const_iterator
        {
        public:
            typedef std::random_access_iterator_tag iterator_category;
            typedef const T value_type;
            typedef std::ptrdiff_t difference_type;
            typedef const T *pointer;
            typedef const T &reference;

        private:
            pointer *block_ptr; 
            size_type index;

        public:
            const_iterator() : block_ptr(NULL), index(0) {}
            const_iterator(T **bp, size_type i)
                : block_ptr(const_cast<pointer *>(bp)), index(i) {}
            const_iterator(const const_iterator &other)
                : block_ptr(other.block_ptr), index(other.index) {}
            const_iterator(const iterator &it)
                : block_ptr(it.block_ptr), index(it.index) {}
            const_iterator &operator=(const const_iterator &other)
            {
                block_ptr = other.block_ptr;
                index = other.index;
                return *this;
            }

            reference operator*() const { return (*block_ptr)[index]; }
            pointer operator->() const { return &(*block_ptr)[index]; }

            const_iterator &operator++()
            {
                ++index;
                if (index == BLOCK_SIZE)
                {
                    ++block_ptr;
                    index = 0;
                }
                return *this;
            }
            const_iterator operator++(int)
            {
                const_iterator tmp(*this);
                ++(*this);
                return tmp;
            }

            const_iterator &operator--()
            {
                if (index == 0)
                {
                    --block_ptr;
                    index = BLOCK_SIZE - 1;
                }
                else
                    --index;
                return *this;
            }
            const_iterator operator--(int)
            {
                const_iterator tmp(*this);
                --(*this);
                return tmp;
            }

            bool operator==(const const_iterator &other) const
            {
                return block_ptr == other.block_ptr && index == other.index;
            }
            bool operator!=(const const_iterator &other) const
            {
                return !(*this == other);
            }
        };

        deque()
            : map(NULL), map_size(0), start_block(0), start_index(0),
              end_block(0), end_index(0), sz(0), alloc(Alloc())
        {
            map_size = 8;
            map = new pointer[map_size];
            for (size_type i = 0; i < map_size; ++i)
                map[i] = NULL;

            size_type mid = map_size / 2;
            map[mid] = allocate_block();
            start_block = end_block = mid;
            start_index = end_index = 0;
        }

        template <class InputIt>
        deque(InputIt first, InputIt last)
            : map(NULL), map_size(0), start_block(0), start_index(0),
              end_block(0), end_index(0), sz(0), alloc(Alloc())
        {
            map_size = 8;
            map = new pointer[map_size];
            for (size_type i = 0; i < map_size; ++i)
                map[i] = NULL;

            size_type mid = map_size / 2;
            map[mid] = allocate_block();
            start_block = end_block = mid;
            start_index = end_index = 0;

            for (; first != last; ++first)
                push_back(*first);
        }

        deque(const deque &other)
            : map(NULL), map_size(0), start_block(0), start_index(0),
              end_block(0), end_index(0), sz(0), alloc(other.alloc)
        {
            map_size = other.map_size;
            map = new pointer[map_size];
            for (size_type b = 0; b < map_size; ++b)
            {
                if (other.map[b])
                {
                    map[b] = allocate_block();
                    for (size_type i = 0; i < BLOCK_SIZE; ++i)
                        alloc.construct(map[b] + i, other.map[b][i]);
                }
                else
                    map[b] = NULL;
            }

            start_block = other.start_block;
            start_index = other.start_index;
            end_block = other.end_block;
            end_index = other.end_index;
            sz = other.sz;
        }

        deque &operator=(const deque &other)
        {
            if (this == &other)
                return *this;
            clear_storage();

            alloc = other.alloc;
            map_size = other.map_size;
            map = new pointer[map_size];
            for (size_type b = 0; b < map_size; ++b)
            {
                if (other.map[b])
                {
                    map[b] = allocate_block();
                    for (size_type i = 0; i < BLOCK_SIZE; ++i)
                        alloc.construct(map[b] + i, other.map[b][i]);
                }
                else
                    map[b] = NULL;
            }

            start_block = other.start_block;
            start_index = other.start_index;
            end_block = other.end_block;
            end_index = other.end_index;
            sz = other.sz;

            return *this;
        }

        ~deque()
        {
            clear_storage();
        }

        size_type size() const { return sz; }
        bool empty() const { return sz == 0; }

        iterator begin() { return iterator(&map[start_block], start_index); }
        const_iterator begin() const
        {
            return const_iterator(&map[start_block], start_index);
        }
        iterator end() { return iterator(&map[end_block], end_index); }
        const_iterator end() const { return const_iterator(&map[end_block], end_index); }

        void push_back(const value_type &val)
        {
            if (end_index == BLOCK_SIZE)
            {
                ++end_block;
                end_index = 0;
                if (end_block >= map_size)
                    grow_map();
                if (!map[end_block])
                    map[end_block] = allocate_block();
            }
            alloc.construct(map[end_block] + end_index, val);
            ++end_index;
            ++sz;
        }

        void push_front(const value_type &val)
        {
            if (start_index == 0)
            {
                if (start_block == 0)
                    grow_map();
                --start_block;
                start_index = BLOCK_SIZE;
                if (!map[start_block])
                    map[start_block] = allocate_block();
            }
            --start_index;
            alloc.construct(map[start_block] + start_index, val);
            ++sz;
        }

        void pop_back()
        {
            if (sz == 0)
                throw std::out_of_range("deque::pop_back");
            if (end_index == 0)
            {
                --end_block;
                end_index = BLOCK_SIZE;
            }
            --end_index;
            alloc.destroy(map[end_block] + end_index);
            --sz;
        }

        void pop_front()
        {
            if (sz == 0)
                throw std::out_of_range("deque::pop_front");
            alloc.destroy(map[start_block] + start_index);
            ++start_index;
            if (start_index == BLOCK_SIZE)
            {
                ++start_block;
                start_index = 0;
            }
            --sz;
        }

        reference operator[](size_type n)
        {
            if (n >= sz)
                throw std::out_of_range("deque::operator[]");
            size_type abs_index = start_index + n;
            size_type block_offset = abs_index / BLOCK_SIZE;
            size_type idx = abs_index % BLOCK_SIZE;
            return map[start_block + block_offset][idx];
        }

        const_reference operator[](size_type n) const
        {
            if (n >= sz)
                throw std::out_of_range("deque::operator[]");
            size_type abs_index = start_index + n;
            size_type block_offset = abs_index / BLOCK_SIZE;
            size_type idx = abs_index % BLOCK_SIZE;
            return map[start_block + block_offset][idx];
        }
    };

} 
