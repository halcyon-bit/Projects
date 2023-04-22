#define _BASE_TEST_
#include "base/utility/utility.h"

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
    std::cout << "test1" << std::endl;
}

int test2(int arg1, int arg2)
{
    std::cout << "test2" << std::endl;
    return arg1 + arg2;
}

std::string test3(std::string arg1, std::string arg2)
{
    std::cout << "test3" << std::endl;
    return arg1 + arg2;
}

std::string test4(const std::string& arg1, const std::string& arg2)
{
    std::cout << "test4" << std::endl;
    return arg1 + arg2;
}

std::string test5(std::string& arg1, const std::string& arg2)
{
    std::cout << "test5" << std::endl;
    arg1 = "hello";
    return arg1 + arg2;
}

class FooA
{
public:
    std::string test6(const std::string& arg1, std::string& arg2)
    {
        std::cout << "FooA::test6" << std::endl;
        arg2 = " world";
        return arg1 + arg2;
    }

    static double test7(double arg1, double arg2)
    {
        std::cout << "FooA::test7" << std::endl;
        return arg1 + arg2;
    }
};

class FooB
{
public:
    std::string operator()(const std::string& arg1, const std::string& arg2)
    {
        std::cout << "FooB::operator()" << std::endl;
        return arg1 + arg2;
    }
};

int main()
{
    std::cout << "======================= test type_name" << std::endl;
    std::cout << base::type_name<double>() << std::endl;
    std::cout << base::type_name<std::string>() << std::endl;
    std::cout << base::type_name<std::map<int, int>>() << std::endl;


    using Tuple = std::tuple<int, double, std::string, int, int>;
    {
        std::cout << std::endl << std::endl;
        std::cout << "======================= test find_tuple_index" << std::endl;
        Tuple tp = std::make_tuple(1, 2.0, "test", 3, 2);
        std::cout << "find 1 in tuple(1, 2.0, 'test', 3, 2): " << base::find_tuple_index(tp, 1) << std::endl;
        std::cout << "find 2 in tuple(1, 2.0, 'test', 3, 2): " << base::find_tuple_index(tp, 2) << std::endl;
        std::cout << "find 'test' in tuple(1, 2.0, 'test', 3, 2): " << base::find_tuple_index(tp, std::string("test")) << std::endl;
        double f{ 2.0 };
        std::cout << "find 2.0 in tuple(1, 2.0, 'test', 3, 2): " << base::find_tuple_index(tp, f) << std::endl;
        std::cout << "find 3 in tuple(1, 2.0, 'test', 3, 2): " << base::find_tuple_index(tp, 3) << std::endl;
        std::cout << "find 9 in tuple(1, 2.0, 'test', 3, 2): " << base::find_tuple_index(tp, 9) << std::endl;
    }


    {
        std::cout << std::endl << std::endl;
        std::cout << "======================= test tuple_for_each" << std::endl;
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


    {
        std::cout << std::endl << std::endl;
        std::cout << "======================= test reverse_tuple" << std::endl;
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

    
#if defined USE_CPP11 || defined USE_CPP14
    {
        std::cout << std::endl << std::endl;
        std::cout << "======================= test apply" << std::endl;
        const auto tp1 = std::make_tuple(1, 2.2, "hello", 3);
        auto tp2 = std::make_tuple(10, 20.22, std::string("tuple"), 30);
        base::apply(&test1, tp1);
        base::apply(&test1, tp2);
        
        int ret1 = base::apply(&test2, std::make_tuple(10, 44));
        assert(ret1 == 54);
        std::string ret2 = base::apply(&test3, std::make_tuple("hello", " world"));
        assert(ret2 == "hello world");
        
        std::string str1("hello"), str2(" world");
        ret2 = base::apply(&test3, std::make_tuple(str1, str2));
        assert(ret2 == "hello world");
        ret2 = base::apply(&test4, std::make_tuple<const std::string&, const std::string&>(str1, str2));
        assert(ret2 == "hello world");
        str1 = "";
        std::tuple<std::string&, const std::string&> tp3(str1, str2);
        ret2 = base::apply(&test5, tp3);
        assert(ret2 == "hello world");
        assert(str1 == "hello");

        FooA a;
        str2 = "";
        auto tp4 = std::make_tuple(&a, str1, std::ref(str2));
        ret2 = base::apply(&FooA::test6, tp4);
        assert(str2 == " world");
        assert(ret2 == "hello world");

        double ret3 = base::apply(&FooA::test7, std::make_tuple(12.12, 13.13));
        assert(ret3 == 25.25);

        FooB b;
        ret2 = base::apply(b, std::make_tuple(str1, str2));
        assert(ret2 == "hello world");
    }
#endif
    return 0;
}
