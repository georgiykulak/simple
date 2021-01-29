#include <iostream>
#include <future>
#include <list>
#include <vector>
#include <cassert>

template< typename BIterator, typename UnaryF, typename BinaryF >
auto map_reduce( BIterator p, BIterator q, UnaryF f1, BinaryF f2, size_t threads )
    -> decltype( std::declval< BinaryF& >()( *std::declval< BIterator >(), *std::declval< BIterator >() ) )
{
    auto const length = std::distance( p, q );
    std::size_t const cnt_per_thrd = length / threads;

    /*auto res = f1( *p );
    while( ++p != q )
        res = f2( res, f1( *p ) );*/

    auto res = f1( *p );
    ++p;
    for ( std::size_t t = 0; t < threads; ++t )
    {
        std::size_t counter = 0;
        for ( ; counter < cnt_per_thrd && p != q; ++p, ++counter )
        {
            res = f2( res, f1( *p ) );
        }
    }

    for ( ; p != q; ++p )
    {
        res = f2( res, f1( *p ) );
    }

    return res;
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

    std::cout << sum << std::endl;
    assert( sum == 55 );

    // проверка наличия чётных чисел в четыре потока
    auto has_even = map_reduce(
            l.begin()
        ,   l.end()
        ,   []( int i ){ return i % 2 == 0; }
        ,   std::logical_or< bool >()
        ,   4
    );

    std::cout << has_even << std::endl;
    assert( has_even == true );

    std::vector< std::string > const v = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
    for ( std::size_t i = 1; i <= v.size(); ++i )
    {
        auto ssum = map_reduce(
                v.begin()
            ,   v.end()
            ,   []( std::string i ){ return i; }
            ,   std::plus< std::string >()
            ,   i
        );

        std::cout << ssum << std::endl;
        assert( ssum == "1234567891011" );
    }

    return 0;
}