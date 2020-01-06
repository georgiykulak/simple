#include <iostream>

struct Expression {
    public:
    std::string getName() const { return str; }
    Expression(std::string str = "empty") : str{str} {}
    private:
    std::string str;
};

class SharedPtr
{
    public:
    ~SharedPtr()
    {
        _delete();
    }

    explicit SharedPtr(Expression* ptr = 0) :
        ptr{ptr},
        cnt{nullptr}
    {
        if ( ptr )
            cnt = new size_t(1);
    }

    SharedPtr(const SharedPtr& obj) :
        ptr{obj.ptr},
        cnt{obj.cnt}
    {
        if ( cnt )
            ++*cnt;
    }

    SharedPtr& operator=(const SharedPtr& obj)
    {
        if ( &obj != this ) {
            if ( ptr )
                _delete();
            if ( obj.ptr )
                ++*obj.cnt;
            ptr = obj.ptr;
            cnt = obj.cnt;
        }

        return *this;
    }
    
    void reset(Expression *p = 0)
    {
        if ( ptr ) {
            _delete();
            ptr = p;
            if ( !p )
                cnt = nullptr;
            else
                cnt = new size_t(1);
        }
        else {
            if ( p ) {
                ptr = p;
                cnt = new size_t(1);
            }
        }
    }
    
    Expression* get() const { return ptr; }

    Expression& operator*() const { return *ptr; }
    Expression* operator->() const { return ptr; }
    
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

int main()
{
    SharedPtr p1;
    {
        SharedPtr p2(new Expression("expr1"));
        SharedPtr p3(new Expression("expr2"));
        SharedPtr p4(p2);
        SharedPtr p5;
        p5 = p2;
        p5 = p4;
        p1 = p5;
        p3.reset(NULL);
        p3 = p5;
        p5.reset(NULL);
        SharedPtr p6;
        SharedPtr p7;
        p7 = p7;
        p7.reset(NULL);
        p7.reset(new Expression("expr3"));
        SharedPtr p8(new Expression("expr4"));
        p8.reset(NULL);
    }
    p1 = p1;

    return EXIT_SUCCESS;
}
