#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <limits>
#include <type_traits>
#include <utility>

namespace ft
{

    template <typename T, class Alloc = std::allocator<T>>
    class list
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

    private:
        struct Node
        {
            value_type data;
            Node *next;
            Node *prev;
            Node(const value_type &val) : data(val), next(nullptr), prev(nullptr) {}
            Node() : data(value_type()), next(nullptr), prev(nullptr) {}
        };
        Node *head;
        Node *tail;
        size_type _size;
        allocator_type _alloc;
        typedef typename Alloc::template rebind<Node>::other node_allocator;
        node_allocator node_alloc;

    public:
        class iterator
        {
        public:
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef T value_type;
            typedef std::ptrdiff_t difference_type;
            typedef T *pointer;
            typedef T &reference;

            iterator() : _node(nullptr) {}
            explicit iterator(Node *n) : _node(n) {}
            iterator(const iterator &other) : _node(other._node) {}

            iterator &operator=(const iterator &other)
            {
                _node = other._node;
                return *this;
            }

            reference operator*() const { return _node->data; }
            pointer operator->() const { return &_node->data; }

            iterator &operator++()
            {
                if (_node)
                    _node = _node->next;
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
                if (_node)
                    _node = _node->prev;
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
                return _node == other._node;
            }
            bool operator!=(const iterator &other) const
            {
                return _node != other._node;
            }

            Node *base() const { return _node; }

        private:
            Node *_node;
        };

        class const_iterator
        {
        public:
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef T value_type;
            typedef std::ptrdiff_t difference_type;
            typedef const T *pointer;
            typedef const T &reference;

            const_iterator() : _node(nullptr) {}
            explicit const_iterator(Node *n) : _node(n) {}
            const_iterator(const const_iterator &other) : _node(other._node) {}
            const_iterator(const iterator &other) : _node(other.base()) {}

            const_iterator &operator=(const const_iterator &other)
            {
                _node = other._node;
                return *this;
            }

            reference operator*() const { return _node->data; }
            pointer operator->() const { return &_node->data; }

            const_iterator &operator++()
            {
                if (_node)
                    _node = _node->next;
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
                if (_node)
                    _node = _node->prev;
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
                return _node == other._node;
            }
            bool operator!=(const const_iterator &other) const
            {
                return _node != other._node;
            }

            Node *base() const { return _node; }

            typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
            typedef std::reverse_iterator<iterator> reverse_iterator;

        private:
            Node *_node;
        };

    public:
        explicit list(const allocator_type &alloc = allocator_type())
            : head(nullptr), tail(nullptr), _size(0), _alloc(alloc), node_alloc() {}

        explicit list(size_type n, const value_type &val = value_type(),
                      const allocator_type &alloc = allocator_type())
            : head(nullptr), tail(nullptr), _size(0), _alloc(alloc), node_alloc()
        {
            for (size_type i = 0; i < n; i++)
                push_back(val);
        }
        ~list()
        {
            clear();
        }

        iterator begin()
        {
            return iterator(head);
        }
        const_iterator begin() const
        {
            return const_iterator(head);
        }

        iterator end()
        {
            return iterator(nullptr);
        }
        const_iterator end() const
        {
            return const_iterator(nullptr);
        }

        iterator rbegin()
        {
            return iterator(tail);
        }
        const_iterator rbegin() const
        {
            return const_iterator(tail);
        }

        iterator rend()
        {
            return iterator(nullptr);
        }
        const_iterator rend() const
        {
            return const_iterator(nullptr);
        }

        void clear()
        {
            while (!empty())
            {
                pop_back();
            }
        }

        size_type size() const
        {
            return _size;
        }
        bool empty() const
        {
            return _size == 0;
        }
        size_type max_size() const
        {
            return node_alloc.max_size();
        }

        reference front()
        {
            if (empty())
                throw std::out_of_range("list is empty");
            return head->data;
        }
        const_reference front() const
        {
            if (empty())
                throw std::out_of_range("list is empty");
            return head->data;
        }
        reference back()
        {
            if (empty())
                throw std::out_of_range("list is empty");
            return tail->data;
        }
        const_reference back() const
        {
            if (empty())
                throw std::out_of_range("list is empty");
            return tail->data;
        }

        void assign(size_type n, const value_type &val)
        {
            clear();
            for (size_type i = 0; i < n; i++)
                push_back(val);
        }

        template <typename InputIterator>
        typename std::enable_if<!std::is_integral<InputIterator>::value>::type
        assign(InputIterator first, InputIterator last)
        {
            clear();
            for (InputIterator it = first; it != last; ++it)
                push_back(*it);
        }

        void assign(std::initializer_list<value_type> il)
        {
            clear();
            for (const value_type &val : il)
                push_back(val);
        }

