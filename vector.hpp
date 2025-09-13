#ifndef FT_VECTOR_HPP
#define FT_VECTOR_HPP

#include <memory>
#include <stdexcept>
#include <iterator>
#include <cstddef>

namespace ft
{

    template <typename T, class Alloc = std::allocator<T>>
    class vector
    {
    public:
        typedef T value_type;
        typedef Alloc allocator_type;
        typedef typename Alloc::size_type size_type;
        typedef std::ptrdiff_t difference_type;
        typedef typename Alloc::reference reference;
        typedef typename Alloc::const_reference const_reference;
        typedef typename Alloc::pointer pointer;
        typedef typename Alloc::const_pointer const_pointer;

        class iterator
        {
        public:
            typedef std::random_access_iterator_tag iterator_category;
            typedef T value_type;
            typedef std::ptrdiff_t difference_type;
            typedef T *pointer;
            typedef T &reference;

        private:
            pointer _ptr;
        public:
            iterator() : _ptr(NULL) {}
            explicit iterator(pointer p) : _ptr(p) {}
            iterator(const iterator &other) : _ptr(other._ptr) {}
            iterator &operator=(const iterator &other)
            {
                _ptr = other._ptr;
                return *this;
            }

            reference operator*() const { return *_ptr; }
            pointer operator->() const { return _ptr; }

            iterator &operator++()
            {
                ++_ptr;
                return *this;
            }
            iterator operator++(int)
            {
                iterator tmp(*this);
                ++_ptr;
                return tmp;
            }
            iterator &operator--()
            {
                --_ptr;
                return *this;
            }
            iterator operator--(int)
            {
                iterator tmp(*this);
                --_ptr;
                return tmp;
            }

            iterator operator+(difference_type n)
                const { return iterator(_ptr + n); }
            iterator operator-(difference_type n)
                const { return iterator(_ptr - n); }
            difference_type operator-(const iterator &other)
                const { return _ptr - other._ptr; }
            iterator &operator+=(difference_type n)
            {
                _ptr += n;
                return *this;
            }
            iterator &operator-=(difference_type n)
            {
                _ptr -= n;
                return *this;
            }
            reference operator[](difference_type n)
                const { return _ptr[n]; }

            bool operator==(const iterator &other)
                const { return _ptr == other._ptr; }
            bool operator!=(const iterator &other)
                const { return _ptr != other._ptr; }
            bool operator<(const iterator &other)
                const { return _ptr < other._ptr; }
            bool operator>(const iterator &other)
                const { return _ptr > other._ptr; }
            bool operator<=(const iterator &other)
                const { return _ptr <= other._ptr; }
            bool operator>=(const iterator &other)
                const { return _ptr >= other._ptr; }

            pointer base() const { return _ptr; }
        };

        class const_iterator
        {
        public:
            typedef std::random_access_iterator_tag iterator_category;
            typedef T value_type;
            typedef std::ptrdiff_t difference_type;
            typedef const T *pointer;
            typedef const T &reference;

        private:
            pointer _ptr;

        public:
            const_iterator() : _ptr(NULL) {}
            explicit const_iterator(pointer p) : _ptr(p) {}
            const_iterator(const const_iterator &other) : _ptr(other._ptr) {}
            const_iterator(const iterator &other) : _ptr(other.base()) {}
            const_iterator &operator=(const const_iterator &other)
            {
                _ptr = other._ptr;
                return *this;
            }

            reference operator*() const
            {
                return *_ptr;
            }
            pointer operator->() const
            {
                return _ptr;
            }

            const_iterator &operator++()
            {
                ++_ptr;
                return *this;
            }
            const_iterator operator++(int)
            {
                const_iterator tmp(*this);
                ++_ptr;
                return tmp;
            }
            const_iterator &operator--()
            {
                --_ptr;
                return *this;
            }
            const_iterator operator--(int)
            {
                const_iterator tmp(*this);
                --_ptr;
                return tmp;
            }

