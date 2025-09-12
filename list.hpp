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

            Node *next_node = position.base();
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
            else if (position.base() == head)
            {
                node->next = head;
                node->prev = nullptr;
                head->prev = node;
                head = node;
            }
            else if (position.base() == nullptr)
            {
                node->prev = tail;
                node->next = nullptr;
                tail->next = node;
                tail = node;
            }
            else
            {
                Node *next_node = position.base();
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

            Node *next_node = position.base();
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

        void emplace(
            iterator position,
            const value_type &value)
        {
            insert(position, value);
        }

        iterator erase(iterator position)
        {
            Node *node = position.base();
            if (!node)
                return end();

            Node *prev_node = node->prev;
            Node *next_node = node->next;
            if (prev_node)
                prev_node->next = next_node;
            else
                head = next_node;
            if (next_node)
                next_node->prev = prev_node;
            else
                tail = prev_node;
            node_alloc.destroy(node);
            node_alloc.deallocate(node, 1);
            --_size;
            return iterator(next_node);
        }

        iterator erase(
            iterator first,
            iterator last)
        {
            while (first != last)
                first = erase(first);
            return last;
        }

        void swap(list &other)
        {
            Node *tmp_head = head;
            head = other.head;
            other.head = tmp_head;
            Node *tmp_tail = tail;
            tail = other.tail;
            other.tail = tmp_tail;
            size_t tmp_size = _size;
            _size = other._size;
            other._size = tmp_size;
            std::swap(node_alloc, other.node_alloc);
        }

        void resize(size_t new_size, const T &value = T())
        {
            if (new_size < _size)
                while (_size > new_size)
                    erase(--end());
            else if (new_size > _size)
                while (_size < new_size)
                    push_back(value);
        }

        allocator_type get_allocator() const
        {
            return _alloc;
        }

        void splice(iterator pos, list &other)
        {
            if (other.empty())
                return;

            Node *before = pos.base() ? pos.base()->prev : tail;
            if (before)
            {
                before->next = other.head;
                other.head->prev = before;
            }
            else
            {
                head = other.head;
            }

            if (pos.base())
            {
                pos.base()->prev = other.tail;
                other.tail->next = pos.base();
            }
            else
            {
                tail = other.tail;
            }

            _size += other._size;
            other.head = other.tail = 0;
            other._size = 0;
        }

        void splice(iterator pos, list &other, iterator it)
        {
            Node *node = it.base();
            if (!node)
                return;
            if (node->prev)
                node->prev->next = node->next;
            else
                other.head = node->next;

            if (node->next)
                node->next->prev = node->prev;
            else
                other.tail = node->prev;

            --other._size;

            Node *before = pos.base() ? pos.base()->prev : tail;

            node->prev = before;
            node->next = pos.base();

            if (before)
                before->next = node;
            else
                head = node;

            if (pos.base())
                pos.base()->prev = node;
            else
                tail = node;

            ++_size;
        }

        void splice(iterator pos, list &other, iterator first, iterator last)
        {
            if (first == last)
                return;
            Node *first_node = first.base();
            Node *last_node = last.base() ? last.base()->prev : other.tail;

            Node *before_first = first_node->prev;
            Node *after_last = last_node->next;

            if (before_first)
                before_first->next = after_last;
            else
                other.head = after_last;

            if (after_last)
                after_last->prev = before_first;
            else
                other.tail = before_first;
            size_t count = 0;
            for (Node *n = first_node; n != after_last; n = n->next)
                ++count;

            other._size -= count;
            Node *before = pos.base() ? pos.base()->prev : tail;

            first_node->prev = before;
            last_node->next = pos.base();

            if (before)
                before->next = first_node;
            else
                head = first_node;

            if (pos.base())
                pos.base()->prev = last_node;
            else
                tail = last_node;

            _size += count;
        }

        void remove(const value_type &value)
        {
            iterator it = begin();
            while (it != end())
            {
                if (*it == value)
                    it = erase(it);
                else
                    ++it;
            }
        }

        template <class Predicate>
        void remove_if(Predicate pred)
        {
            iterator it = begin();
            while (it != end())
            {
                if (pred(*it))
                    it = erase(it);
                else
                    ++it;
            }
        }

        void unique()
        {
            if (empty())
                return;

            iterator first = begin();
            iterator next = first;
            ++next;
            while (next != end())
            {
                if (*first == *next)
                    next = erase(next);
                else
                {
                    first = next;
                    ++next;
                }
            }
        }

        template <class BinaryPredicate>
        void unique(BinaryPredicate pred)
        {
            if (empty())
                return;
            iterator first = begin();
            iterator next = first;
            ++next;
            while (next != end())
            {
                if (pred(*first, *next))
                    next = erase(next);
                else
                {
                    first = next;
                    ++next;
                }
            }
        }

        void sort()
        {
            if (_size < 2)
                return;

            list carry;
            list counter[64];
            int fill = 0;
            while (!empty())
            {
                carry.splice(carry.begin(), *this, begin());
                int i = 0;
                while (i < fill && !counter[i].empty())
                {
                    counter[i].merge(carry);
                    carry.swap(counter[i]);
                    ++i;
                }
                carry.swap(counter[i]);
                if (i == fill)
                    ++fill;
            }

            for (int i = 1; i < fill; ++i)
            {
                counter[i].merge(counter[i - 1]);
            }
            swap(counter[fill - 1]);
        }

        template <class Compare>
        void sort(Compare comp)
        {
            if (_size < 2)
                return;

            list carry;
            list counter[64];
            int fill = 0;

            while (!empty())
            {
                carry.splice(carry.begin(), *this, begin());
                int i = 0;
                while (i < fill && !counter[i].empty())
                {
                    counter[i].merge(carry, comp);
                    carry.swap(counter[i]);
                    ++i;
                }
                carry.swap(counter[i]);
                if (i == fill)
                    ++fill;
            }

            for (int i = 1; i < fill; ++i)
            {
                counter[i].merge(counter[i - 1], comp);
            }
            swap(counter[fill - 1]);
        }

        void merge(list &other)
        {
            if (this == &other)
                return;
            iterator it1 = begin();
            iterator it2 = other.begin();
            while (it1 != end() && it2 != other.end())
            {
                if (*it2 < *it1)
                {
                    iterator next = it2;
                    ++next;
                    splice(it1, other, it2);
                    it2 = next;
                }
                else
                    ++it1;
            }
            if (it2 != other.end())
                splice(end(), other, it2, other.end());
        }

        void reverse()
        {
            if (_size <= 1)
                return;

            Node *current = head;
            Node *tmp = 0;

            while (current)
            {
                tmp = current->prev;
                current->prev = current->next;
                current->next = tmp;
                current = current->prev; 
            }
            tmp = head;
            head = tail;
            tail = tmp;
        }

        template <class Compare>
        void merge(list &other, Compare comp)
        {
            if (this == &other)
                return;
            iterator it1 = begin();
            iterator it2 = other.begin();
            while (it1 != end() && it2 != other.end())
            {
                if (comp(*it2, *it1))
                {
                    iterator next = it2;
                    ++next;
                    splice(it1, other, it2);
                    it2 = next;
                }
                else
                    ++it1;
            }
            if (it2 != other.end())
                splice(end(), other, it2, other.end());
        }
    };

    template <typename T, class Alloc>
    bool operator==(
        const list<T, Alloc> &lhs,
        const list<T, Alloc> &rhs)
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
    bool operator!=(
        const list<T, Alloc> &lhs,
        const list<T, Alloc> &rhs)
    {
        return !(lhs == rhs);
    }

    template <typename T, class Alloc>
    bool operator<(
        const list<T, Alloc> &lhs,
        const list<T, Alloc> &rhs)
    {
        return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                            rhs.begin(), rhs.end());
    }

    template <typename T, class Alloc>
    bool operator>(
        const list<T, Alloc> &lhs,
        const list<T, Alloc> &rhs)
    {
        return rhs < lhs;
    }

    template <typename T, class Alloc>
    bool operator<=(
        const list<T, Alloc> &lhs,
        const list<T, Alloc> &rhs)
    {
        return !(rhs < lhs);
    }

    template <typename T, class Alloc>
    bool operator>=(
        const list<T, Alloc> &lhs,
        const list<T, Alloc> &rhs)
    {
        return !(lhs < rhs);
    }

}