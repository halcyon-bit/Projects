#include "base/utility/lazy.h"

#include "gtest/gtest.h"

#include <string>
#include <iostream>
#include <memory>
using namespace halcyon;

struct TestA
{
    int n;
    double f;
    std::string s;

    TestA(int n, double f, const std::string& s)
        : n(n), f(f), s(s)
    {
        std::cout << "construct" << std::endl;
    }

    bool operator==(const TestA& rhs) const
    {
        return n == rhs.n && f == rhs.f && s == rhs.s;
    }
};

int func(int n) 
{
    return n * n;
}

TEST(LazyTest, lazy_test)
{
    TestA t{ 100, 1.23, "hello" };
    auto lazy1 = base::lazy(func, 10);
    EXPECT_EQ(lazy1.hasValue(), false);
    EXPECT_EQ(lazy1.value(), 100);

    auto lazy2 = base::lazy([]() { return std::string("hello world"); });
    EXPECT_EQ(lazy2.hasValue(), false);
    EXPECT_EQ(lazy2.value(), std::string("hello world"));

    std::cout << "create lazy value" << std::endl;
    auto lazy3 = base::lazy([](int n, double f, const std::string& s) {return std::make_shared<TestA>(n, f, s); },
        100, 1.23, "hello");
    std::cout << "load value" << std::endl;
    EXPECT_EQ(lazy3.hasValue(), false);
    lazy3.value();
    EXPECT_EQ(lazy3.hasValue(), true);
    EXPECT_EQ(*lazy3.value(), t);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