            const_iterator operator+(difference_type n) const
            {
                return const_iterator(_ptr + n);
            }
            const_iterator operator-(difference_type n) const
            {
                return const_iterator(_ptr - n);
            }
            difference_type operator-(const const_iterator &other) const
            {
                return _ptr - other._ptr;
            }
            const_iterator &operator+=(difference_type n)
            {
                _ptr += n;
                return *this;
            }
            const_iterator &operator-=(difference_type n)
            {
                _ptr -= n;
                return *this;
            }
            reference operator[](difference_type n) const
            {
                return _ptr[n];
            }

            bool operator==(const const_iterator &other) const
            {
                return _ptr == other._ptr;
            }
            bool operator!=(const const_iterator &other) const
            {
                return _ptr != other._ptr;
            }
            bool operator<(const const_iterator &other) const
            {
                return _ptr < other._ptr;
            }
            bool operator>(const const_iterator &other) const
            {
                return _ptr > other._ptr;
            }
            bool operator<=(const const_iterator &other) const
            {
                return _ptr <= other._ptr;
            }
            bool operator>=(const const_iterator &other) const
            {
                return _ptr >= other._ptr;
            }

            pointer base() const { return _ptr; }
        };

        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    private:
        pointer _data;
        size_type _size;
        size_type _capacity;
        allocator_type _alloc;

    public:
        explicit vector(const allocator_type &alloc = allocator_type())
            : _data(NULL), _size(0), _capacity(0), _alloc(alloc) {}

        explicit vector(size_type n, const value_type &val = value_type(),
                        const allocator_type &alloc = allocator_type())
            : _data(NULL), _size(0), _capacity(0), _alloc(alloc)
        {
            _data = _alloc.allocate(n);
            for (size_type i = 0; i < n; i++)
                _alloc.construct(&_data[i], val);
            _size = n;
            _capacity = n;
        }

        vector(const vector &other)
            : _data(NULL), _size(0), _capacity(0), _alloc(other._alloc)
        {
            reserve(other._capacity);
            for (size_type i = 0; i < other._size; i++)
                _alloc.construct(&_data[i], other._data[i]);
            _size = other._size;
        }

        vector &operator=(const vector &other)
        {
            if (this != &other)
            {
                clear();
                if (_capacity < other._size)
                {
                    if (_data)
                        _alloc.deallocate(_data, _capacity);
                    _data = _alloc.allocate(other._capacity);
                    _capacity = other._capacity;
                }
                for (size_type i = 0; i < other._size; i++)
                    _alloc.construct(&_data[i], other._data[i]);
                _size = other._size;
            }
            return *this;
        }

        ~vector()
        {
            clear();
            if (_data)
                _alloc.deallocate(_data, _capacity);
        }

        iterator begin()
        {
            return iterator(_data);
        }
        const_iterator begin() const
        {
            return const_iterator(_data);
        }

        iterator end()
        {
            return iterator(_data + _size);
        }
        const_iterator end() const
        {
            return const_iterator(_data + _size);
        }

        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }

        size_type size() const
        {
            return _size;
        }
        size_type capacity() const
        {
            return _capacity;
        }
        size_type max_size() const
        {
            return _alloc.max_size();
        }
        void resize(size_t new_size, const T &value = T())
        {
            if (new_size < _size)
            {
                for (size_t i = new_size; i < _size; ++i)
                    _alloc.destroy(_data + i);
                _size = new_size;
            }
            else if (new_size > _size)
            {
                if (new_size > _capacity)
                    reserve(new_size);
                for (size_t i = _size; i < new_size; ++i)
                    _alloc.construct(_data + i, value);

                _size = new_size;
            }
        }

        void shrink_to_fit()
        {
            if (_size < _capacity)
            {
                T *new_data = _alloc.allocate(_size);
                for (size_t i = 0; i < _size; ++i)
                {
                    _alloc.construct(new_data + i, _data[i]);
                    _alloc.destroy(_data + i);
                }
                if (_data)
                    _alloc.deallocate(_data, _capacity);
                _data = new_data;
                _capacity = _size;
            }
        }

        bool empty() const
        {
            return _size == 0;
        }

        reference operator[](size_type n)
        {
            return _data[n];
        }
        const_reference operator[](size_type n) const
        {
            return _data[n];
        }

        reference at(size_type n)
        {
            if (n >= _size)
                throw std::out_of_range("vector::at");
            return _data[n];
        }

