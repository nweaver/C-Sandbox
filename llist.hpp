#ifndef _LLIST_H
#define _LLIST_H

#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <functional>

// Yes, C++ standard containers already have a similar (and indeed more advanced)
// version called std::list, but we are doing this as an example of how
// to program references in C++ and similar work.

template <class T>
class LinkedListCell;
template <class T>
class LinkedList;

class LinkedListException : public std::exception
{
public:
    LinkedListException(std::string in) { _msg = in; }
    virtual const char *what() { return _msg.c_str(); }

protected:
    std::string _msg;
};

template <class T>
class LinkedListIterator : std::input_iterator_tag
{
public:
    void operator++()
    {
        _at = _at->_next;
    }

    bool operator!=(LinkedListIterator<T> &comp)
    {
        (void)comp; // Suppresses unused variable compiler warning
        return _at != nullptr;
    }

    T &operator*()
    {
        return _at->_data;
    }

    LinkedListIterator(std::shared_ptr<LinkedListCell<T>> at) { _at = at; }

protected:
    std::shared_ptr<LinkedListCell<T>> _at;
};

template <class T>
class ConstLinkedListIterator : std::input_iterator_tag
{
public:
    void operator++()
    {
        _at = _at->_next;
    }

    bool operator!=(const ConstLinkedListIterator<T> &comp)
    {
        (void)comp; // Suppresses unused variable compiler warning
        return _at != nullptr;
    }

    const T &operator*()
    {
        return _at->_data;
    }

    ConstLinkedListIterator(std::shared_ptr<LinkedListCell<T>> at) { _at = at; }

protected:
    std::shared_ptr<LinkedListCell<T>> _at;
};

template <class T>
class LinkedListCell
{
public:
    friend class LinkedList<T>;
    friend class LinkedListIterator<T>;
    friend class ConstLinkedListIterator<T>;

    LinkedListCell(const T &data, std::shared_ptr<LinkedListCell<T>> next)
    {
        _next = next;
        _data = data;
    }

protected:
    T _data;
    std::shared_ptr<LinkedListCell<T>> _next;
};

template <class T>
class LinkedList
{
public:
    LinkedList()
    {
        _head = nullptr;
        _tail = nullptr;
        _len = 0;
    }

    // In an attempt to bring things into line with
    // standard C++ libraries, assignment is a deep copy
    // (like string or vectors are).
    LinkedList &operator=(const LinkedList &other)
    {
        // For self assignment we do nothing...
        if (&other == this)
        {
            return *this;
        }

        // Otherwise, we erase our old data...
        _head = nullptr;
        _tail = nullptr;
        _len = 0;
        for (const auto &data : other)
        {
            append(data);
        }
        return *this;
    }

    LinkedList(const LinkedList &other)
    {
        _head = nullptr;
        _tail = nullptr;
        _len = 0;
        for (const auto &data : other)
        {
            append(data);
        }
    }

    virtual size_t len() { return _len; }

    virtual void prepend(const T &data)
    {
        _head = std::make_shared<LinkedListCell<T>>(data, _head);
        _len++;
    }

    virtual void append(const T &data)
    {
        if (!_head)
        {
            _head = std::make_shared<LinkedListCell<T>>(data, nullptr);
            _tail = _head;
            _len++;
            return;
        }

        _tail->_next = std::make_shared<LinkedListCell<T>>(data, nullptr);
        _tail = _tail->_next;
        _len++;
    }

    virtual T &operator[](size_t location)
    {
        auto at = _head;
        while (true)
        {
            if (at == nullptr)
                throw LinkedListException("Index out of range");
            if (location == 0)
                return at->_data;
            location--;
            at = at->_next;
        }
    }

    LinkedListIterator<T> begin() const
    {
        return LinkedListIterator<T>(_head);
    };
    LinkedListIterator<T> end() const
    {
        return LinkedListIterator<T>(nullptr);
    }

    ConstLinkedListIterator<T> cbegin() const
    {
        return ConstLinkedListIterator<T>(_head);
    };
    ConstLinkedListIterator<T> cend() const
    {
        return ConstLinkedListIterator<T>(nullptr);
    }

protected:
    std::shared_ptr<LinkedListCell<T>> _head;
    std::shared_ptr<LinkedListCell<T>> _tail;
    size_t _len;
};

template <class T>
std::string to_string(LinkedList<T> &in)
{
    std::stringstream s;
    s << "[";
    auto length = in.len();
    size_t i = 0;
    for (auto item : in)
    {
        s << item;
        i++;
        if (i < length)
            s << ", ";
    }
    s << "]";
    return s.str();
}

// Note that for all of these the need
// to return a new list does an additional copying.
//
// One is probably better served in C++20 to use the
// std::views | operations and just iterate rather than
// create a new list, but sometimes you do want a new list
template <class U, class T>
LinkedList<U> list_map(LinkedList<T> &in,
                       std::function<U(T)> f)
{
    LinkedList<U> ret;
    for (auto c : in)
    {
        ret.append(f(c));
    }
    return ret;
}

template <class U, class T>
U list_reduce(LinkedList<T> &in,
              std::function<U(U, T)> f, U initval)
{
    for (auto c : in)
    {
        initval = f(initval, c);
    }
    return initval;
}

template <class T>
LinkedList<T> list_filter(LinkedList<T> &in,
                          std::function<bool(T)> f)
{
    LinkedList<T> ret;
    for (auto c : in)
    {
        if (f(c))
            ret.append(c);
    }
    return ret;
}

#endif