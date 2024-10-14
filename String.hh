#include <stddef.h>
#include <stdlib.h>
#include <iostream>
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

    //REQUIERED methods

    //create
    String();
    String(size_t size);
    ~String();

    //copy
    String(const String& reference);
    String(char const* c_str);
    String& operator = (const String& reference);

    //move
    String(String&& Rvalue) noexcept;
    String& operator = (String&& Lvalue) noexcept;

    //append
    String& operator += (const String& other);
    String& operator += (const char* other);

    //concat
    friend String operator+(const String& lhs, const String& rhs);
    friend String operator+(const String& lhs, const char* rhs);
    friend String operator+(const char* lhs, const String& rhs);


    //ADITIONAL methods
    //compare
    bool operator == (const String& other) const;
    bool operator < (const String& other) const;
    //geters
    inline size_t len() const { return length; }
    inline const char* c_str() const { return buffer; }
    inline char& operator [] (size_t idx) { return  buffer[idx]; }
    inline char const& operator [] (size_t idx) const { return buffer[idx]; }
    //cast
    inline operator const char* () const { return buffer; }
    inline operator bool() const { return length; }
    //resize
    void expand(size_t size);
    void enshure(size_t size);
    //streams
    void empty();
    friend std::ostream& operator << (std::ostream& right_stream, const String& string);
    friend std::istream& operator >> (std::istream& left_stream, String& string);

    //helper methods
    String& log() const;
private:
    //return closest power of 2 biger then argument
    static size_t pow2_capacity_calc(size_t length);
};