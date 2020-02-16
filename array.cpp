#include <iostream>
#include <cstddef>

class TestType {
    public:
    TestType(int data) : data{data} {}

    TestType(const TestType& obj)
    {
        data = obj.data;
    }
    
    ~TestType() = default;
    
    void setValue(int data) { this->data = data; }
    int getValue() const { return data; }

    private:
    TestType() = delete;
    TestType& operator= (const TestType& obj);
    int data;
};

template <typename T>
class Array
{
    public:
    explicit Array(size_t size, T const& value = T())
        : _data{nullptr}, _size{size}
    {
        if ( size ) {
            _data = static_cast<T*>(operator new[] (_size * sizeof(T)));
            for ( size_t i = 0; i < _size; ++i ) {
                new (_data + i) T(value);
            }
        }
    }
    
    ~Array()
    {
        if ( _data ) {
            for ( size_t i = 0; i < _size; ++i ) {
                _data[i].~T();
            }
            operator delete[] (_data);
        }
    }
    
    Array() : _data{nullptr}, _size{0} {}
    
    Array(Array const& obj)
    {
        if ( &obj != this ) {
            _size = obj._size;
            _data = static_cast<T*>(operator new[] (_size * sizeof(T)));
            for ( size_t i = 0; i < _size; ++i ) {
                new (_data + i) T(obj[i]);
            }
        }
    }
    
    Array& operator=(Array const& obj)
    {
        if ( &obj != this ) {
            if ( _data ) {
                for ( size_t i = 0; i < _size; ++i ) {
                    _data[i].~T();
                }
                operator delete[] (_data);
            }

            _size = obj._size;
            _data = static_cast<T*>(operator new[] (_size * sizeof(T)));
            for ( size_t i = 0; i < _size; ++i ) {
                new (_data + i) T(obj[i]);
            }
        }

        return *this;
    }

    size_t size() const { return _size; }
    
    T& operator[](size_t i) { return _data[i]; }
    const T& operator[](size_t i) const { return _data[i]; }
    
    private:
    size_t _size;
    T* _data;
};

int main()
{
    TestType obj1 = 9;
    Array<TestType> arr(5, obj1);
    for ( size_t i = 0; i < arr.size(); ++i ) {
        std::cout << arr[i].getValue() << " ";
    }
    std::cout << std::endl;

    return 0;
}
