#ifndef _LLIST_H
#define _LLIST_H

#include <memory>

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

template <class T> class LinkedListIterator : std::input_iterator_tag {
    public:
    void operator++(){
        _at = _at->_next;
    }

    bool operator!=(LinkedListIterator<T> &comp){
        return _at == nullptr;
    }

    T& operator*(){
        return _at->_data;
    }

    LinkedListIterator(std::shared_ptr<LinkedListCell<T>> at){ _at = at;}

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

    size_t len() { return _len; }

    void prepend(T data)
    {
        _head = std::make_shared<LinkedListCell<T>>(data, _head);
    }

    void append(T data)
    {
        if (!_head)
        {
            prepend(data);
            return;
        }
        auto at = _head;
        while (at->_next)
        {
            at = at->_next;
        }
        at->_next = std::make_shared<LinkedListCell<T>>(data, nullptr);
    }

    T &operator[](int location)
    {
        auto at = _head;
        if (location < 0)
            throw LinkedListException("Index out of range");
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

    LinkedListIterator<T> begin(){
        return LinkedListIterator<T>(_head);
    };
    LinkedListIterator<T> end(){
        return LinkedListIterator<T>(nullptr);
    }

private:
    std::shared_ptr<LinkedListCell<T>> _head;
    size_t _len;
};

#endif