#define _BASE_TEST_
#define USE_HALCYON_INVOKE_APPLY
#define USE_HALCYON_INDEX_SEQUENCE
#include "base/utility/utility.h"

#include "gtest/gtest.h"

#include <map>
#include <cassert>
#include <iostream>
using namespace halcyon;

class TupleElem
{
public:
    template<typename T>
    void operator()(const T& t)
    {
        std::cout << t << " ";
    }
};

class TupleElemChange
{
public:
    template<typename T>
    void operator()(T& t)
    {
    }

    void operator()(std::string& t)
    {
        t = "change";
    }
};

void test1(int, double, std::string, int)
{
    std::cout << "void test1(int, double, std::string, int)" << std::endl;
}

int test2(int arg1, int arg2)
{
    std::cout << "int test2(int arg1, int arg2)" << std::endl;
    return arg1 + arg2;
}

std::string test3(std::string arg1, std::string arg2)
{
    std::cout << "std::string test3(std::string arg1, std::string arg2)" << std::endl;
    return arg1 + arg2;
}

std::string test4(const std::string& arg1, const std::string& arg2)
{
    std::cout << "std::string test4(const std::string& arg1, const std::string& arg2)" << std::endl;
    return arg1 + arg2;
}

std::string test5(std::string& arg1, const std::string& arg2)
{
    std::cout << "std::string test5(std::string& arg1, const std::string& arg2)" << std::endl;
    arg1 = "hello";
    return arg1 + arg2;
}

class FooA
{
public:
    std::string test6(const std::string& arg1, std::string& arg2)
    {
        std::cout << "std::string FooA::test6(const std::string& arg1, std::string& arg2)" << std::endl;
        arg2 = " world";
        return arg1 + arg2;
    }

    static double test7(double arg1, double arg2)
    {
        std::cout << "static double FooA::test7(double arg1, double arg2)" << std::endl;
        return arg1 + arg2;
    }
};

class FooB
{
public:
    std::string operator()(const std::string& arg1, const std::string& arg2)
    {
        std::cout << "std::string FooB::operator()(const std::string& arg1, const std::string& arg2)" << std::endl;
        return arg1 + arg2;
    }
};

TEST(UtilityTest, type_name_)
{
    std::cout << base::type_name<double>() << std::endl;
    std::cout << base::type_name<std::string>() << std::endl;
    std::cout << base::type_name<std::map<int, int>>() << std::endl;
}

TEST(UtilityTest, invoke_)
{
    int n1{ 1 };
    double db{ 1.1 };
    std::string str{ "hello" };
    int n2{ 2 };
    base::invoke(&test1, n1, db, str, n2);
    base::invoke(&test1, n1, 4.4, str, 12);

    int ret1 = base::invoke(&test2, 10, 44);
    EXPECT_EQ(ret1, 54);
    ret1 = base::invoke(&test2, n1, n2);
    EXPECT_EQ(ret1, 3);

    std::string ret2 = base::invoke(&test3, "hello", " world");
    EXPECT_EQ(ret2, "hello world");

    std::string str1("hello"), str2(" world");
    ret2 = base::invoke(&test3, str1, str2);
    EXPECT_EQ(ret2, "hello world");

    ret2 = base::invoke(&test4, str1, str2);
    EXPECT_EQ(ret2, "hello world");

    str1 = "";
    ret2 = base::invoke(&test5, str1, str2);
    EXPECT_EQ(ret2, "hello world");
    EXPECT_EQ(str1, "hello");

    FooA a;
    str2 = "";
    ret2 = base::invoke(&FooA::test6, &a, str1, str2);
    EXPECT_EQ(str2, " world");
    EXPECT_EQ(ret2, "hello world");

    str2 = "";
    ret2 = base::invoke(&FooA::test6, a, "hello", str2);
    EXPECT_EQ(str2, " world");
    EXPECT_EQ(ret2, "hello world");

    double ret3 = base::invoke(&FooA::test7, 12.12, 13.13);
    EXPECT_EQ(ret3, 25.25);

    FooB b;
    ret2 = base::invoke(b, str1, str2);
    EXPECT_EQ(ret2, "hello world");
}

TEST(UtilityTest, find_tuple_index_)
{
    using Tuple = std::tuple<int, double, std::string, int, int>;
    Tuple tp = std::make_tuple(1, 2.0, "test", 3, 2);
    int32_t pos = base::find_tuple_index(tp, 1);
    EXPECT_EQ(pos, 0);
    pos = base::find_tuple_index(tp, 2);
    EXPECT_EQ(pos, 4);
    pos = base::find_tuple_index(tp, std::string("test"));
    EXPECT_EQ(pos, 2);

    double f{ 2.0 };
    pos = base::find_tuple_index(tp, f);
    EXPECT_EQ(pos, 1);
    pos = base::find_tuple_index(tp, 3);
    EXPECT_EQ(pos, 3);
    pos = base::find_tuple_index(tp, 9);
    EXPECT_EQ(pos, -1);
}

