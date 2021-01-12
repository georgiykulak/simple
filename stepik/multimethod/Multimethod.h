#ifndef MULTIMETHOD_MULTIMETHOD_H
#define MULTIMETHOD_MULTIMETHOD_H

#include <map>
#include <typeindex>
#include <functional>

// Base - базовый класс иерархии
// Result - тип возвращаемого значения мультиметода
// Commutative - флаг, который показывает, что
// мультиметод коммутативный (т.е. f(x,y) = f(y,x)).
template< class Base, class Result, bool Commutative >
class Multimethod2
{
public:
    using F = std::function< Result ( Base *, Base * ) >;
    using Pair = std::pair< std::type_index, std::type_index >;

    // устанавливает реализацию мультиметода
    // для типов t1 и t2 заданных через typeid
    // f - это функция или функциональный объект
    // принимающий два указателя на Base
    // и возвращающий значение типа Result
    void addImpl ( std::type_index t1, std::type_index t2, F f )
    {
        m_dict.insert( { { t1, t2 }, f } );
    }

    // проверяет, есть ли реализация мультиметода
    // для типов объектов a и b
    bool hasImpl ( Base * a, Base * b ) const
    {
        std::type_index t1 = typeid( *a );
        std::type_index t2 = typeid( *b );

        auto funcIt = m_dict.find( { t1, t2 } );
        
        if ( Commutative && funcIt == m_dict.end() )
        {
            funcIt = m_dict.find( { t2, t1 } );
        }

        return funcIt != m_dict.end();
    }

    // Применяет мультиметод к объектам
    // по указателям a и b
    Result call ( Base * a, Base * b ) const
    {
        if ( !a || !b )
            return Result();

        std::type_index t1 = typeid( *a );
        std::type_index t2 = typeid( *b );

        auto funcIt = m_dict.find( { t1, t2 } );
        
        if ( Commutative && funcIt == m_dict.end() )
        {
            funcIt = m_dict.find( { t2, t1 } );
                
            return ( funcIt->second )( b, a );
        }

        return ( funcIt->second )( a, b );
    }

private:    
    std::map< Pair, F > m_dict;
};

#endif // MULTIMETHOD_MULTIMETHOD_H //
