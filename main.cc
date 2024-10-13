#include "String.hh"
#include  <iostream>
int main() {
    String i_am_string = String("Hello World");
    String new_str = i_am_string + "Buy My tomatoes";
    std::cout << (const char*)new_str << std::endl;
}