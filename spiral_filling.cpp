#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
    std::vector<std::vector<long>> mtx;
    std::vector<long>::size_type sz;
    size_t iter = 0;

    std::cout << "Enter length of matrix: ";
    std::cin >> sz;

    mtx.resize(sz);
    for ( auto& v: mtx ) {
        v.resize(sz);
    }
    
    std::vector<long>::size_type start = 0;
    std::vector<long>::size_type finish = sz - 1;
    const bool parity = sz % 2;
    bool flag = true;

    sz = parity ? sz - 1 : sz;
    while ( sz-- ) {
        do {
            auto i = start;
            auto j = finish;
            for ( auto k = i; k != j; flag ? ++k : --k ) {
                mtx[start][k] = iter++;
            }
            for ( auto k = i; k != j; flag ? ++k : --k ) {
                mtx[k][finish] = iter++;
            }
            std::swap(start, finish);
        } while ( !(flag = !flag) );
        --finish;
        ++start;
        --sz;
    }
    
    auto mid = mtx.size() / 2;
    if ( parity )
        mtx[mid][mid] = iter;

    for ( const auto& m: mtx ) {
        for ( const auto& v: m ) {
            std::cout << v << "\t";
        }
        std::cout << "\n\n\n";
    }

    return EXIT_SUCCESS;
}
