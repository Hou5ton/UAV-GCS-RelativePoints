#ifndef MATH_H
#define MATH_H

namespace math {

template<class T>
const T& max(const T& a, const T& b)
{
    return (a < b) ? b : a;
}

template<class T>
const T& min(const T& a, const T& b)
{
    return (b < a) ? b : a;
}

}

#endif // MATH_H
