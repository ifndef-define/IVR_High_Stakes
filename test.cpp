#include <iostream>
#include <assert.h>

int main() {

    std::cout << "Hello, World!" << std::endl;
    int a=4, b=5;
    if (a < b)
        throw std::domain_error("a is less than b");
    std::cout << "Hello, World!2" << std::endl;

    return 0;
}