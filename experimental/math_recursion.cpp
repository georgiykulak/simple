#include <iostream>
#include <cassert>
#include <functional>

// num = 0; factorial(num) = 1
// num = 1; factorial(num) = 1
// num = 2; factorial(num) = 1*2
// num = 5; factorial(num) = 1*2*3*4*5
unsigned long factorial ( unsigned long num )
{
    if ( num <= 1 )
        return 1;

    return num * factorial( num - 1 );
}

// num = 0; fibonacci(num) = 0
// num = 1; fibonacci(num) = 1
// num = 2; fibonacci(num) = 0 + 1
// num = 3; fibonacci(num) = (0 + 1) + 1
// num = 4; fibonacci(num) = 0 + (1 + 1) + 2
// num = 5; fibonacci(num) = 0 + 1 + ( 1 + 2 ) + 3
unsigned long fibonacci ( unsigned long num )
{
    if ( num <= 1 )
        return num;

    return fibonacci( num - 1 ) + fibonacci( num - 2 );
}

// https://oeis.org/A000217
// 1 + 3 + 6 + 10 + 15 + 21 + 28 + ...
// num = 0, 2, 4, 5, 7, 8, 9, 11 ...; triangle_sum(num) = 0
// num = 1; triangle_sum(num) = 1
// num = 3; triangle_sum(num) = 2
// num = 6; triangle_sum(num) = 3
// num = 10; triangle_sum(num) = 4
unsigned long triangle_sum ( unsigned long num )
{
    unsigned long counter = 0;
    unsigned long counter2 = 0;
    unsigned long result = 0;

    std::function< unsigned long ( unsigned long ) > triangle_sum_impl;
    triangle_sum_impl =
        [ &counter, &counter2, &result, &triangle_sum_impl ]
        ( unsigned long num )
        {
            if ( num != 0 )
                triangle_sum_impl( num - 1 );

            if ( num == counter2 )
            {
                result = counter;
                counter2 = num + (++counter);
            }
            else
            {
                result = 0;
            }

            return result;
        };
    
    unsigned long res_count = triangle_sum_impl( num );
    
    return res_count;
}

int main()
{
    assert( factorial( 0 ) == 1 );
    assert( factorial( 1 ) == 1 );
    assert( factorial( 2 ) == 2 );
    assert( factorial( 5 ) == 120 );
    assert( factorial( 10 ) == 3628800 );

    assert( fibonacci( 0 ) == 0 );
    assert( fibonacci( 1 ) == 1 );
    assert( fibonacci( 2 ) == 1 );
    assert( fibonacci( 5 ) == 5 );
    assert( fibonacci( 10 ) == 55 );

    assert( triangle_sum( 0 ) == 0 );
    assert( triangle_sum( 1 ) == 1 );
    assert( triangle_sum( 2 ) == 0 );
    assert( triangle_sum( 3 ) == 2 );
    assert( triangle_sum( 4 ) == 0 );
    assert( triangle_sum( 5 ) == 0 );
    assert( triangle_sum( 6 ) == 3 );
    assert( triangle_sum( 7 ) == 0 );
    assert( triangle_sum( 8 ) == 0 );
    assert( triangle_sum( 9 ) == 0 );
    assert( triangle_sum( 10 ) == 4 );

    return 0;
}