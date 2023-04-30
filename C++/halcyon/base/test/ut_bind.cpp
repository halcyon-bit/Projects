#define USE_HALCYON_INVOKE_APPLY
#include "base/utility/bind.h"

#include "gtest/gtest.h"

#include <string>
#include <iostream>
#include <functional>
using namespace halcyon;

void test1()
{
    std::cout << "test1" << std::endl;
}

int test2(int& arg)
{
    arg = 100 + arg;
    std::cout << "test2" << std::endl;
    return arg;
}

class TestA
{
public:
    void test3(const std::string& arg1, std::string& arg2)
    {
        std::cout << "test3" << std::endl;
        arg2 = "world";
    }

    static int test4(int arg)
    {
        std::cout << "test4" << std::endl;
        return arg;
    }
};

class TestB
{
public:
    std::string operator()(const std::string& arg1, const std::string& arg2)
    {
        std::cout << "operator()" << std::endl;
        return arg1 + " " + arg2;
    }
};

TEST(Bind, test)
{
    {
        auto func1 = base::bind(&test1);
        func1();
    }

    {
        auto func2 = base::bind(&test2, base::_1);
        int a = 10;
        int ret = func2(a);
        EXPECT_EQ(a, ret);

        auto func3 = base::bind(&test2, std::ref(a));
        ret = func3();
        EXPECT_EQ(a, ret);
    }

    std::string str1("hello"), str2;
    {
        TestA ta;
        auto func4 = base::bind(&TestA::test3, &ta, str1, std::ref(str2));
        func4();
        EXPECT_EQ(str2, "world");

        str2 = "";
        auto func5 = base::bind(&TestA::test3, ta, str1, std::ref(str2));
        func5();
        EXPECT_EQ(str2, "world");
    }

    {
        auto func6 = base::bind(&TestA::test4, 10);
        int32_t ret = func6();
        EXPECT_EQ(ret, 10);
    }

    {
        TestB tb;
        auto func7 = base::bind(tb, str1, str2);
        std::string ret = func7();
        EXPECT_EQ(ret, "hello world");
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
