#include <iostream>
#include <cassert>
#include <set>

//#define NO_DEBUG

#ifndef NO_DEBUG
#define DEBUG( out )    \
    std::cout << "Line=" << __LINE__ << " , " << out << std::endl;
#else
#define DEBUG( out ) ;
#endif // NO_DEBUG //

class Polynomial
{
public:
    using Set = std::multiset< Polynomial >;
    
    Polynomial () = default;

    Polynomial ( std::string const & expr )
    {
        if ( expr.empty() )
            return;
        
        auto pos = expr.find( "x" );
        if ( pos == std::string::npos )
        {
            coefficient = std::stoi( expr );
            return;
        }
        
        if ( pos == 0 )
            coefficient = 1;

        auto number = expr.substr( 0, pos );

        if ( number == "-" )
        {
            coefficient = -1;
            ++pos;
        }
        if ( number == "+" )
        {
            coefficient = 1;
            ++pos;
        }

        if ( pos == expr.size() )
        {
            exponent = 1;
            return;
        }

        if ( number.back() == '*' )
            number.pop_back();

        if ( !number.empty() && number != "+" && number != "-" )
            coefficient = std::stoi( number );

        pos = expr.find( "x^" );
        if ( pos == std::string::npos )
        {
            exponent = 1;
            return;
        }
        
        number = expr.substr( pos + 2 );
        exponent = std::stoi( number );
    }

    bool operator< ( Polynomial const & poly ) const
    {
        if ( exponent == poly.exponent )
            return coefficient > poly.coefficient;

        return exponent > poly.exponent;
    }

    Polynomial & operator+=( Polynomial const & poly )
    {
        if ( exponent == poly.exponent )
            coefficient += poly.coefficient;

        return *this;
    }

    std::string derivate () const
    {
        std::string res;

        if ( coefficient > 0 )
            res += '+';

        if ( exponent == 1 )
            return res + std::to_string( coefficient );
        
        auto const newCoef = coefficient * static_cast< signed >( exponent );
        auto const newExpn = exponent - 1;

        if ( newCoef == 0 )
            return std::string();

        if ( newExpn == 0 )
        {
            res += std::to_string( newCoef );
        }
        else if ( newCoef == 1 )
        {
            res += "x";
        }
        else
        {
            res += std::to_string( newCoef ) + "*x";
        }

        if ( newExpn == 1 )
            return res;

        res += '^' + std::to_string( newExpn );

        return res;
    }

    std::string toString () const
    {
        if ( coefficient == 0 )
            return std::string();

        std::string res;

        if ( coefficient > 0 )
            res += '+';
        
        if ( exponent == 0 )
        {
            res += std::to_string( coefficient );
        }
        else if ( coefficient == 1 )
        {
            res += "x";
        }
        else
        {
            res += std::to_string( coefficient ) + "*x";
        }

        if ( exponent > 1 )
            res += '^' + std::to_string( exponent );

        return res;
    }

    static std::string sum ( Set && ms )
    {
        std::string res;

        for ( auto it = ms.begin(); it != ms.end(); it = ms.upper_bound( *it ) )
        {
            if ( ms.count( *it ) > 1 )
            {
                Polynomial tmp;
                auto pr = ms.equal_range( *it );
        
                for ( auto rIt = pr.first; rIt != pr.second; ++rIt )
                {
                    tmp += *rIt;
                }
                
                res += tmp.toString();
                continue;
            }

            res += it->toString();
        }

        return res;
    }

private:
    unsigned exponent{ 0 };
    signed   coefficient{ 0 };
};

Polynomial::Set toPolynomialSet ( std::string const & str )
{
    Polynomial::Set pSet;
    std::size_t start{ 0 };

    for ( std::size_t i{ 1 }; i < str.size(); ++i )
    {
        if ( str[ i ] == '+' || str[ i ] == '-' )
        {
            auto const subStr = str.substr( start, i - start );
            pSet.insert( subStr );
            start = i;
        }
    }

    auto const subStr = str.substr( start );
    pSet.insert( subStr );

    return pSet;
}

std::string derivative ( std::string const & polyString )
{
    if ( polyString.empty() )
        return std::string();

    std::string res;

    for ( auto const & p : toPolynomialSet( polyString ) )
        res += p.derivate();

    if ( !res.empty() && res[ 0 ] == '+' )
        res.erase( 0, 1 );

    res = Polynomial::sum( std::move( toPolynomialSet( res ) ) );

    if ( !res.empty() && res[ 0 ] == '+' )
        res.erase( 0, 1 );

    return res;
}

int main()
{
    std::string test;

    // Test 0
    assert( "" == derivative( test ) );

    // Test 1
    test = "x^2+x";
    assert( "2*x+1" == derivative( test ) );

    // Test 2
    test = "2*x^100+100*x^2";
    assert( "200*x^99+200*x" == derivative( test ) );

    // Test 3
    test = "x^10000+x+1";
    assert( "10000*x^9999+1" == derivative( test ) );

    // Test 4
    test = "-x^2-x^3";
    assert( "-3*x^2-2*x" == derivative( test ) );
    //

    // Test 5
    test = "x+x+x+x+x+x+x+x+x+x";
    assert( "10" == derivative( test ) );
    
    return 0;
}