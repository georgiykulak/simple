#include <iostream>
#include <cassert>

template < typename T, typename SizeType >
bool compare(T const& p, T const& q, SizeType (T::*foo)() const)
{
    return (p.*foo)() < (q.*foo)();
}

template < typename T, typename Foo >
bool cmp(T const& a, T const& b, Foo foo)
{
	return (a.*foo)() < (b.*foo)();
}

int main()
{
    std::string s1("Elf");
    std::string s2("Archer");

    // сравнение строк по длине
    bool r1 = compare(s1, s2, &std::string::size); // true
    assert(r1 == true);
    bool r2 = compare(s1, s1, &std::string::size); // false
    assert(r2 == false);

    r1 = cmp(s1, s2, &std::string::size); // true
    assert(r1 == true);
    r2 = cmp(s1, s1, &std::string::size); // false
    assert(r2 == false);

    return EXIT_SUCCESS;
}