TEST(UtilityTest, tuple_for_each_)
{
    using Tuple = std::tuple<int, double, std::string, int, int>;
    const Tuple tp1 = std::make_tuple(1, 2.2, "hello", 3, 2);
    std::cout << "const tuple tp1: ";
    base::tuple_for_each(TupleElem(), tp1);
    std::cout << std::endl;

    Tuple tp2 = std::make_tuple(10, 20.22, "tuple", 30, 20);
    std::cout << "tuple tp2: ";
    base::tuple_for_each(TupleElem(), tp2);
    std::cout << std::endl;
    std::cout << "change tuple tp2: ";
    base::tuple_for_each(TupleElemChange(), tp2);
    base::tuple_for_each(TupleElem(), tp2);
    std::cout << std::endl;

    std::cout << "tuple: ";
    base::tuple_for_each(TupleElem(), std::make_tuple(11, 22.22, "hello tuple", 33));
    std::cout << std::endl;
    base::tuple_for_each(TupleElem(), std::make_tuple());
}

TEST(UtilityTest, reverse_tuple_)
{
    using Tuple = std::tuple<int, double, std::string, int, int>;
    const Tuple tp1 = std::make_tuple(1, 2.2, "hello", 3, 2);
    std::cout << "origin tuple: ";
    base::tuple_for_each(TupleElem(), tp1);
    std::cout << std::endl;
    auto rtp1 = base::reverse_tuple(tp1);
    std::cout << "reverse result: ";
    base::tuple_for_each(TupleElem(), rtp1);
    std::cout << std::endl << std::endl;

    Tuple tp2 = std::make_tuple(10, 20.22, "tuple", 30, 20);
    std::cout << "origin tuple: ";
    base::tuple_for_each(TupleElem(), tp2);
    std::cout << std::endl;
    auto rtp2 = base::reverse_tuple(tp2);
    std::cout << "reverse result: ";
    base::tuple_for_each(TupleElem(), rtp2);
    std::cout << std::endl << std::endl;

    std::cout << "origin tuple: ";
    base::tuple_for_each(TupleElem(), std::make_tuple(11, 22.22, "hello tuple", 33));
    std::cout << std::endl;
    auto rtp3 = base::reverse_tuple(std::make_tuple(11, 22.22, "hello tuple", 33));
    std::cout << "reverse result: ";
    base::tuple_for_each(TupleElem(), rtp3);
    std::cout << std::endl;

    auto rtp4 = base::reverse_tuple(std::make_tuple());
    base::tuple_for_each(TupleElem(), rtp4);
}

TEST(UtilityTest, apply_)
{
    const auto tp1 = std::make_tuple(1, 2.2, "hello", 3);
    auto tp2 = std::make_tuple(10, 20.22, std::string("tuple"), 30);
    base::apply(&test1, tp1);
    base::apply(&test1, tp2);

    int ret1 = base::apply(&test2, std::make_tuple(10, 44));
    EXPECT_EQ(ret1, 54);
    std::string ret2 = base::apply(&test3, std::make_tuple("hello", " world"));
    EXPECT_EQ(ret2, "hello world");

    std::string str1("hello"), str2(" world");
    ret2 = base::apply(&test3, std::make_tuple(str1, str2));
    EXPECT_EQ(ret2, "hello world");
    ret2 = base::apply(&test4, std::make_tuple<const std::string&, const std::string&>(str1, str2));
    EXPECT_EQ(ret2, "hello world");
    str1 = "";
    std::tuple<std::string&, const std::string&> tp3(str1, str2);
    ret2 = base::apply(&test5, tp3);
    EXPECT_EQ(ret2, "hello world");
    EXPECT_EQ(str1, "hello");

    FooA a;
    str2 = "";
    auto tp4 = std::make_tuple(&a, str1, std::ref(str2));
    ret2 = base::apply(&FooA::test6, tp4);
    EXPECT_EQ(str2, " world");
    EXPECT_EQ(ret2, "hello world");

    double ret3 = base::apply(&FooA::test7, std::make_tuple(12.12, 13.13));
    EXPECT_EQ(ret3, 25.25);

    FooB b;
    ret2 = base::apply(b, std::make_tuple(str1, str2));
    EXPECT_EQ(ret2, "hello world");
}

TEST(UtilityTest, zip_)
{
    {
        std::tuple<int, double> t1{ 1, 6.6 }, t2{ 2, 7.7 };
        auto ret = base::zip(t1, t2);
        EXPECT_EQ(std::get<0>(t1), std::get<0>(ret).first);
        EXPECT_EQ(std::get<0>(t2), std::get<0>(ret).second);
        EXPECT_EQ(std::get<1>(t1), std::get<1>(ret).first);
        EXPECT_EQ(std::get<1>(t2), std::get<1>(ret).second);
    }

    {
        std::tuple<int, std::string> t1{ 1, "hello" };
        std::tuple<std::string, double> t2{ "world", 2.0 };
        auto ret = base::zip(t1, t2);
        EXPECT_EQ(std::get<0>(t1), std::get<0>(ret).first);
        EXPECT_EQ(std::get<0>(t2), std::get<0>(ret).second);
        EXPECT_EQ(std::get<1>(t1), std::get<1>(ret).first);
        EXPECT_EQ(std::get<1>(t2), std::get<1>(ret).second);
    }

    {
        std::tuple<int, std::string> t1{ 1, "hello" };
        std::tuple<std::string, double, int> t2{ "world", 2.0, 3 };
        // auto ret = base::zip(t1, t2);
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
