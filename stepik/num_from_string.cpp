#include <iostream>
#include <vector>
#include <sstream>

struct bad_from_string : public std::exception
{
    bad_from_string()
        :   m_s( "" )
    {}
    
    bad_from_string( std::string const & s )
        :   m_s( s )
    {}
    
    virtual const char * what() const noexcept
    {
        return m_s.c_str();
    }
    
private:
    std::string m_s;
};

template< class T >
T from_string( std::string const& s )
{
    T tmp{};
    
    if ( s.empty() )
    {
        throw bad_from_string( s );
        return tmp;
    }
    
    std::istringstream is( s );
    is >> std::noskipws >> tmp;

    if ( is.fail() || is.peek() != EOF )
        throw bad_from_string( s );
    
    return tmp;
}

using namespace std; 

#define _SFROM_STRING( type )                   \
try {                                           \
    cout << endl << "<" #type ">: '";           \
    cout << from_string< type >( str ) << "'";  \
}                                               \
catch ( exception const & e ) {                 \
    cout << "catch std::exception: "<< e.what();\
}                                               \
catch (...) {                                   \
    cout << "catch unknown";                    \
}

int main()
{
    vector< string > strings{ "123", "12.3", "", " ", "abc", " 123", "123 ", "12 3", "-1", "a", "a ", " a", "a b", "A" };
    //                 string  123    12.3   e    e   "abc"   e       e       e      "-1"  "a"   e     e     e     "A"
    //                 double  123    12.3   e    e    e      e       e       e       -1    e    e     e     e      e 
    //                    int  123    e      e    e    e      e       e       e       -1    e    e     e     e      e 
    //                   char  e      e      e   ' '   e      e       e       e       e    'a'   e     e     e     'A' 
    
    for ( auto & str : strings ) {
        cout << endl << "from_string(\'" << str << "\'):";
        _SFROM_STRING( string );
        _SFROM_STRING( double );
        _SFROM_STRING( int );
        _SFROM_STRING( char );
        cout << endl;
    }

    return 0;
}