#include <iostream>
#include <cstdio>
#include <iomanip>

constexpr static const int N = 14;

namespace hackerrank {

void in() {}

void out() {}

template < typename T, typename... Args >
void in(T&& tmp, Args&&... args)
{
    std::cin >> tmp;
    in(args...);
}

template < typename T, typename... Args >
void out(T tmp, Args... args)
{
    std::cout << std::setprecision(N) << tmp << std::endl;
    out(args...);
}

} // hackerrank 

int main()
{
    int i;
    long l;
    char a;
    float f;
    double d;

    hackerrank::in(i, l, a, f, d);
    hackerrank::out(i, l, a, f, d);

    return 0;
}
