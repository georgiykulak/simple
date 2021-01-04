#include <iostream>
#include <vector>

#define DEBUG

namespace task {
    int strstr(const char *text, const char *pattern)
    {
        if ( !*pattern )
            return 0;
    
        for ( int i = 0; *text; ++text, ++i ) {
            const char* t = text;
            const char* p = pattern;
            while ( *p++ == *t++ ) {
                if ( !*p )
                    return i;
            }
        }

        return -1;
    }

    struct test {
        int ans;
        std::string str;
        std::string sub;
    };

    inline void testbench(const int& n, const test& t)
    {
        const int& answer =
            strstr(t.str.c_str(), t.sub.c_str());

        std::cout << "Test\t#" << n;
        if ( t.ans == answer )
            std::cout << "\tSUCCESSED:\t"
                << t.ans << " == " << answer;
        else
            std::cout << "\tFAILED:\t"
                << t.ans << " != " << answer;
        std::cout << std::endl;
    }

    std::vector<test> testcase = {
        { 0,       "", "" },
        { 0,      "a", "" },
        { 0,      "a", "a"},
        {-1,      "a", "b"},
        { 0,     "aa", "" },
        { 0,     "aa", "a"},
        { 0,     "ab", "a"},
        { 1,     "ba", "a"},
        {-1,     "bb", "a"},
        { 0,    "aaa", "" },
        { 0,    "aaa", "a"},
        { 1,    "abc", "b"},
        { 2,    "abc", "c"},
        {-1,    "abc", "d"},
        {-1,      "a", "aa"},
        {-1,      "a", "ba"},
        {-1,      "a", "ab"},
        {-1,      "a", "bb"},
        {-1,      "a", "aaa"},
        {-1,     "aa", "aaa"},
        { 0,    "aaa", "aaa"},
        { 0,   "aaab", "aaa"},
        { 1,   "baaa", "aaa"},
        { 1,  "baaaa", "aaa"},
        { 1,  "baaab", "aaa"},
        {-1,    "abd", "abc"},
        { 2,  "ababc", "abc"},
        { 3, "abdabc", "abc"}
    };
}

int main()
{

#ifdef DEBUG
    size_t i = 1;
    for ( auto& t: task::testcase ) {
        task::testbench(i++, t);
    }
#endif

    return EXIT_SUCCESS;
}
