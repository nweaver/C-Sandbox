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

private:
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

private:
    std::shared_ptr<LinkedListCell<T>> _at;
};

template <class T>
class LinkedListCell
{
public:
    friend class LinkedList<T>;
    friend class LinkedListIterator<T>;

    LinkedListCell(T data, std::shared_ptr<LinkedListCell<T>> next)
    {
        _next = next;
        _data = data;
    }

private:
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
        _len = 0;
    }

    LinkedList &operator=(const LinkedList &other)
    {
        _head = other._head;
        _len = other._len;
        return *this;
    }

    LinkedList(LinkedList &other)
    {
        _head = other._head;
        _len = other._len;
    }

    size_t len() { return _len; }

    void prepend(T data)
    {
        _head = std::make_shared<LinkedListCell<T>>(data, _head);
        _len++;
    }

    void append(T data)
    {
        if (!_head)
        {
            _head = std::make_shared<LinkedListCell<T>>(data, nullptr);
            _len++;
            return;
        }

        auto at = _head;
        while (at->_next)
        {
            at = at->_next;
        }
        _len++;
        at->_next = std::make_shared<LinkedListCell<T>>(data, nullptr);
    }

    T &operator[](size_t location)
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

    LinkedListIterator<T> begin()
    {
        return LinkedListIterator<T>(_head);
    };
    LinkedListIterator<T> end()
    {
        return LinkedListIterator<T>(nullptr);
    }

private:
    std::shared_ptr<LinkedListCell<T>> _head;
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