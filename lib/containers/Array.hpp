
#pragma once

#include <stdlib.h>

template<class T, size_t N>
class Array
{
public:
    bool push_back(const T &x)
    {
        if (_size == N) {
            _overflow = true;
            return false;

        } else {
            _items[_size] = x;
            ++_size;
            return true;
        }
    }

    void remove(unsigned idx)
    {
        if (idx < _size) {
            --_size;

            for (unsigned i = idx; i < _size; ++i) {
                _items[i] = _items[i + 1];
            }
        }
    }

    void erase(T *item)
    {
        if (item - _items < static_cast<int>(_size)) {
            --_size;

            for (T *it = item; it != &_items[_size]; ++it) {
                *it = *(it + 1);
            }
        }
    }

    T &operator[](size_t n) { return _items[n]; }
    const T &operator[](size_t n) const { return _items[n]; }

    T &at(size_t n) { return _items[n]; }
    const T &at(size_t n) const { return _items[n]; }

    size_t size() const { return _size; }
    size_t max_size() const { return N; }
    size_t capacity() const { return N; }

    bool empty() const { return _size == 0; }

    bool is_overflowed() { return _overflow; }

    T *begin() { return &_items[0]; }
    T *end() { return &_items[_size]; }

    const T *begin() const { return &_items[0]; }
    const T *end() const { return &_items[_size]; }

private:
    T _items[N];
    size_t _size{0};
    bool _overflow{false};
};
