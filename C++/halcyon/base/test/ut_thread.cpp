#include "base/thread/thread.h"

#include <chrono>
#include <iostream>

using std::chrono::seconds;
using namespace halcyon::base;

// 类成员函数
class TestX
{
public:
    void test() const
    {
        std::cout << "TestX::test const" << std::endl;
        std::cout << std::endl;
    }

    void test1()
    {
        std::cout << "TestX::test1: sleep(2s)" << std::endl;
        std::this_thread::sleep_for(seconds(2));
        std::cout << std::endl;
    }
};

// 无参的普通函数
void test2()
{
    std::cout << "test2: add(0 ~ 99999): ";
    int64_t res = 0;
    for (int32_t i = 0; i < 100000; ++i) {
        res += i;
    }
    std::cout << res << std::endl;
    std::cout << std::endl;
}

// 带参数的普通函数
int test3(int32_t i, const std::string& str)
{
    std::cout << "test3(int, string): " << i << " " << str << std::endl;
    std::cout << std::endl;
    return 0;
}

// 可调用对象
struct TestY
{
    int operator()(int) const
    {
        std::cout << "TestY::operator" << std::endl;
        std::cout << std::endl;
        return 0;
    }
};

int main(int argc, char* argv[])
{
    Thread thd;

    TestX x;
    thd.run(&TestX::test, &x);
    thd.run(&TestX::test1, &x);

    thd.run(&test2);

    auto func3 = std::bind(&test3, 10, "hello");
    thd.run(std::move(func3));

    thd.run(&test3, 1000, "world");

    TestY y;
    thd.run(y, 100);

    std::function<void()> ss;
    thd.run(ss);

    auto lambda_func = []() {
        std::cout << "lambda test4" << std::endl;
        std::cout << std::endl;
    };
    thd.run(lambda_func);

    //thd.run(nullptr);   // nullptr_t

    thd.run([]() {
        std::cout << "lambda test5" << std::endl;
        std::this_thread::sleep_for(seconds(1));
        std::cout << std::endl;
    });

    thd.run([](int a, int b) {
        std::cout << "lambda test6: ";
        std::cout << a << "\t" << b << std::endl;
    }, 1, 3);

    thd.join();
    return 0;
}
