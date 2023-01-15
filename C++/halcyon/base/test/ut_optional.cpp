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

static size_t move_count = 0;
class TestMove
{
public:
    TestMove()
    {
        data_ = new char[100];
    }

    ~TestMove()
    {
        if (valid_) {
            ++move_count;
            delete[] data_;
        }
    }

    TestMove(const TestMove& rhs)
    {
        data_ = new char[100];
    }
    TestMove& operator=(const TestMove& rhs)
    {
        data_ = new char[100];
    }

    TestMove(TestMove&& rhs) noexcept
    {
        data_ = rhs.data_;
        rhs.data_ = nullptr;
        rhs.valid_ = false;
    }
    TestMove& operator=(TestMove&& rhs) noexcept
    {
        if (&rhs != this) {
            data_ = rhs.data_;
            rhs.data_ = nullptr;
            rhs.valid_ = false;
        }
        return *this;
    }

    bool valid_{ true };
    char* data_{ nullptr };
};

static size_t no_move_count = 0;
class TestNoMove
{
public:
    TestNoMove() = default;

    ~TestNoMove()
    {
        ++no_move_count;
    }

    TestNoMove(const TestNoMove& rhs) = default;
    TestNoMove& operator=(const TestNoMove& rhs) = default;
};

TEST(OptionalTest, constructor)
{
    // int
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

        op7 = 100;
        EXPECT_EQ(op7.isInit(), true);
        EXPECT_EQ(*op7, 100);

        op8 = n;
        EXPECT_EQ(op8.isInit(), true);
        EXPECT_EQ(*op8, n);
    }

    // std::string
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

        std::string t = s;
        base::Optional<std::string> op4(std::move(t));
        EXPECT_EQ(op4.isInit(), true);
        EXPECT_EQ(t.empty(), true);
        EXPECT_EQ(*op4, s);

        base::Optional<std::string> op5(op1);
        EXPECT_EQ(op5.isInit(), false);

        base::Optional<std::string> op6(op2);
        EXPECT_EQ(op6.isInit(), true);
        EXPECT_EQ((*op6).compare("world"), 0);

        base::Optional<std::string> op7(std::move(op3));
        EXPECT_EQ(op3.isInit(), false);
        EXPECT_EQ(op7.isInit(), true);
        EXPECT_EQ(*op7, s);

        base::Optional<std::string> op8;
        op8 = op5;
        EXPECT_EQ(op8.isInit(), false);

        base::Optional<std::string> op9;
        op9 = op6;
        EXPECT_EQ(op9.isInit(), true);
        EXPECT_EQ((*op9).compare("world"), 0);

        base::Optional<std::string> op10;
        op10 = std::move(op7);
        EXPECT_EQ(op10.isInit(), true);
        EXPECT_EQ(op7.isInit(), false);
        EXPECT_EQ(*op10, s);

        op9 = "hello";
        EXPECT_EQ(op9.isInit(), true);
        EXPECT_EQ(*op9, s);

        t = s;
        op10 = std::move(t);
        EXPECT_EQ(op10.isInit(), true);
        EXPECT_EQ(t.empty(), true);
        EXPECT_EQ(*op10, s);
    }

    // TestA
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

        op7 = t;
        EXPECT_EQ(op7.isInit(), true);
        EXPECT_EQ(*op7, *op5);
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

TEST(OptionalTest, move_1)
{
    std::string str1{ "hello" };
    std::string str2{ "hello" };
    std::string str3{ "world" };
    base::Optional<std::string> op1{ std::move(str1) };
    EXPECT_EQ(op1.isInit(), true);
    EXPECT_EQ(*op1, str2);
    EXPECT_EQ(str1.empty(), true);

    base::Optional<std::string> op2{ str2 };
    EXPECT_EQ(op2.isInit(), true);
    EXPECT_EQ(*op2, str2);
    EXPECT_EQ(str2.empty(), false);

    base::Optional<std::string> op3(op1);
    EXPECT_EQ(op3.isInit(), true);
    EXPECT_EQ(*op3, *op1);
    EXPECT_EQ(*op3, str2);

    base::Optional<std::string> op4;
    base::Optional<std::string> op5(op4);
    EXPECT_EQ(op5.isInit(), false);

    base::Optional<std::string> op6(std::move(op1));
    EXPECT_EQ(op1.isInit(), false);
    EXPECT_EQ(op6.isInit(), true);
    EXPECT_EQ(*op6, str2);

    base::Optional<std::string> op7(std::move(op4));
    EXPECT_EQ(op4.isInit(), false);
    EXPECT_EQ(op7.isInit(), false);

    base::Optional<std::string> op8{ str3 };
    EXPECT_EQ(*op8, str3);
    op8 = op2;
    EXPECT_EQ(op8.isInit(), true);
    EXPECT_EQ(*op8, str2);
    op8 = op5;
    EXPECT_EQ(op8.isInit(), false);

    base::Optional<std::string> op9{ str3 };
    EXPECT_EQ(*op9, str3);
    op9 = std::move(op2);
    EXPECT_EQ(*op9, str2);
    EXPECT_EQ(op2.isInit(), false);
    op9 = std::move(op8);
    EXPECT_EQ(op9.isInit(), false);
}

TEST(OptionalTest, move_2)
{
    {
        TestNoMove t;
        base::Optional<TestNoMove> op1{ t };
        base::Optional<TestNoMove> op2(std::move(op1));
        EXPECT_EQ(op1.isInit(), false);
        EXPECT_EQ(op2.isInit(), true);
        EXPECT_EQ(no_move_count, 1);

        base::Optional<TestNoMove> op3;
        op3 = std::move(op2);
        EXPECT_EQ(op2.isInit(), false);
        EXPECT_EQ(op3.isInit(), true);
        EXPECT_EQ(no_move_count, 2);
    }
    {
        TestMove t;
        base::Optional<TestMove> op1{ t };
        base::Optional<TestMove> op2(std::move(op1));
        EXPECT_EQ(op1.isInit(), false);
        EXPECT_EQ(op2.isInit(), true);
        EXPECT_EQ(move_count, 0);

        base::Optional<TestMove> op3;
        op3 = std::move(op2);
        EXPECT_EQ(op2.isInit(), false);
        EXPECT_EQ(op3.isInit(), true);
        EXPECT_EQ(move_count, 0);
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
