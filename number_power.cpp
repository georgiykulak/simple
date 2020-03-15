#include <iostream>
#include <cmath>

class Number {
public:
    struct _PowerPointer {
        _PowerPointer(long double _n)
            : m_p{_n}
        {}

        long double m_p;
    };

    friend long double operator* (Number const& _n,
                           _PowerPointer const& _p);

    ~Number() = default;

    explicit Number(long double _n)
        : m_n{_n}
    {}

    Number(Number const& _n)
        : m_n{_n.m_n}
    {}

    // unar operator for power
    _PowerPointer operator*() const
    {
        return _PowerPointer(m_n);
    }

    // power operator for power
    long double operator*(Number const& _e) const
    {
        return m_n * _e.m_n;
    }

private:
    long double m_n;
};

long double operator* (Number const& _n,
        Number::_PowerPointer const& _p)
{
    return std::pow(_n.m_n, _p.m_p);
}

int main()
{
    Number base(5.0);
    Number  exp(2.0);

    std::cout << base *  exp << std::endl;
    std::cout << base ** exp << std::endl;

    return EXIT_SUCCESS;
}
