#include "String.hh"
#include <cstring>
#include <exception>
#include <immintrin.h>


//default create constructor
inline String::String(const size_t capacity, const size_t length) :
    capacity(capacity),
    length(length) {
    buffer = reinterpret_cast<char*>(malloc(capacity));
    if (!buffer) throw std::bad_alloc();
    *(buffer + capacity - 1) = '\0';
}

//default copy constructor
inline String::String(size_t capacity, size_t length, const char* buffer) :
    String(capacity, length) {
    std::memcpy(this->buffer, buffer, length + 1);
}



//REQUIERED 

//Basic constructors
//create
String::String() :
    String(DEFAULT_CAPACITY, 0) {}
//Copy
String::String(const String& reference) :
    String(reference.capacity, reference.length, (const char*)reference) {}
//Move
String::String(String&& Rvalue) noexcept :
    String(Rvalue.capacity, Rvalue.length) {
    this->buffer = Rvalue.buffer;
    Rvalue.buffer = nullptr;
};

//C-string copy constructor
String::String(const char* c_str) :
    String(pow2_capacity_calc(strlen(c_str)), strlen(c_str), c_str) {}

//copy assigment
String& String::operator = (const String& reference) {
    size_t req_capacity = pow2_capacity_calc(reference.length);
    if (capacity < req_capacity) {
        buffer = reinterpret_cast<char*>(realloc(buffer, req_capacity));
        if (!buffer) throw std::bad_alloc();
        *(buffer + capacity - 1) = '\0';
        capacity = req_capacity;
    }

    std::memcpy(this->buffer, reference.buffer, reference.length + 1);
    this->length = reference.length;
    return *this;
}

//move assigment
String& String::operator = (String&& Rvalue) noexcept {
    if (this == &Rvalue) return *this;
    free(buffer);
    this->capacity = Rvalue.capacity;
    this->length = Rvalue.length;
    this->buffer = Rvalue.buffer;
    return *this;
}

String::~String() {
    free(buffer);
}

//append String
String& String::operator += (const String& other) {
    expand(other.length);
    std::memcpy(buffer + length, other.buffer, other.length + 1);
    return *this;
};

//append C-string
String& String::operator += (const char* other) {
    size_t c_str_len = strlen(other);
    expand(c_str_len);
    std::memcpy(buffer + length, other, c_str_len + 1);
    return *this;
}

//string concatinations external methods
String operator+(const String& lhs, const String& rhs) {
    size_t result_len = lhs.length + rhs.length;
    String result(String::pow2_capacity_calc(result_len), result_len);
    std::memcpy(result.buffer, lhs.buffer, lhs.length);
    std::memcpy(result.buffer + lhs.length, rhs.buffer, rhs.length + 1);
    return result;
}
String operator+(const String& lhs, const char* rhs) {
    size_t rhs_len = strlen(rhs);
    size_t result_len = lhs.length + rhs_len;
    String result(String::pow2_capacity_calc(result_len), result_len);
    std::memcpy(result.buffer, lhs, lhs.length);
    std::memcpy(result.buffer + lhs.length, rhs, rhs_len + 1);
    return result;
};
String operator+(const char* lhs, const String& rhs) {
    size_t lhs_len = strlen(lhs);
    size_t result_len = lhs_len + rhs.length;
    String result(String::pow2_capacity_calc(result_len), result_len);
    std::memcpy(result.buffer, lhs, lhs_len);
    std::memcpy(result.buffer + lhs_len, rhs.buffer, rhs.length + 1);
    return result;
}


//ADITIONAL convertions
size_t String::len() const {
    return length;
}
/*
inline String::operator const char* () const {
    return buffer;
}
*/
String::operator bool() const {
    return length;
};
bool String::operator == (const String& other) const {
    if (this == &other) return true;
    if (length != other.length) return false;
    char* iter1 = buffer, * iter2 = other.buffer;
    while (*iter1 && *iter1++ == *iter2++);
    return iter1 - buffer >= length;
};
bool String::operator < (const String& other) const {
    if (this == &other) return false;
    char* iter1 = buffer, * iter2 = other.buffer;
    while (*iter1 && *iter1++ == *iter2++);
    return iter1 - buffer >= length;
};

char& String::operator [] (size_t idx) {
    return buffer[idx];
}
char const& String::operator [] (size_t idx) const {
    return buffer[idx];
}


//ADITIONAL nenory menagment
inline void String::enshure(size_t size) {
    if (capacity - 1 < size) {
        capacity = pow2_capacity_calc(size);
        buffer = reinterpret_cast<char*>(realloc(buffer, capacity));
        if (!buffer) throw std::bad_alloc();
        *(buffer + capacity - 1) = '\0';
    }

}

inline void String::expand(size_t size) {
    enshure(length + size);
}


inline size_t String::pow2_capacity_calc(size_t length) {
#if (defined(__GNUC__) || defined(__clang__)) //&& defined(__has_builtin) && __has_builtin(_bit_scan_reverse)
    //_bit_scan_reverse - index of highest set bit
    //_lzcnt_u64 - number of leading zeros
    //__builtin_clz- number of leading zeros
    return 2ULL << _bit_scan_reverse(length);
#else

    length |= length >> 1;
    length |= length >> 2;
    length |= length >> 4;
    length |= length >> 8;
    length |= length >> 16;
    length |= length >> 32;
    return length + 1;
#endif
}


/*
возможные пути развития
1) внедрение собственной утилиты копирывания текста с использыванием MMX, AVX инструкций
1.1) выравнивание буферов согласно инструкциям в наличии для выравненного чтения
1.2) использование спец инструкций для сдвига текста в регистрах для выравненой записи
1.3) приминение техник векторизации для ускорения за счёт паралелизма на уровне инструкций
2) внедрение SSO для ускорения работы с малыми строками.
3) Добавление методов для оптимизированноего обьединения множества сток.
4) Функционал для получения подстрок, вставки.
 */