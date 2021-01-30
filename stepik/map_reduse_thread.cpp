#include <iostream>
#include <cassert>
#include <list>
#include <thread>

#include <mutex>
#include <vector>
#include <type_traits>

// DON'T FORGET TO LINK "-lpthread" !!!

std::mutex g_mutex;

template< typename I, typename UF, typename BF >
using MapReduceRetT =
    decltype(
        std::declval< BF >()(
                std::declval< UF >()( *std::declval< I >() )
            ,   std::declval< UF >()( *std::declval< I >() )
        )
    );

template< typename BIterator, typename UnaryF, typename BinaryF >
MapReduceRetT< BIterator, UnaryF, BinaryF > map_reduce (
        BIterator p
    ,   BIterator q
    ,   UnaryF f1
    ,   BinaryF f2
    ,   size_t threads
)
{
    using RetT = MapReduceRetT< BIterator, UnaryF, BinaryF >;

    assert( threads );
    
    auto length = std::distance( p, q );

    assert( length );

    std::size_t const cnt_per_thrd = length / threads;

    std::vector< RetT > partedResults( threads );

    auto lambda =
        [ &f1, &f2, &partedResults ]
        ( BIterator p, BIterator q, std::size_t i )
        {
            std::lock_guard< std::mutex > l_guard( g_mutex );
            partedResults[ i ] = f1( *p );
            while ( ++p != q )
                partedResults[ i ] = f2( partedResults[ i ], f1( *p ) );
        };

    std::vector< std::thread > resVector;
    resVector.reserve( threads );
    
    for ( std::size_t t = 0; t < threads; ++t )
    {
        std::size_t counter = 0;
        
        auto it = p;
        if ( t + 1 != threads )
            for ( ; counter < cnt_per_thrd && it != q; ++it, ++counter ) ;
        else
            for ( ; it != q; ++it ) ;

        resVector.emplace_back(
            std::thread( lambda, p, it, t )
        );

        p = it;
    }

    for ( auto & fut : resVector )
        fut.join();

    RetT res{};

    for ( auto const & pr : partedResults )
        res = f2( res, pr );

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

    assert( sum == 55 );

    // проверка наличия чётных чисел в четыре потока
    auto has_even = map_reduce(
            l1.begin()
        ,   l1.end()
        ,   []( int i ){ return i % 2 == 0; }
        ,   std::logical_or< bool >()
        ,   4
    );

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

        assert( ssum == 190 );
    }

    std::vector< std::string > const v2 =
        { "multithread", "and", "async", "in", "C++", "is", "total", "sh!t" };

    // If "i <= v2.size()": on Stepik OK, there NOT OK
    for ( std::size_t i = 1; i <= 8; ++i )
    {    
        auto size_sum = map_reduce(
                v2.begin()
            ,   v2.end()
            ,   []( std::string s ){ return s.size(); }
            ,   std::plus< std::size_t >()
            ,   i
        );

        assert( size_sum == 35 );
    }

    return 0;
}