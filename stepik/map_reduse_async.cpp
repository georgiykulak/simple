#include <iostream>
#include <future>
#include <list>
#include <vector>
#include <cassert>

template< typename BIterator, typename UnaryF, typename BinaryF >
auto map_reduce( BIterator p, BIterator q, UnaryF f1, BinaryF f2, size_t threads )
    -> decltype( std::declval< BinaryF& >()( *std::declval< BIterator >(), *std::declval< BIterator >() ) )
{
    return {};
}

int main()
{
    std::list< int > l = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    
    // параллельное суммирование в 3 потока
    auto sum = map_reduce(
            l.begin()
        ,   l.end()
        ,   []( int i ){ return i; }
        ,   std::plus< int >()
        ,   3
    );

    assert( sum == 55 );

    // проверка наличия чётных чисел в четыре потока
    auto has_even = map_reduce(
            l.begin()
        ,   l.end()
        ,   []( int i ){ return i % 2 == 0; }
        ,   std::logical_or< bool >()
        ,   4
    );

    assert( has_even == true );

    std::vector< std::string > const v = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
    for ( std::size_t i = 1; i <= v.size(); ++i )
    {
        auto sum = map_reduce(
                v.begin()
            ,   v.end()
            ,   []( std::string i ){ return i; }
            ,   std::plus< std::string >()
            ,   i
        );

        assert( sum == "1234567891011" );
    }

    return 0;
}