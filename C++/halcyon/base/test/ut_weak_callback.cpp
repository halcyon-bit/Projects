#include "base/function/weak_callback.h"

#include "gtest/gtest.h"

#include <iostream>

using namespace halcyon::base;

class TestX
{
public:
    void zero() 
    {
        std::cout << "zero()\n\n";
    }
    void zeroc() const
    {
        std::cout << "TestX::zeroc()\n\n";
    }

    void one(int32_t x)
    {
        std::cout << "TestX::one() x = " << x << std::endl << std::endl;
    }
    void oner(int32_t& x)
    {
        std::cout << "TestX::oner() x = " << x << std::endl << std::endl;
        x = 1000;
    }
    void onec(int32_t x) const
    {
        std::cout << "TestX::onec() x = " << x << std::endl << std::endl;
    }
};

TEST(WeakCallBackTest, weakcallback)
{
    std::shared_ptr<TestX> testX(new TestX);
    WeakCallback<TestX> cb0 = makeWeakCallback(testX, &TestX::zero);
    auto cb0c = makeWeakCallback(testX, &TestX::zeroc);
    std::cout << "call zero: ";
    cb0();
    std::cout << "call zeroc: ";
    cb0c();

    WeakCallback<TestX, int32_t> cb1 = makeWeakCallback(testX, &TestX::one);
    auto cb1c = makeWeakCallback(testX, &TestX::onec);
    auto cb1r = makeWeakCallback(testX, &TestX::oner);
    std::cout << "call one: ";
    cb1(100);
    std::cout << "call onec: ";
    cb1c(200);

    int32_t i = 300;
    std::cout << "call oner: ";
    cb1r(i);
    EXPECT_EQ(i, 1000);
    
    WeakCallback<TestX> cb2(testX, std::bind(&TestX::one, std::placeholders::_1, 900));
    std::cout << "call one: ";
    cb2();

    std::cout << "reset ptr\n\n";
    testX.reset();
    std::cout << "call zero: ";
    cb0();
    std::cout << std::endl << std::endl;

    std::cout << "call zeroc: ";
    cb0c();
    std::cout << std::endl << std::endl;
    
    std::cout << "call one: ";
    cb1(120);
    std::cout << std::endl << std::endl;
    
    std::cout << "call onec: ";
    cb1c(119);
    std::cout << std::endl << std::endl;
    
    std::cout << "call one: ";
    cb2();
    std::cout << std::endl << std::endl;
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