        template <typename InputIterator>
        typename std::enable_if<!std::is_integral<InputIterator>::value>::type
        insert(
            iterator position,
            InputIterator first,
            InputIterator last)
        {
            if (first == last)
                return;

            Node *next_node = position.node;
            Node *prev_node = next_node ? next_node->prev : tail;
            Node *first_new = nullptr;
            Node *last_new = nullptr;

            for (InputIterator it = first; it != last; ++it)
            {
                Node *node = node_alloc.allocate(1);
                node_alloc.construct(node, Node(*it));
                node->prev = prev_node;
                node->next = nullptr;

                if (prev_node)
                    prev_node->next = node;
                else
                    first_new = node;

                prev_node = node;

                if (!first_new)
                    first_new = node;
                last_new = node;
                ++_size;
            }

            if (next_node)
            {
                last_new->next = next_node;
                next_node->prev = last_new;
            }
            else
            {
                tail = last_new;
            }

            if (!head)
                head = first_new;
        }

        void insert(iterator position, const value_type &val)
        {
            Node *node = node_alloc.allocate(1);
            node_alloc.construct(node, Node(val));

            if (empty())
            {
                head = tail = node;
                node->prev = node->next = nullptr;
            }
            else if (position.node == head)
            {
                node->next = head;
                node->prev = nullptr;
                head->prev = node;
                head = node;
            }
            else if (position.node == nullptr)
            {
                node->prev = tail;
                node->next = nullptr;
                tail->next = node;
                tail = node;
            }
            else
            {
                Node *next_node = position.node;
                Node *prev_node = next_node->prev;
                node->prev = prev_node;
                node->next = next_node;
                prev_node->next = node;
                next_node->prev = node;
            }

            ++_size;
        }

        void insert(
            iterator position,
            size_type n,
            const value_type &value)
        {
            if (n == 0)
                return;

            Node *next_node = position.node;
            Node *prev_node = next_node ? next_node->prev : tail;

            for (size_type i = 0; i < n; ++i)
            {
                Node *node = node_alloc.allocate(1);
                node_alloc.construct(node, Node(value));
                node->prev = prev_node;
                node->next = nullptr;

                if (prev_node)
                    prev_node->next = node;
                else
                    head = node;

                prev_node = node;
            }

            if (next_node)
            {
                prev_node->next = next_node;
                next_node->prev = prev_node;
            }
            else
            {
                tail = prev_node;
            }

            _size += n;
        }

        void insert(

        )
        {
        }

        void emplace_front(const value_type &val)
        {
            push_front(val);
        }

        void emplace_back(const value_type &val)
        {
            push_back(val);
        }

        void push_back(const value_type &val)
        {
            Node *new_node = node_alloc.allocate(1);
            node_alloc.construct(new_node, Node(val));
            if (empty())
            {
                head = new_node;
                tail = new_node;
            }
            else
            {
                tail->next = new_node;
                new_node->prev = tail;
                tail = new_node;
            }
            _size++;
        }

        void push_front(const value_type &val)
        {
            Node *new_node = node_alloc.allocate(1);
            node_alloc.construct(new_node, Node(val));
            if (empty())
            {
                head = new_node;
                tail = new_node;
            }
            else
            {
                new_node->next = head;
                head->prev = new_node;
                head = new_node;
            }
            _size++;
        }

        void pop_back()
        {
            if (empty())
                throw std::out_of_range("list is empty");
            Node *to_delete = tail;
            if (head == tail)
            {
                head = nullptr;
                tail = nullptr;
            }
            else
            {
                tail = tail->prev;
                tail->next = nullptr;
            }
            node_alloc.destroy(to_delete);
            node_alloc.deallocate(to_delete, 1);
            _size--;
        }

        void pop_front()
        {
            if (empty())
                throw std::out_of_range("list is empty");
            Node *to_delete = head;
            if (head == tail)
            {
                head = nullptr;
                tail = nullptr;
            }
            else
            {
                head = head->next;
                head->prev = nullptr;
            }
            node_alloc.destroy(to_delete);
            node_alloc.deallocate(to_delete, 1);
            _size--;
        }

        void emplace() {}

        allocator_type get_allocator() const
        {
            return _alloc;
        }
    };
    template <typename T, class Alloc>
    bool operator==(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
    {
        if (lhs.size() != rhs.size())
            return false;
        typename list<T, Alloc>::const_iterator it1 = lhs.begin();
        typename list<T, Alloc>::const_iterator it2 = rhs.begin();
        for (; it1 != lhs.end() && it2 != rhs.end(); ++it1, ++it2)
        {
            if (*it1 != *it2)
                return false;
        }
        return true;
    }

    template <typename T, class Alloc>
    bool operator!=(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
    {
        return !(lhs == rhs);
    }

    template <typename T, class Alloc>
    bool operator<(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
    {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <typename T, class Alloc>
    bool operator>(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
    {
        return rhs < lhs;
    }

    template <typename T, class Alloc>
    bool operator<=(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
    {
        return !(rhs < lhs);
    }

    template <typename T, class Alloc>
    bool operator>=(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
    {
        return !(lhs < rhs);
    }

    template <typename T, class Alloc>
    void swap(list<T, Alloc> &lhs, list<T, Alloc> &rhs)
    {
        lhs.swap(rhs);
    }
}