        const_reference at(size_type n) const
        {
            if (n >= _size)
                throw std::out_of_range("vector::at");
            return _data[n];
        }

        reference front()
        {
            return _data[0];
        }

        const_reference front() const
        {
            return _data[0];
        }

        reference back()
        {
            return _data[_size - 1];
        }

        const_reference back() const
        {
            return _data[_size - 1];
        }

        reference data()
        {
            return _data;
        }

        const_reference data() const
        {
            return _data;
        }

        void assign(size_type n, const value_type &val)
        {
            clear();
            if (n > _capacity)
                reserve(n);
            for (size_type i = 0; i < n; i++)
                _alloc.construct(&_data[i], val);
            _size = n;
        }

        void push_back(const value_type &val)
        {
            if (_size == _capacity)
                reserve(_capacity == 0 ? 1 : _capacity * 2);
            _alloc.construct(&_data[_size], val);
            ++_size;
        }

        void pop_back()
        {
            if (_size > 0)
            {
                --_size;
                _alloc.destroy(&_data[_size]);
            }
        }

        void insert(iterator position, const value_type &val)
        {
            size_type pos_index = position - begin();
            if (_size == _capacity)
                reserve(_capacity == 0 ? 1 : _capacity * 2);
            for (size_type i = _size; i > pos_index; i--)
                _alloc.construct(&_data[i], _data[i - 1]);
            _alloc.construct(&_data[pos_index], val);
            ++_size;
        }

        void insert(iterator position, size_type n, const value_type &val)
        {
            size_type pos_index = position - begin();
            if (_size + n > _capacity)
                reserve(_size + n > _capacity * 2 ? _size + n : _capacity * 2);
            for (size_type i = _size + n - 1; i >= pos_index + n; i--)
                _alloc.construct(&_data[i], _data[i - n]);
            for (size_type i = 0; i < n; i++)
                _alloc.construct(&_data[pos_index + i], val);
            _size += n;
        }

        void erase(iterator position)
        {
            size_type pos_index = position - begin();
            if (pos_index < _size)
            {
                _alloc.destroy(&_data[pos_index]);
                for (size_type i = pos_index; i < _size - 1; i++)
                {
                    _alloc.construct(&_data[i], _data[i + 1]);
                    _alloc.destroy(&_data[i + 1]);
                }
                --_size;
            }
        }

        void swap(vector &other)
        {
            std::swap(_data, other._data);
            std::swap(_size, other._size);
            std::swap(_capacity, other._capacity);
            std::swap(_alloc, other._alloc);
        }

        void clear()
        {
            for (size_type i = 0; i < _size; i++)
                _alloc.destroy(&_data[i]);
            _size = 0;
        }

        void reserve(size_type n)
        {
            if (n > _capacity)
            {
                pointer new_data = _alloc.allocate(n);
                for (size_type i = 0; i < _size; i++)
                    _alloc.construct(&new_data[i], _data[i]);
                for (size_type i = 0; i < _size; i++)
                    _alloc.destroy(&_data[i]);
                if (_data)
                    _alloc.deallocate(_data, _capacity);
                _data = new_data;
                _capacity = n;
            }
        }

        void emplace(iterator position, const value_type &val)
        {
            insert(position, val);
        }

        void emplace_back(const value_type &val)
        {
            push_back(val);
        }
    };

    template <typename T, class Alloc>
    bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        if (lhs.size() != rhs.size())
            return false;
        for (size_t i = 0; i < lhs.size(); i++)
        {
            if (lhs[i] != rhs[i])
                return false;
        }
        return true;
    }

    template <typename T, class Alloc>
    bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return !(lhs == rhs);
    }

    template <typename T, class Alloc>
    bool operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        size_t n = lhs.size() < rhs.size() ? lhs.size() : rhs.size();
        for (size_t i = 0; i < n; i++)
        {
            if (lhs[i] < rhs[i])
                return true;
            if (rhs[i] < lhs[i])
                return false;
        }
        return lhs.size() < rhs.size();
    }

    template <typename T, class Alloc>
    bool operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return !(rhs < lhs);
    }

    template <typename T, class Alloc>
    bool operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return rhs < lhs;
    }

    template <typename T, class Alloc>
    bool operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return !(lhs < rhs);
    }

}

#endif
