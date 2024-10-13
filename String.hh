#include <stddef.h>
#include <stdlib.h>
#pragma once
class String {
private:
    static const size_t DEFAULT_CAPACITY = 16ULL;
    size_t capacity;
    //does not inlude zero terminator
    size_t length;
    char* buffer;


    String(const size_t capacity, const size_t length);
    String(size_t capacity, size_t length, const char* buffer);
    String(size_t capacity, size_t length, char* buffer);
public:

    //REQUIERED 
    //empty constructor
    String();

    //from C-string
    String(char const* c_str);

    //copy construcor
    String(const String& reference);

    //Move constructor
    String(String&& Rvalue) noexcept;

    //copy assigment
    String& operator = (const String& reference);

    //move assigment
    String& operator = (String&& Lvalue) noexcept;

    ~String();

    //append String
    String& operator += (const String& other);

    //append C-string
    String& operator += (const char* other);

    //string concatinations external methods
    friend String operator+(const String& lhs, const String& rhs);
    friend String operator+(const String& lhs, const char* rhs);
    friend String operator+(const char* lhs, const String& rhs);


    //ADITIONAL convertions
    inline size_t len() const;
    inline operator const char* () const { return buffer; }
    operator bool() const;
    bool operator == (const String& other) const;
    bool operator < (const String& other) const;
    char& operator [] (size_t idx);
    char const& operator [] (size_t idx) const;
    //ADITIONAL nenory menagment
    void expand(size_t size);
    void enshure(size_t size);
private:
    //void rebuffer(size_t size);
    static size_t pow2_capacity_calc(size_t length);

};