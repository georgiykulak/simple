#include <iostream>
#include <vector>
#include <list>

// [ 1, 2, 3, 4 ]<--->[ 5, 6 ]<--->[ 7, 8, 9 ]
// ^                  ^    ^
// |                  |    |
// |                  |    m_elemIt
// |                  m_vecIt
// m_data

template< class T >
class VectorList // std::deque, is it you ???
{
private:
    using VectT =   std::vector< T >;
    using ListT = std::list< VectT >;

public:
    using value_type = T;

    VectorList () = default;

    VectorList ( VectorList const & ) = default;
    
    VectorList ( VectorList && ) = default;
    
    VectorList & operator= ( VectorList && ) = default;
    
    VectorList & operator= ( VectorList const & ) = default;

    template< class It >
    void append ( It p, It q ); // defined outside

    bool empty () const { return size() == 0; } 

    size_t size () const 
    {
        size_t res = 0;

        for ( auto const & v : data_ )
            res += v.size();

        return res;
    }

    struct const_iterator : public std::iterator<
            std::bidirectional_iterator_tag // iterator_category
        ,   T const // value_type
        ,   decltype( std::declval< T* >() - std::declval< T* >() ) // difference_type
        ,   T const * // pointer
        ,   T const & // reference
    >
    {
    public:
        ~const_iterator () = default;

        const_iterator () = default;

        const_iterator ( const_iterator const & ci ) = default;

        const_iterator (
                ListT const * data
            ,   ListT::const_iterator vecIt
            ,   VectT::const_iterator elemIt
        )
            :   m_data{ data }
            ,   m_vecIt{ vecIt }
            ,   m_elemIt{ elemIt }
        {}
                
        const_iterator operator++ ( int )
        {
            const_iterator tmp = *this;

            this->operator++();

            return tmp;
        }
        
        const_iterator & operator++ ()
        {
            if ( !m_data )
                return *this;

            if ( m_elemIt != m_vecIt->cend() )
            {
                ++m_elemIt;
            }
            
            if ( m_elemIt == m_vecIt->cend() )
            {
                if ( m_vecIt != m_data->cend() )
                {
                    if ( std::next( m_vecIt ) == m_data->cend() )
                    {
                        m_elemIt = m_vecIt->cend();
                    }
                    else
                    {
                        ++m_vecIt;
                        m_elemIt = m_vecIt->cbegin();
                    }
                }
            }

            return *this;
        }

        const_iterator operator-- ( int )
        {
            const_iterator tmp = *this;

            this->operator--();

            return tmp;
        }
        
        const_iterator & operator-- ()
        {
            if ( !m_data )
                return *this;

            if ( m_elemIt != m_vecIt->cbegin() )
            {
                --m_elemIt;
            }
            else
            {
                if ( m_vecIt != m_data->cbegin() )
                {
                    --m_vecIt;
                    
                    m_elemIt = m_vecIt->cbegin();
                    while ( std::next( m_elemIt ) != m_vecIt->cend() )
                        ++m_elemIt;
                }
            }
            
            return *this;
        }

        bool operator== ( const_iterator const & ci ) const
        {
            return m_data == ci.m_data
                && m_vecIt == ci.m_vecIt
                && m_elemIt == ci.m_elemIt;
        }
        
        bool operator!= ( const_iterator const & ci ) const
        {
            return !( *this == ci );
        }
        
        std::iterator_traits< const_iterator >::reference operator* () const
        {
            return *m_elemIt;
        }

        std::iterator_traits< const_iterator >::pointer  operator-> () const
        {
            return &*m_elemIt;
        }
    
    private:
        ListT const * m_data = nullptr;
        ListT::const_iterator m_vecIt;
        VectT::const_iterator m_elemIt;
    };

    const_iterator begin () const
    {
        if ( data_.empty() )
            return const_iterator();
        
        auto it = data_.cbegin();
        return const_iterator( &data_, it, it->begin() );
    }

    const_iterator end ()   const
    {
        if ( data_.empty() )
            return const_iterator();
        
        auto it = std::prev( data_.cend() );
        return const_iterator( &data_, it, it->end() );
    }

    using const_reverse_iterator = std::reverse_iterator< const_iterator >;

    const_reverse_iterator rbegin () const
    {
        return const_reverse_iterator( end() );
    }
    
    const_reverse_iterator rend ()   const
    {
        return const_reverse_iterator( begin() );
    }

private:
    ListT data_;
};

template< class T >
template< class It >
void VectorList< T >::append ( It p, It q )
{
    if ( p != q )
        data_.push_back( VectT( p, q ) );
}

int main()
{
    VectorList< char > vList;

    std::vector< char > v1 = { 'A', 'B', 'C' };

    std::vector< char > v2 = { 'D', 'E', 'F', 'G' };
    vList.append( v1.begin(), v1.end() );
    vList.append( v2.begin(), v2.end() );

    auto i = vList.begin();

    std::cout << "Size is " << vList.size() << std::endl;
    std::cout << "begin is " << *i << std::endl;
    std::cout << "std::distance( begin, end ) is "
        << ( std::distance( vList.begin(), vList.end() ) ) << std::endl;
    std::cout << "*(++begin) == 'B'? "
        << ( *++vList.begin() == 'B' ) << std::endl;
    std::cout << "*(++begin) == 'A'? "
        << ( *++vList.begin() == 'A' ) << std::endl << std::endl;

    std::cout << "Test ++i" << std::endl;
    for ( i = vList.begin(); i != vList.end(); ++i )
        std::cout << *i << " ";
    std::cout << std::endl << std::endl;

    std::cout << "Test i++" << std::endl;
    for ( i = vList.begin(); i != vList.end(); i++ )
        std::cout << *i << " ";
    std::cout << std::endl << std::endl;

    std::cout << "Test --i" << std::endl;
    for ( i = vList.end(); i != vList.begin(); )
        std::cout << *--i << " ";
    std::cout << std::endl << std::endl;

    std::cout << "Test i--" << std::endl;
    for ( i = vList.end(); i != vList.begin(); )
    {
        i--;
        std::cout << *i << " ";
    }
    std::cout << std::endl << std::endl << std::endl;

    auto j = vList.rbegin();
    std::cout << "rbegin is " << *j << std::endl;
    j = --vList.rend();
    std::cout << "--rend is " << *j << std::endl << std::endl;

    std::cout << "Test reverse_const_iterator ++" << std::endl;
    for ( j = vList.rbegin(); j != vList.rend(); ++j )
        std::cout << *j << " ";
    std::cout << std::endl << std::endl;

    std::cout << "Test reverse_const_iterator --" << std::endl;
    for ( j = --vList.rend(); j != vList.rbegin(); --j )
        std::cout << *j << " ";
    std::cout << *j << std::endl << std::endl;

    VectorList< std::string > vList2;

    std::cout << std::boolalpha
        << ( std::distance( vList2.begin(), vList2.end() ) == 0 ) << std::endl;

    return 0;
}