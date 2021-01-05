#include <iostream>
#include <map>

class SmallAllocator
{
public:
    using byte_type = char *;
    using size_type = unsigned;

    void * Alloc ( size_type size )
    {
        return ReAlloc( nullptr, size );
    }

    void * ReAlloc ( void * ptr, size_type size )
    {
        if ( !ptr )
            return reinterpret_cast< void * >( alloc( size ) );

        auto it = m_heap.find( reinterpret_cast< byte_type >( ptr ) );
        if ( it == m_heap.end() )
            return reinterpret_cast< void * >( alloc( size ) );

        if ( it->first - m_mem >= size )
        {
            m_heap[ it->first ] = size;
            return ptr;
        }

        auto newPtr = alloc( size );
        if ( !newPtr )
            return nullptr;

        for ( size_type i = 0; i < it->second; ++i )
        {
            *( newPtr + i ) = *( it->first + i );
        }

        m_heap.erase( it );

        return newPtr;
    }

    void Free ( void * ptr )
    {
        if ( !ptr )
            return;

        auto it = m_heap.find( reinterpret_cast< byte_type >( ptr ) );
        if ( it != m_heap.end() )
            m_heap.erase( it );
    }

private:
    static constexpr size_type m_max_size = 1048576;
    // sizeof( m_mem ) == 2^20, from address m_mem + 0 to m_mem + 1048576
    /* static */ char m_mem[ m_max_size ]; 
    // map of "address" and offset
    /* static */ std::map< byte_type, size_type > m_heap;

    byte_type alloc ( size_type size )
    {
        if ( !size )
            return nullptr;

        byte_type back;
        auto prev = m_heap.crbegin(); // maximum

        if ( prev != m_heap.crend() )
        {
            if ( prev->first + prev->second + size > m_mem + m_max_size )
                back = defragment(); // last entry
            else
                back = prev->first + prev->second;
        }
        else
        {
            back = m_mem; // first entry
        }
        
        if ( back )
            m_heap.insert( { back, size } );

        return back;
    }

    byte_type defragment ()
    {
        // TODO
        return nullptr;
    }
};

int main()
{
    // Test 1
    SmallAllocator A1;
    int * A1_P1 = ( int * )A1.Alloc( sizeof( int ) );
    A1_P1 = ( int * )A1.ReAlloc( A1_P1, 2 * sizeof( int ) );
    A1.Free( A1_P1 );

    // Test 2
    SmallAllocator A2;
    int * A2_P1 = ( int * )A2.Alloc( 10 * sizeof( int ) );
    
    for ( unsigned i = 0; i < 10; i++ )
        A2_P1[ i ] = i;

    for ( unsigned i = 0; i < 10; i++ )
        if ( A2_P1[ i ] != i )
            std::cout << "ERROR 1" << std::endl;
    
    int * A2_P2 = ( int * )A2.Alloc( 10 * sizeof( int ) );
    
    for ( unsigned i = 0; i < 10; i++ )
        A2_P2[ i ] = -1;
    
    for ( unsigned i = 0; i < 10; i++ )
        if ( A2_P1[ i ] != i )
            std::cout << "ERROR 2" << std::endl;
    
    for ( unsigned i = 0; i < 10; i++ )
        if ( A2_P2[i] != -1 )
            std::cout << "ERROR 3" << std::endl;
    
    A2_P1 = ( int * )A2.ReAlloc( A2_P1, 20 * sizeof( int ) );

    for ( unsigned i = 10; i < 20; i++ )
        A2_P1[i] = i;
    
    for ( unsigned i = 0; i < 20; i++ )
        if ( A2_P1[i] != i )
            std::cout << "ERROR 4" << std::endl;
    
    for ( unsigned i = 0; i < 10; i++ )
        if ( A2_P2[i] != -1 )
            std::cout << "ERROR 5" << std::endl;
    
    A2_P1 = ( int * )A2.ReAlloc( A2_P1, 5 * sizeof( int ) );
    
    for ( unsigned i = 0; i < 5; i++ )
        if ( A2_P1[i] != i )
            std::cout << "ERROR 6" << std::endl;
    
    for ( unsigned i = 0; i < 10; i++ )
        if( A2_P2[i] != -1 )
            std::cout << "ERROR 7" << std::endl;
    
    A2.Free( A2_P1 );
    A2.Free( A2_P2 );

    return 0;
}