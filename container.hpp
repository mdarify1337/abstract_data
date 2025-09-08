#ifndef FT_VECTOR_HPP
#define FT_VECTOR_HPP

#include <memory>
#include <stdexcept>

namespace ft
{

    template <typename T, class Alloc = std::allocator<T>>
    class vector
    {
    public:
        typedef T value_type;
        typedef Alloc allocator_type;
        typedef typename Alloc::size_type size_type;
        typedef typename Alloc::reference reference;
        typedef typename Alloc::const_reference const_reference;
        typedef typename Alloc::pointer pointer;
        typedef typename Alloc::const_pointer const_pointer;

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

        size_type size() const { return _size; }
        size_type capacity() const { return _capacity; }
        bool empty() const { return _size == 0; }

        reference operator[](size_type n) { return _data[n]; }
        const_reference operator[](size_type n) const { return _data[n]; }

        reference at(size_type n)
        {
            if (n >= _size)
                throw std::out_of_range("vector::at");
            return _data[n];
        }

        // --- Modifiers ---
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
    };

}

#endif
