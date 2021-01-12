#include "Shape.h"

bool is_intersect_r_r( Shape * a, Shape * b )
{
    std::cout << "Called '" << __FUNCTION__ << "' with " << a->name() << " and " << b->name() << std::endl;
    assert( a->name() == "Rectangle" );
    assert( b->name() == "Rectangle" );
    return {};
}

bool is_intersect_r_t( Shape * a, Shape * b )
{
    std::cout << "Called '" << __FUNCTION__ << "' with " << a->name() << " and " << b->name() << std::endl;
    assert( a->name() == "Rectangle" );
    assert( b->name() == "Triangle" );
    return {};
}

/*--------------------------------------------------------------------------------------------------------------------*/

bool is_intersect_t_t( Shape * a, Shape * b )
{
    std::cout << "Called '" << __FUNCTION__ << "' with " << a->name() << " and " << b->name() << std::endl;
    assert( a->name() == "Triangle" );
    assert( b->name() == "Triangle" );
    return {};
}

bool is_intersect_t_r( Shape * a, Shape * b )
{
    std::cout << "Called '" << __FUNCTION__ << "' with " << a->name() << " and " << b->name() << std::endl;
    assert( a->name() == "Triangle" );
    assert( b->name() == "Rectangle" );
    return {};
}