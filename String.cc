#include "String.hh"
#include <cstring>
#include <exception>
#include <immintrin.h>
#include <cstdio>


//default create constructor
inline String::String(const size_t capacity, const size_t length) :
    capacity(capacity),
    length(length) {
    buffer = reinterpret_cast<char*>(malloc(capacity));
    if (!buffer) throw std::bad_alloc();
    *(buffer + length) = '\0';
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

String::String(size_t size) :
    String(pow2_capacity_calc(size), 0) {}

String::~String() {
    free(buffer);
}

//Copy
String::String(const String& reference) :
    String(reference.capacity, reference.length, (const char*)reference) {}

String::String(const char* c_str) :
    String(pow2_capacity_calc(strlen(c_str)), strlen(c_str), c_str) {}

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

//Move
String::String(String&& Rvalue) noexcept :
    String(Rvalue.capacity, Rvalue.length) {
    this->buffer = Rvalue.buffer;
    Rvalue.buffer = nullptr;
}

String& String::operator = (String&& Rvalue) noexcept {
    if (this == &Rvalue) return *this;
    free(buffer);
    this->capacity = Rvalue.capacity;
    this->length = Rvalue.length;
    this->buffer = Rvalue.buffer;
    Rvalue.buffer = nullptr;
    return *this;
}

//append 
String& String::operator += (const String& other) {
    expand(other.length);
    std::memcpy(buffer + length, other.buffer, other.length + 1);
    length += other.length;
    return *this;
}

String& String::operator += (const char* other) {
    size_t c_str_len = strlen(other);
    expand(c_str_len);
    std::memcpy(buffer + length, other, c_str_len + 1);
    length += c_str_len;
    return *this;
}

//concat
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
}

String operator+(const char* lhs, const String& rhs) {
    size_t lhs_len = strlen(lhs);
    size_t result_len = lhs_len + rhs.length;
    String result(String::pow2_capacity_calc(result_len), result_len);
    std::memcpy(result.buffer, lhs, lhs_len);
    std::memcpy(result.buffer + lhs_len, rhs.buffer, rhs.length + 1);
    return result;
}


//ADITIONAL
//compare
bool String::operator == (const String& other) const {
    if (this == &other) return true;
    if (length != other.length) return false;
    char* iter1 = buffer, * iter2 = other.buffer;
    while (*iter1 && *iter1++ == *iter2++);
    return iter1 - buffer >= length;
};
//case inseceitive
bool String::operator < (const String& other) const {
    if (this == &other) return false;
    char* iter1 = buffer, * iter2 = other.buffer;
    char ch1, ch2;
    while (true) {
        ch1 = *iter1;
        ch2 = *iter2;
        //compare lowercase chars
        ch1 += 0x20 * ('A' <= ch1 && ch1 <= 'Z');
        ch2 += 0x20 * ('A' <= ch2 && ch2 <= 'Z');
        if (ch1 != ch2 || ch1) break;
        ++iter1;
        ++iter2;
    }
    return ch1 < ch2;
};

//resize
inline void String::enshure(size_t size) {
    if (capacity - 1 < size) {
        capacity = pow2_capacity_calc(size);
        buffer = reinterpret_cast<char*>(realloc(buffer, capacity));
        if (!buffer) throw std::bad_alloc();
        *(buffer + capacity - 1) = '\0';
    }

}

inline void String::empty() {
    length = 0;
    *buffer = '\0';
}

inline void String::expand(size_t size) {
    enshure(length + size);
}

//streams
std::ostream& operator << (std::ostream& right_stream, const String& string) {
    return (right_stream << string.buffer);
}
std::istream& operator >> (std::istream& stream, String& string) {
    /*
        string.empty();
        stream.getline(string.buffer + string.length, string.capacity - string.length, ' ');
        string.length += stream.gcount();
        while (stream.fail() && !stream.eof()) {//if there were not enough space in string buffer and some chars are left unread.
            string.expand(1);
            stream.clear();
            stream.getline(string.buffer + string.length, string.capacity - string.length, ' ');
            string.length += stream.gcount();
        }
    */
    char buffer[1024];
    stream >> buffer;
    string = buffer;
    return stream;
}

//helper methods
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

String& String::log() const {
    String& log = *new String("String{");
    char int_buff[20];

    sprintf(int_buff, "%p", this);
    (log += "ptr:") += int_buff;
    sprintf(int_buff, "%u", length);
    (log += ", length:") += int_buff;
    sprintf(int_buff, "%u", capacity);
    (log += ", capacity:") += int_buff;
    sprintf(int_buff, "%p", buffer);
    (log += ", buffer:") += int_buff;
    sprintf(int_buff, "%u", *(buffer + length) == '\0');
    (log += ", zero terminator?:") += int_buff;
    log += '}';
    return log;


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