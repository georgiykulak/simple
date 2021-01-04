#include <iostream>
#include <algorithm>
#include <cstring>

struct String {
	~String();
	String(const char*);
	String(size_t n, char c);

    String(String const&);
    String& operator=(String const&);

	void append(String const&);

    struct My_scoped_str {
        My_scoped_str(char* _str)
            : str{_str}
        {}

        char* str = nullptr;
    };

    String(const char* str, const size_t i)
        : String(str)
    {
        tmp_i = i;
    }

    String(My_scoped_str const& pstr)
        : String(pstr.str)
    {
        delete[] pstr.str;
    }

    String operator[](const size_t i) const
    {
        if ( i > size )
            return String(str, i);
        
        return String(str + i, i);
    }
    
    String operator[](const size_t j)
    {
        if ( j >= size + tmp_i )
            return *this;

        if ( j <= tmp_i )
            return String("");

        const auto shift = j - tmp_i;
        My_scoped_str new_str(new char[shift + 1]);
        std::copy(str, str + shift, new_str.str);
		*(new_str.str + shift) = '\0';

        return String(new_str);
    }

	size_t size;
	char* str;

private:
    size_t tmp_i;
};

inline void test_specific(String const& s,
        const size_t i, const size_t j);
inline void test_string(String const& s, std::string const& ss = "");
inline void test_operator(String const& s);

int main()
{
    String const hello("hello");
    test_string(hello);
    test_operator(hello);

    return EXIT_SUCCESS;
}

inline void test_specific(String const& s,
        const size_t i, const size_t j)
{
    String const l_str = s[i][j];
    test_string(l_str,
            std::string("i&j: ")
            + std::to_string(i)
            + " "
            + std::to_string(j));
}

inline void test_string(String const& s, std::string const& ss) 
{
    static size_t i = 0;
    std::cout << "Test String #" << i++ << '\t' << ss << std::endl;
    std::cout << "str  == \"" << std::string(s.str) << '"' << std::endl;
    std::cout << "size == " << s.size << std::endl;
}

inline void test_operator(String const& s)
{
    auto size  = s.size + 1 + 1;

    for ( auto i = 0; i < size; ++i ) {
        for ( auto j = 0; j < size; ++j ) {
            test_specific(s, i, j);
        }
    }
}

inline String::~String()
{
    delete[] str;
}

inline String::String(const char* str = "")
    : size{strlen(str)}
    , str(new char[size + 1])
{
    strcpy(this->str, str);
}

inline String::String(size_t n, char c)
    : size{n}
    , str(new char[size + 1])
{
    std::fill(str, str + size + 1, c);
}

inline String::String(String const& other)
    : size{other.size}
    , str(new char[size + 1])
{
    strcpy(str, other.str);
}

inline String& String::operator=(String const& other)
{
    if ( this != &other ) {
        delete[] str;
        size = other.size;
        str = new char[size + 1];
        strcpy(str, other.str);
    }

    return *this;
}

inline void String::append(String const& other) 
{
    char* temp = new char[size + 1];
    size_t tempsize = size;
    for ( size_t i = 0; i < tempsize + 1; ++i ) {
        temp[i] = str[i];
    }
    
    size += strlen(other.str);
    delete[] str;
    str = new char[size + 1];
    size_t i;
    for (i = 0; i < tempsize; ++i) {
        str[i] = temp[i];
    }
    delete[] temp;
    for (int k = 0; i < size; ++i, ++k) {
        str[i] = other.str[k];
    }
    str[size] = '\0';
}
