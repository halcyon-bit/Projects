#include "base/utility/function_traits.h"

#include <string>
#include <iostream>

using namespace halcyon;

void test1();

void test2(int, double, const std::string&);

std::string test3(double, int);

class TestA
{
public:
    void test4();

    int test5(double) const;

    void test6(int, double);

    double test7(int, double, std::string&) const;

    static const std::string& test8(const std::string&, double);
};

class TestB
{
public:
    double operator()(std::string, int);
};

int main()
{
    // test1
    {
        static_assert(std::is_same_v<base::function_traits<decltype(&test1)>::return_type, void>);
        static_assert(base::function_traits<decltype(&test1)>::arity == 0);
    }
    // test2
    {
        static_assert(std::is_same_v<base::function_traits<decltype(&test2)>::return_type, void>);
        static_assert(base::function_traits<decltype(&test2)>::arity == 3);
        static_assert(std::is_same_v<base::function_traits<decltype(&test2)>::arg_type<0>, int>);
        static_assert(std::is_same_v<base::function_traits<decltype(&test2)>::arg_type<1>, double>);
        static_assert(std::is_same_v<base::function_traits<decltype(&test2)>::arg_type<2>, const std::string&>);
    }
    // test3
    {
        static_assert(std::is_same_v<base::function_traits<decltype(&test3)>::return_type, std::string>);
        static_assert(base::function_traits<decltype(&test3)>::arity == 2);
        static_assert(std::is_same_v<base::function_traits<decltype(&test3)>::arg_type<0>, double>);
        static_assert(std::is_same_v<base::function_traits<decltype(&test3)>::arg_type<1>, int>);
    }
    // TestA::test4
    {
        static_assert(std::is_same_v<base::function_traits<decltype(&TestA::test4)>::return_type, void>);
        static_assert(base::function_traits<decltype(&TestA::test4)>::arity == 0);
    }
    // TestA::test5
    {
        static_assert(std::is_same_v<base::function_traits<decltype(&TestA::test5)>::return_type, int>);
        static_assert(base::function_traits<decltype(&TestA::test5)>::arity == 1);
        static_assert(std::is_same_v<base::function_traits<decltype(&TestA::test5)>::arg_type<0>, double>);
    }
    // TestA::test6
    {
        static_assert(std::is_same_v<base::function_traits<decltype(&TestA::test6)>::return_type, void>);
        static_assert(base::function_traits<decltype(&TestA::test6)>::arity == 2);
        static_assert(std::is_same_v<base::function_traits<decltype(&TestA::test6)>::arg_type<0>, int>);
        static_assert(std::is_same_v<base::function_traits<decltype(&TestA::test6)>::arg_type<1>, double>);
    }
    // TestA::test7
    {
        static_assert(std::is_same_v<base::function_traits<decltype(&TestA::test7)>::return_type, double>);
        static_assert(base::function_traits<decltype(&TestA::test7)>::arity == 3);
        static_assert(std::is_same_v<base::function_traits<decltype(&TestA::test7)>::arg_type<0>, int>);
        static_assert(std::is_same_v<base::function_traits<decltype(&TestA::test7)>::arg_type<1>, double>);
        static_assert(std::is_same_v<base::function_traits<decltype(&TestA::test7)>::arg_type<2>, std::string&>);
    }
    // TestA::test8
    {
        static_assert(std::is_same_v<base::function_traits<decltype(&TestA::test8)>::return_type, const std::string&>);
        static_assert(base::function_traits<decltype(&TestA::test8)>::arity == 2);
        static_assert(std::is_same_v<base::function_traits<decltype(&TestA::test8)>::arg_type<0>, const std::string&>);
        static_assert(std::is_same_v<base::function_traits<decltype(&TestA::test8)>::arg_type<1>, double>);
    }
    // TestB::operator
    {
        static_assert(std::is_same_v<base::function_traits<TestB>::return_type, double>);
        static_assert(base::function_traits<TestB>::arity == 2);
        static_assert(std::is_same_v<base::function_traits<TestB>::arg_type<0>, std::string>);
        static_assert(std::is_same_v<base::function_traits<TestB>::arg_type<1>, int>);
    }
    // std::function
    {
        static_assert(std::is_same_v<base::function_traits<std::function<int(double, int)>>::return_type, int>);
        static_assert(base::function_traits<std::function<int(double, int)>>::arity == 2);
        static_assert(std::is_same_v<base::function_traits<std::function<int(double, int)>>::arg_type<0>, double>);
        static_assert(std::is_same_v<base::function_traits<std::function<int(double, int)>>::arg_type<1>, int>);
    }
    return 0;
}
