#include "base/function/function.h"
#include "gtest/gtest.h"

#include <string>
#include <vector>
#include <iostream>

using namespace halcyon::base;

void printVector(const std::vector<int32_t>& v)
{
    std::cout << "(";
    for (auto n : v) {
        std::cout << n << " ";
    }
    std::cout << ")";
}

void funcWithArg0()
{
    std::cout << __func__ << ": no parameter\n";
}

void funcWithArg1(int32_t n)
{
    std::cout << __func__ << ": " << n << std::endl;
}

void funcWithArg2(int32_t n, bool b)
{
    std::cout << __func__ << ": " << n << ", " << b << std::endl;
}

void funcWithArg3(int32_t n, bool b, double d)
{
    std::cout << __func__ << ": " << n << ", " << b << ", " << d << std::endl;
}

void funcWithArg4(int32_t n, bool b, double d, const std::string& s)
{
    std::cout << __func__ << ": " << n << ", " << b << ", " << d
        << ", " << s << std::endl;
}

void funcWithArg5(int32_t n, bool b, double d, const std::string& s, const std::vector<int32_t>& v)
{
    std::cout << __func__ << ": " << n << ", " << b << ", " << d
        << ", " << s << ", ";
    printVector(v);
    std::cout << std::endl;
}

class TestX
{
public:
    void memfuncWithArg0()
    {
        std::cout << __func__ << ": no parameter\n";
    }

    void memfuncWithArg1(int32_t n)
    {
        std::cout << __func__ << ": " << n << std::endl;
    }

    void memfuncWithArg2(int32_t n, bool b)
    {
        std::cout << __func__ << ": " << n << ", " << b << std::endl;
    }

    void memfuncWithArg3(int32_t n, bool b, double d)
    {
        std::cout << __func__ << ": " << n << ", " << b << ", " << d << std::endl;
    }

    void memfuncWithArg4(int32_t n, bool b, double d, const std::string& s)
    {
        std::cout << __func__ << ": " << n << ", " << b << ", " << d
            << ", " << s << std::endl;
    }

    void memfuncWithArg5(int32_t n, bool b, double d, const std::string& s, const std::vector<int32_t>& v)
    {
        std::cout << __func__ << ": " << n << ", " << b << ", " << d
            << ", " << s << ", ";
        printVector(v);
        std::cout << std::endl;
    }
};

using Function_Arg_0 = Function0<void>;
using Function_Arg_1 = Function1<void, int32_t>;
using Function_Arg_2 = Function2<void, int32_t, bool>;
using Function_Arg_3 = Function3<void, int32_t, bool, double>;
using Function_Arg_4 = Function4<void, int32_t, bool, double, const std::string&>;
using Function_Arg_5 = Function5<void, int32_t, bool, double, const std::string&, const std::vector<int32_t>&>;

TEST(FunctionTest, function)
{
    Function_Arg_0 genfunc0_1 = &funcWithArg0;
    EXPECT_EQ(true, genfunc0_1);
    Function_Arg_0 genfunc0_2 = genfunc0_1;
    EXPECT_EQ(true, genfunc0_2);
    EXPECT_EQ(genfunc0_1, genfunc0_2);

    Function_Arg_0 genfunc0_3 = std::move(genfunc0_1);
    EXPECT_EQ(true, genfunc0_3);
    EXPECT_EQ(false, genfunc0_1);
    EXPECT_EQ(genfunc0_3, genfunc0_2);
}

TEST(FunctionTest, function_call)
{
    // 普通函数
    Function_Arg_0 genfunc0 = &funcWithArg0;
    Function_Arg_1 genfunc1 = &funcWithArg1;
    Function_Arg_2 genfunc2 = &funcWithArg2;
    Function_Arg_3 genfunc3 = &funcWithArg3;
    Function_Arg_4 genfunc4 = &funcWithArg4;
    Function_Arg_5 genfunc5 = &funcWithArg5;

    genfunc0();
    genfunc1(10);
    genfunc2(11, false);
    genfunc3(12, false, 66.66);
    genfunc4(13, false, 67.67, { "hello girls" });
    genfunc5(14, false, 68.68, { "hello boys" }, { 1, 2, 3, 4, 5, 6 });

    std::cout << std::endl;

    // 类的成员函数
    TestX x;
    Function_Arg_0 memfunc0 = Function_Arg_0(&TestX::memfuncWithArg0, &x);
    Function_Arg_1 memfunc1 = Function_Arg_1(&TestX::memfuncWithArg1, &x);
    Function_Arg_2 memfunc2 = Function_Arg_2(&TestX::memfuncWithArg2, &x);
    Function_Arg_3 memfunc3 = Function_Arg_3(&TestX::memfuncWithArg3, &x);
    Function_Arg_4 memfunc4 = Function_Arg_4(&TestX::memfuncWithArg4, &x);
    Function_Arg_5 memfunc5 = Function_Arg_5(&TestX::memfuncWithArg5, &x);

    memfunc0();
    memfunc1(10);
    memfunc2(11, false);
    memfunc3(12, false, 66.66);
    memfunc4(13, false, 67.67, { "hello girls" });
    memfunc5(14, false, 68.68, { "hello boys" }, { 1, 2, 3, 4, 5, 6 });
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
