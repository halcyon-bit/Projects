#include "base/range/range.h"

#include <iostream>
using namespace halcyon::base;

void rangeTest1()
{
    std::cout << "range(10): ";
    for (int32_t i : range(10)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void rangeTest2()
{
    std::cout << "range(3, 9): ";
    for (int32_t i : range(3, 9)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void rangeTest3()
{
    std::cout << "range(1, 10, 3): ";
    for (int32_t i : range(1, 10, 3)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void rangeTest4()
{
    std::cout << "range(-2, -20, -3): ";
    for (int32_t i : range(-2, -20, -3)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void rangeTest5()
{
    std::cout << "range(90, 78, -1): ";
    for (int32_t i : range(90, 78, -1)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void rangeTest6()
{
    std::cout << "range(4.5, 16.5): ";
    for (auto i : range(4.5, 16.5)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void rangeTest7()
{
    std::cout << "range(12, 18, 0.5): ";
    for (auto i : range(12, 18, 0.5)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void rangeTest8()
{
    std::cout << "range(10, 8, -0.1): ";
    for (double i : range(10, 8, -0.1)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void rangeTest9()
{
    std::cout << "range('a', 'z'): ";
    for (char i : range('a', 'z')) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void rangeTest10()
{
    std::cout << "range(1, 10, 2): ";
    auto r = RangeImpl<int32_t>(1, 10, 2);
    for (auto iter = r.begin(); iter != r.end(); ++iter) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;
}

void rangeTest11()
{
    std::cout << "range(0, 10, 2): ";
    auto r = RangeImpl<int32_t>(0, 10, 2);
    for (auto iter = r.end(); iter != r.begin(); --iter) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    rangeTest1();
    rangeTest2();
    rangeTest3();
    rangeTest4();
    rangeTest5();
    rangeTest6();
    rangeTest7();
    rangeTest8();
    rangeTest9();
    rangeTest10();
    rangeTest11();
    return 0;
}
