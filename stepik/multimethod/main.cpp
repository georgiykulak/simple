#include <iostream>
#include "Multimethod.h"
#include "Shape.h"

int main()
{
    // мультиметод для наследников Shape
    // возращающий bool и являющийся коммутативным
    Multimethod2< Shape, bool, true > is_intersect;

    // Test 1

    // добавляем реализацию мультиметода для двух прямоугольников
    is_intersect.addImpl( typeid( Rectangle ), typeid( Rectangle ), is_intersect_r_r );

    // добавляем реализацию мультиметода для прямоугольника и треугольника
    is_intersect.addImpl( typeid( Rectangle ), typeid( Triangle ), is_intersect_r_t );

    // создаём две фигуры
    Shape * s1 = new Triangle();
    Shape * s2 = new Rectangle();

    // проверяем, что реализация для s1 и s2 есть
    assert( is_intersect.hasImpl( s1, s2 ) );
    if ( is_intersect.hasImpl( s1, s2 ) )
    {
        // вызывается функция is_intersect_r_t(s2, s1)
        std::cout << "Should be called is_intersect_r_t:\n";
        bool res = is_intersect.call( s1, s2 );

        // Замечание: is_intersect_r_t ожидает,
        // что первым аргументом будет прямоугольник
        // а вторым треугольник, а здесь аргументы
        // передаются в обратном порядке.
        // ваша реализация должна самостоятельно
        // об этом позаботиться
    }

    // Should NOT be called
    is_intersect.call( s1, s1 );

    // Test 2
    
    is_intersect.addImpl( typeid( Triangle ), typeid( Triangle ), is_intersect_t_t );

    is_intersect.addImpl( typeid( Triangle ), typeid( Rectangle ), is_intersect_t_r );

    std::cout << "Should be called is_intersect_r_r:\n";
    is_intersect.call( s2, s2 );

    assert( is_intersect.hasImpl( s2, s1 ) );
    if ( is_intersect.hasImpl( s2, s1 ) )
    {
        std::cout << "Should be called is_intersect_r_t:\n";
        is_intersect.call( s2, s1 );
    }

    return 0;
}
