#include <iostream>
#include <future>
#include <list>
#include <vector>
#include <cassert>
#include <type_traits>

// DON'T FORGET TO LINK "-lpthread" !!!

template< typename I, typename UF >
using MapReduceRetT =
    typename std::result_of< UF( decltype( *std::declval< I >() ) ) >::type;

template< typename BIterator, typename UnaryF, typename BinaryF >
auto map_reduce( BIterator p, BIterator q, UnaryF f1, BinaryF f2, size_t threads )
    -> MapReduceRetT< BIterator, UnaryF >
{
    auto length = std::distance( p, q );
    std::size_t const cnt_per_thrd = length / threads;

    MapReduceRetT< BIterator, UnaryF > res{};
    for ( std::size_t t = 0; t < threads; ++t )
    {
        std::size_t counter = 0;
        
        auto it = p;
        if ( t + 1 != threads )
            for ( ; counter < cnt_per_thrd && it != q; ++it, ++counter ) ;
        else
            for ( ; it != q; ++it ) ;

        auto lambda =
            [ &f1, &f2 ]( BIterator p, BIterator q )
                -> MapReduceRetT< BIterator, UnaryF >
            {
                auto res = f1( *p );
                while ( ++p != q )
                    res = f2( res, f1( *p ) );
                return res;
            };

        auto handle = std::async( std::launch::async, lambda, p, it );

        res = f2( res, handle.get() );
        p = it;
    }

    return res;
}

int main()
{
    std::list< int > const l1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    
    // параллельное суммирование в 3 потока
    auto sum = map_reduce(
            l1.begin()
        ,   l1.end()
        ,   []( int i ){ return i; }
        ,   std::plus< int >()
        ,   3
    );

    std::cout << sum << std::endl;
    assert( sum == 55 );

    // проверка наличия чётных чисел в четыре потока
    auto has_even = map_reduce(
            l1.begin()
        ,   l1.end()
        ,   []( int i ){ return i % 2 == 0; }
        ,   std::logical_or< bool >()
        ,   4
    );

    std::cout << has_even << std::endl;
    assert( has_even == true );

    std::vector< std::string > const v1 =
        { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
    for ( std::size_t j = 1; j <= v1.size(); ++j )
    {
        auto ssum = map_reduce(
                v1.begin()
            ,   v1.end()
            ,   []( std::string s ){ return s; }
            ,   std::plus< std::string >()
            ,   j
        );

        std::cout << ssum << std::endl;
        assert( ssum == "1234567891011" );
    }

    std::list< int > const l2 =
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

    for ( int i = 1; i < 20; ++i )
    {
        auto ssum = map_reduce(
                l2.begin()
            ,   l2.end()
            ,   []( int j ){ return j; }
            ,   std::plus< int >()
            ,   i
        );

        std::cout << ssum << std::endl;
        assert( ssum == 190 );
    }

    std::vector< std::string > const v2 =
        { "multithread", "and", "async", "in", "C++", "is", "total", "shit" };

    for ( std::size_t i = 1; i <= v1.size(); ++i )
    {    
        auto size_sum = map_reduce(
                v2.begin()
            ,   v2.end()
            ,   []( std::string s ){ return s.size(); }
            ,   std::plus< std::size_t >()
            ,   i
        );

        std::cout << size_sum << std::endl;
        assert( size_sum == 35 );
    }

    return 0;
}