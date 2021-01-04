#include <iostream>
#include <cassert>
#include <set>

// I can use regexp in this task

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

    friend std::string sum ( Set && ); 

private:
    unsigned exponent{ 0 };
    signed   coefficient{ 0 };
};

std::string sum ( Polynomial::Set && ms )
{
    std::string res;

    for ( auto it = ms.begin(); it != ms.end(); it = ms.upper_bound( *it ) )
    {
        if ( ms.count( *it ) > 1 )
        {
            Polynomial tmp;
            auto pr = ms.equal_range( *it );
            tmp.exponent = pr.first->exponent;
    
            for ( auto rIt = pr.first; rIt != pr.second; ++rIt )
            {
                tmp += *rIt;
            }
            
            res += tmp.toString();
            continue;
        }
        
        res += it->toString();
    }

    if ( res.empty() )
        res = "0";

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

    res = sum( std::move( toPolynomialSet( res ) ) );

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

    // Test 6
    test = "8*x^2-3*x^4+9*x^2";
    assert( "-12*x^3+34*x" ==  derivative( test ) );

    // Test 7
    test = "0*x^0";
    assert( "0" ==  derivative( test ) );

    // Test 8
    test = "234";
    assert( "0" ==  derivative( test ) );

    return 0;
}