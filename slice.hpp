#ifndef SLICE_HPP
#define SLICE_HPP

#include <vector>
#include <memory>

class SliceException : public std::exception
{
public:
    SliceException(const std::string &in) { _msg = in; }
    virtual const char *what() { return _msg.c_str(); }

protected:
    std::string _msg;
};

class SliceOutOfBoundsException : public SliceException
{
public:
    SliceOutOfBoundsException(const std::string &in) : SliceException(in) {};
};

template <class T>
class Slice
{
public:
    Slice()
    {
        _data = std::make_shared<std::vector<T>>();
        _start = 0;
        _len = 0;
    }

    // Subrange
    Slice(Slice &s, int start, int end)
    {
        _data = s._data;
        auto tmp = ((int)(s._start) + start);
        if (tmp < 0)
            throw SliceException("Negative Start");
        if (end < start)
            throw SliceException("End before start");
        _start = (size_t)tmp;
        _len = (size_t)end - start + 1;
        if (_len > _data->size())
            throw SliceException("Can't have slice beyond end");
    }

    Slice(std::vector<T> &from, int start, int end) {
        _data = std::make_shared<std::vector<T>>(from);
        auto tmp = (start);
        if (tmp < 0)
            throw SliceException("Negative Start");
        if (end < start)
            throw SliceException("End before start");
        _start = (size_t)tmp;
        _len = (size_t)end - start + 1;
        if (_len > _data->size())
            throw SliceException("Can't have slice beyond end");
    }

    void push_back(const T &value)
    {
        if (_start + _len >= _data->size())
        {
            _data->push_back(value);
            _len += 1;
        }
        else {
            _len += 1;
            (*this)[_len-1] = value;
        }
    }

    T &operator[](size_t pos)
    {
        if (pos >= _len)
            throw SliceOutOfBoundsException("Exceeded Bounds");
        if (pos + _start >= _data->size())
            throw SliceOutOfBoundsException("Exceeds Size");
        return (*_data)[pos + _start];
    }

private:
    std::shared_ptr<std::vector<T>> _data;
    size_t _start;
    size_t _len;


};

#endif
