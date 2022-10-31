#include "base/optional/optional.h"

#include "gtest/gtest.h"

#include <string>

using namespace halcyon;

struct TestA
{
    int n;
    double f;
    std::string s;

    TestA(int n, double f, const std::string& s)
        : n(n), f(f), s(s)
    {}

    bool operator==(const TestA& rhs) const
    {
        return n == rhs.n && f == rhs.f && s == rhs.s;
    }
};

TEST(OptionalTest, constructor)
{
    {
        base::Optional<int> op1;
        EXPECT_EQ(op1.isInit(), false);

        base::Optional<int> op2(1);
        EXPECT_EQ(op2.isInit(), true);
        EXPECT_EQ(*op2, 1);

        int n = 10;
        base::Optional<int> op3(n);
        EXPECT_EQ(op3.isInit(), true);
        EXPECT_EQ(*op3, n);

        base::Optional<int> op4(op1);
        EXPECT_EQ(op4.isInit(), false);

        base::Optional<int> op5(op2);
        EXPECT_EQ(op5.isInit(), true);
        EXPECT_EQ(*op5, 1);

        base::Optional<int> op6(std::move(op3));
        EXPECT_EQ(op3.isInit(), false);
        EXPECT_EQ(op6.isInit(), true);
        EXPECT_EQ(*op6, n);

        base::Optional<int> op7;
        op7 = op4;
        EXPECT_EQ(op7.isInit(), false);

        base::Optional<int> op8;
        op8 = op5;
        EXPECT_EQ(op8.isInit(), true);
        EXPECT_EQ(*op8, 1);

        base::Optional<int> op9;
        op9 = std::move(op6);
        EXPECT_EQ(op9.isInit(), true);
        EXPECT_EQ(op6.isInit(), false);
        EXPECT_EQ(*op9, n);
    }

    {
        base::Optional<std::string> op1;
        EXPECT_EQ(op1.isInit(), false);

        base::Optional<std::string> op2("world");
        EXPECT_EQ(op2.isInit(), true);
        EXPECT_EQ((*op2).compare("world"), 0);

        std::string s = "hello";
        base::Optional<std::string> op3(s);
        EXPECT_EQ(op3.isInit(), true);
        EXPECT_EQ(*op3, s);

        base::Optional<std::string> op4(op1);
        EXPECT_EQ(op4.isInit(), false);

        base::Optional<std::string> op5(op2);
        EXPECT_EQ(op5.isInit(), true);
        EXPECT_EQ((*op5).compare("world"), 0);

        base::Optional<std::string> op6(std::move(op3));
        EXPECT_EQ(op3.isInit(), false);
        EXPECT_EQ(op6.isInit(), true);
        EXPECT_EQ(*op6, s);

        base::Optional<std::string> op7;
        op7 = op4;
        EXPECT_EQ(op7.isInit(), false);

        base::Optional<std::string> op8;
        op8 = op5;
        EXPECT_EQ(op8.isInit(), true);
        EXPECT_EQ((*op8).compare("world"), 0);

        base::Optional<std::string> op9;
        op9 = std::move(op6);
        EXPECT_EQ(op9.isInit(), true);
        EXPECT_EQ(op6.isInit(), false);
        EXPECT_EQ(*op9, s);

        base::Optional<std::string> op10(std::move(s));
        EXPECT_EQ(op10.isInit(), true);
        EXPECT_EQ(s.empty(), true);
        EXPECT_EQ(*op10, *op9);
    }

    {
        base::Optional<TestA> op1;
        EXPECT_EQ(op1.isInit(), false);

        TestA t{ 10, 0.123, std::string("world") };
        base::Optional<TestA> op2(t);
        EXPECT_EQ(op2.isInit(), true);
        EXPECT_EQ(*op2, t);

        base::Optional<TestA> op4(op1);
        EXPECT_EQ(op4.isInit(), false);

        base::Optional<TestA> op5(op2);
        EXPECT_EQ(op5.isInit(), true);
        EXPECT_EQ(*op5, t);

        base::Optional<TestA> op6(std::move(op2));
        EXPECT_EQ(op2.isInit(), false);
        EXPECT_EQ(op6.isInit(), true);
        EXPECT_EQ(*op6, *op5);

        base::Optional<TestA> op7;
        op7 = op4;
        EXPECT_EQ(op7.isInit(), false);

        base::Optional<TestA> op8;
        op8 = op5;
        EXPECT_EQ(op8.isInit(), true);
        EXPECT_EQ(*op8, *op5);

        base::Optional<TestA> op9;
        op9 = std::move(op6);
        EXPECT_EQ(op9.isInit(), true);
        EXPECT_EQ(op6.isInit(), false);
        EXPECT_EQ(*op9, *op5);
    }

    {
        base::Optional<std::string> op1;
        op1.emplace("hello");
        EXPECT_EQ((*op1).compare("hello"), 0);
        op1.emplace("world");
        EXPECT_EQ((*op1).compare("world"), 0);

        base::Optional<TestA> op2;
        TestA t{ 10, 1.123, "hello" };
        op2.emplace(10, 1.123, std::string("hello"));
        EXPECT_EQ(*op2, t);

        t.s = "world";
        op2.emplace(10, 1.123, "world");
        EXPECT_EQ(*op2, t);
    }
}

TEST(OptionalTest, operator_)
{
    {
        base::Optional<int> op1(1);
        EXPECT_EQ(*op1, 1);
        EXPECT_EQ(op1.operator bool(), true);
        EXPECT_TRUE(op1 == true);

        std::string s("world");
        base::Optional<std::string> op2("world");
        EXPECT_EQ(*op2, s);
        EXPECT_EQ(op2->size(), s.size());
        EXPECT_EQ(op2.operator bool(), true);
        EXPECT_TRUE(op2 == true);

        TestA t{ 10, 0.123, std::string("world") };
        base::Optional<TestA> op3(t);
        EXPECT_EQ(*op3, t);
        EXPECT_EQ(op3->s, t.s);
        EXPECT_EQ(op3.operator bool(), true);
        EXPECT_TRUE(op3 == true);

        base::Optional<TestA> op4;
        EXPECT_EQ(op4.operator bool(), false);
        EXPECT_TRUE(op4 == false);
    }

    {
        const base::Optional<int> op1(1);
        EXPECT_EQ(*op1, 1);
        EXPECT_EQ(op1.operator bool(), true);
        EXPECT_TRUE(op1 == true);

        std::string s("world");
        const base::Optional<std::string> op2("world");
        EXPECT_EQ(*op2, s);
        EXPECT_EQ(op2->size(), s.size());
        EXPECT_EQ(op2.operator bool(), true);
        EXPECT_TRUE(op2 == true);

        TestA t{ 10, 0.123, std::string("world") };
        const base::Optional<TestA> op3(t);
        EXPECT_EQ(*op3, t);
        EXPECT_EQ(op3->s, t.s);
        EXPECT_EQ(op3.operator bool(), true);
        EXPECT_TRUE(op3 == true);

        const base::Optional<TestA> op4;
        EXPECT_EQ(op4.operator bool(), false);
        EXPECT_TRUE(op4 == false);
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
