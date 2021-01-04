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
DEBUG( "START , expr='" << expr << "'" );
        if ( expr.empty() )
            return;
        
        std::size_t i = 0;

        auto pos = expr.find( "x" );
DEBUG( "expr.find( \"x\" )='" << pos << "'" );
        if ( pos == std::string::npos )
        {
            coefficient = std::stoi( expr );
            return;
        }
        
        if ( pos == 0 )
            coefficient = 1;

        auto number = expr.substr( 0, pos );
DEBUG( "expr.substr( 0, pos )='" << number << "'" );

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
DEBUG( "coefficient='" << coefficient << "' exponent='" << exponent << "'" );
            return;
        }

        if ( number.back() == '*' )
            number.pop_back();

        if ( !number.empty() && number != "+" && number != "-" )
            coefficient = std::stoi( number );

        pos = expr.find( "x^" );
DEBUG( "expr.find( \"x^\" )='" << pos << "'" );
        if ( pos == std::string::npos )
        {
            exponent = 1;
            return;
        }
        
        number = expr.substr( pos + 2 );
DEBUG( "expr.substr( pos + 1 )='" << number << "'" );
        exponent = std::stoi( number );

DEBUG( "FINISH , coefficient='" << coefficient << "' exponent='" << exponent << "'" );
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
DEBUG( "START " << __FUNCTION__ );
        std::string res;

DEBUG( "Before check ( coefficient > 0 )" );
        if ( coefficient > 0 )
            res += '+';

DEBUG( "Before check ( exponent == 1 ) , res='" << res << "'" );
        if ( exponent == 1 )
            return res + std::to_string( coefficient );
        
        auto const newCoef = coefficient * static_cast< signed >( exponent );
        auto const newExpn = exponent - 1;

DEBUG( "Before check ( newCoef == 0 ) , res='" << res << "'" );
        if ( newCoef == 0 )
            return std::string();

DEBUG( "Before check of ( newExpn == 0 ) , res='" << res << "'" );
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

DEBUG( "Before check ( newExpn == 1 ) , res='" << res << "'" );
        if ( newExpn == 1 )
            return res;

DEBUG( "After check ( newExpn == 1 ) , res='" << res << "'" );
        res += '^' + std::to_string( newExpn );

DEBUG( "START " << __FUNCTION__ << " res='" << res << "'" );
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

    friend std::string sum ( Set && ); 

private:
    unsigned exponent{ 0 };
    signed   coefficient{ 0 };
};

std::string sum ( Polynomial::Set && ms )
{
    std::string res;

int i = 0;
    for ( auto it = ms.begin(); it != ms.end(); it = ms.upper_bound( *it ) )
    {
DEBUG( "In loop start " << i++ );
        std::cout << "{ coefficient = " << it->coefficient << " | exponent = " << it->exponent << " }\n";
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
DEBUG( "In loop finish " << i++ << " , res='" << res << "'" );
    }

    return res;
}

Polynomial::Set toPolynomialSet ( std::string const & str )
{
    Polynomial::Set pSet;
    std::size_t start{ 0 };

    for ( std::size_t i{ 1 }; i < str.size(); ++i )
    {
        if ( str[ i ] == '+' || str[ i ] == '-' )
        {
DEBUG( "Inside check of opeartor between polynomials" );
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

DEBUG( "res='" << res << "'" );

    res = /*Polynomial::*/sum( std::move( toPolynomialSet( res ) ) );

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
    /*auto res = derivative( test );
    std::cout << "derivative( test ) = '" << res << "'\n";*/
    assert( "2*x+1" == res );

    ///*
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
    //*/
    return 0;
}