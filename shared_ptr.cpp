#include <iostream>

struct Expression {
    public:
    std::string getName() const { return str; }
    Expression(std::string str = "empty") : str{str} {}
    private:
    std::string str;
};

struct SharedPtr
{
    public:
    ~SharedPtr()
    {
        std::cout << "Destructor 1: ";
        std::cout << "pointer = ";
        if ( ptr ) std::cout << ptr << " " << ptr->getName() << "\t";
        else std::cout << ptr << " <=adress\t";
        std::cout << "counter = ";
        if ( cnt ) std::cout << *cnt << "\n"; //debug
        else std::cout << cnt << " <=adress\n";
        _delete();///////////////////////////////////////////////////////////////////////
        std::cout << "Destructor 2: ";
        std::cout << "pointer = ";
        if ( ptr ) std::cout << ptr << " " << ptr->getName() << "\t";
        else std::cout << ptr << " <=adress\t";
        std::cout << "counter = ";
        if ( cnt ) std::cout << *cnt << "\n"; //debug
        else std::cout << cnt << " <=adress\n";
    }

    explicit SharedPtr(Expression* ptr = 0) :
        ptr(ptr),
        cnt(0)
    {
        if ( ptr )
            cnt = new size_t(1);
    }

    SharedPtr(const SharedPtr& obj) :
        ptr(obj._get_ptr()),
        cnt(obj._get_cnt())
    {
        if ( cnt )
            ++*cnt;
    }

    SharedPtr& operator=(const SharedPtr& obj)
    {
        Expression* p = obj._get_ptr();
        size_t*     c = obj._get_cnt();   
        if ( ptr != p )
        if ( &obj != this ) {
            if ( ptr )
                _delete();
            if ( p )
                ++*c;
            ptr = p;
            cnt = c;
        }

        return *this;
    }
    
    void reset(Expression *ptr = 0)
    {
        if ( this->ptr ) {
            _delete();
        }
        else if ( ptr ) {
            cnt = new size_t(1);
        }
        this->ptr = ptr;
    }
    
    Expression* get() const { return _get_ptr(); }

    Expression& operator*() const { return *ptr; }
    Expression* operator->() const { return ptr; }
    
    Expression* _get_ptr() const { return ptr; }
    size_t*     _get_cnt() const { return cnt; }

    private:
    Expression* ptr;
    size_t*     cnt;

    void _delete()
    {
        if ( cnt && (--*cnt == 0) ) {
            delete ptr;
            ptr = nullptr;
            delete cnt;
            cnt = nullptr;
        }
    }
};

inline void test(const SharedPtr& obj)
{
    static size_t i = 0;
    std::cout << ++i;
    Expression* p = obj._get_ptr();
    size_t*     c = obj._get_cnt();
    std::cout << "\tp = " << p;
    std::cout <<  " c = " << c;
    if ( p ) std::cout << " *p = " << p->getName();
    if ( c ) std::cout << " *c = " << *c;
    std::cout << "\n\n";
}

int main()
{
SharedPtr p1;
test(p1);                                   // 1
{
    SharedPtr p2(new Expression("expr1"));
    test(p2);                               // 2
    SharedPtr p3(new Expression("expr2"));
    test(p3);                               // 3
    SharedPtr p4(p2);
    test(p4);                               // 4
    SharedPtr p5;
    test(p5);                               // 5
    p5 = p2;
    test(p5);                               // 6
    test(p2);                               // 7
    p5 = p4;
    test(p5);                               // 8
    test(p4);                               // 9
    p1 = p5;
    test(p1);                               // 10
    test(p5);                               // 11
    p3.reset(NULL);
    test(p3);                               // 12
    p3 = p5;
    test(p3);                               // 13
    test(p5);                               // 14
    p5.reset(NULL);
    test(p5);                               // 15
    SharedPtr p6;
    SharedPtr p7;
    p7 = p7;
    test(p7);                               // 16
    p7.reset(NULL);
    p7.reset(new Expression("expr3"));
    test(p7);                               // 17
    SharedPtr p8(new Expression("expr4"));
    test(p8);                               // 18
    p8.reset(NULL);
    test(p8);                               // 19
}
p1 = p1;
test(p1);                                   // 20

    return EXIT_SUCCESS;
}
