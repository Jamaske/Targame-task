#include "String.hh"
#include  <iostream>
#include <set>

int main() {
    /*
    String s0;
    String s1 = String("");
    String s2("ab");
    String s3("aa");
    String s12 = s1 + s2;
    String s23 = (String(s2) += s3);

    std::cout << "s0 == s1: " << (s0 == s1) << '\n';
    std::cout << "s1 < s2: " << (s1 < s2) << '\n';
    std::cout << "s2 < s3: " << (s2 < s3) << '\n';
    std::cout << "s2 < s23: " << (s2 < s23) << '\n';
    std::cout << s0.log().c_str() << std::endl;
    std::cout << s1.log().c_str() << std::endl;
    std::cout << s2.log().c_str() << std::endl;
    std::cout << s3.log().c_str() << std::endl;
    std::cout << s12.log().c_str() << std::endl;
    std::cout << s23.log().c_str() << std::endl;
    std::cout << s1.log().log().c_str() << std::endl;
    std::cout << s1.log().log().log().c_str() << std::endl;
*/
    std::set <String> strings;
    String str;
    size_t count;
    std::cin >> count;
    while (count--) {
        std::cin >> str;
        strings.insert(str);
    }

    for (auto it = strings.rbegin(); it != strings.rend(); ++it) {
        std::cout << *it << '\n';
    }



}