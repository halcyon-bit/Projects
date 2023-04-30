#include "base/function/defer.h"

#include <iostream>

void test1(const char* name)
{
    std::cout << "call " << __func__ << ": " << name << std::endl;
}

void test2()
{
    std::cout << "call " << __func__ << std::endl;
    defer([]() {
        std::cout << "test2 defer lambda first\n"; });

    defer([]() {
        std::cout << "test2 defer lambda second\n"; });

    defer([]() {
        std::cout << "test2 defer lambda third\n"; });
}

int main(int argc, char* argv[])
{
    defer([]() {
        std::cout << "main defer lambda first\n"; });

    defer([]() {
        std::cout << "main defer lambda second\n"; });

    defer([]() {
        std::cout << "main defer lambda third\n"; });

    defer([]() {
        test1("hello boy"); });

    {
        defer([]() { std::cout << "defer a\n"; });
        defer([]() { std::cout << "defer b\n"; });
        defer([]() { std::cout << "defer c\n"; });
        std::cout << "in code scope\n";
    }

    std::cout << "main done\n";
    test2();
    return 0;
}
