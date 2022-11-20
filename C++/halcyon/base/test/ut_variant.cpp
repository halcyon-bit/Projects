#include "base/variant/variant.h"

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

    TestMove(TestMove&& rhs)
    {
        data_ = rhs.data_;
        rhs.data_ = nullptr;
        rhs.valid_ = false;
    }
    TestMove& operator=(TestMove&& rhs)
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

TEST(VariantTest, constructor)
{
    {
        base::Variant<int, std::string> var1;
        EXPECT_EQ(var1.is<void>(), true);

        base::Variant<int, std::string> var2(std::string("123"));
        EXPECT_EQ(var2.is<std::string>(), true);
        EXPECT_EQ(var2.get<std::string>(), std::string("123"));

        base::Variant<int, std::string> var3(123);
        EXPECT_EQ(var3.is<int>(), true);
        EXPECT_EQ(var3.get<int>(), 123);

        base::Variant<int, std::string> var4(var2);
        EXPECT_EQ(var4.is<std::string>(), true);
        EXPECT_EQ(var4.get<std::string>(), std::string("123"));

        base::Variant<int, std::string> var5(std::move(var4));
        EXPECT_EQ(var5.is<std::string>(), true);
        EXPECT_EQ(var5.get<std::string>(), std::string("123"));
        EXPECT_EQ(var4.is<void>(), true);

        base::Variant<int, std::string> var6(var3);
        EXPECT_EQ(var6.is<int>(), true);
        EXPECT_EQ(var6.get<int>(), 123);

        base::Variant<int, std::string> var7(std::move(var6));
        EXPECT_EQ(var7.is<int>(), true);
        EXPECT_EQ(var7.get<int>(), 123);
        EXPECT_EQ(var6.is<void>(), true);
    }
    {
        TestA a{ 10, 10.10, "hello" };
        base::Variant<TestA, std::string> var1;
        EXPECT_EQ(var1.is<void>(), true);

        base::Variant<TestA, std::string> var2(std::string("123"));
        EXPECT_EQ(var2.is<std::string>(), true);
        EXPECT_EQ(var2.get<std::string>(), std::string("123"));

        base::Variant<TestA, std::string> var3(a);
        EXPECT_EQ(var3.is<TestA>(), true);
        EXPECT_EQ(var3.get<TestA>().n, 10);
        EXPECT_EQ(var3.get<TestA>().f, 10.10);
        EXPECT_EQ(var3.get<TestA>().s, std::string("hello"));

        base::Variant<TestA, std::string> var6(var3);
        EXPECT_EQ(var6.is<TestA>(), true);
        EXPECT_EQ(var6.get<TestA>().n, 10);
        EXPECT_EQ(var6.get<TestA>().f, 10.10);
        EXPECT_EQ(var6.get<TestA>().s, std::string("hello"));

        base::Variant<TestA, std::string> var7(std::move(var6));
        EXPECT_EQ(var7.is<TestA>(), true);
        EXPECT_EQ(var7.get<TestA>().n, 10);
        EXPECT_EQ(var7.get<TestA>().f, 10.10);
        EXPECT_EQ(var7.get<TestA>().s, std::string("hello"));
        EXPECT_EQ(var6.is<void>(), true);
    }
}

TEST(VariantTest, operator_)
{
    {
        base::Variant<int, std::string> var1;
        base::Variant<int, std::string> var2(123);
        base::Variant<int, std::string> var3(std::string("123"));

        base::Variant<int, std::string> var4;
        var4 = 456;
        EXPECT_EQ(var4.is<int>(), true);
        EXPECT_EQ(var4.get<int>(), 456);

        var4 = std::string("456");
        EXPECT_EQ(var4.is<std::string>(), true);
        EXPECT_EQ(var4.get<std::string>(), std::string("456"));

        var4 = var1;
        EXPECT_EQ(var4.is<void>(), true);

        var4 = var2;
        EXPECT_EQ(var4.is<int>(), true);
        EXPECT_EQ(var4.get<int>(), 123);

        var4 = var3;
        EXPECT_EQ(var4.is<std::string>(), true);
        EXPECT_EQ(var4.get<std::string>(), std::string("123"));

        base::Variant<int, std::string> var5(std::string("hello"));
        var5 = std::move(var1);
        EXPECT_EQ(var5.is<void>(), true);

        var5 = std::move(var2);
        EXPECT_EQ(var5.is<int>(), true);
        EXPECT_EQ(var5.get<int>(), 123);
        EXPECT_EQ(var2.is<void>(), true);

        var5 = std::move(var3);
        EXPECT_EQ(var5.is<std::string>(), true);
        EXPECT_EQ(var5.get<std::string>(), std::string("123"));
        EXPECT_EQ(var3.is<void>(), true);
    }
    {
        TestA a{ 10, 10.10, "hello" };
        base::Variant<TestA, std::string> var1;
        base::Variant<TestA, std::string> var2(a);
        base::Variant<TestA, std::string> var3(std::string("123"));

        base::Variant<TestA, std::string> var4;
        var4 = std::string("world");
        EXPECT_EQ(var4.is<std::string>(), true);
        EXPECT_EQ(var4.get<std::string>(), std::string("world"));

        var4 = a;
        EXPECT_EQ(var4.is<TestA>(), true);
        EXPECT_EQ(var4.get<TestA>(), a);

        var4 = var1;
        EXPECT_EQ(var4.is<void>(), true);

        var4 = var2;
        EXPECT_EQ(var4.is<TestA>(), true);
        EXPECT_EQ(var4.get<TestA>(), a);

        var4 = var3;
        EXPECT_EQ(var4.is<std::string>(), true);
        EXPECT_EQ(var4.get<std::string>(), std::string("123"));

        base::Variant<TestA, std::string> var5(std::string("hello"));
        var5 = std::move(var1);
        EXPECT_EQ(var5.is<void>(), true);

        var5 = std::move(var2);
        EXPECT_EQ(var5.is<TestA>(), true);
        EXPECT_EQ(var5.get<TestA>(), a);
        EXPECT_EQ(var2.is<void>(), true);

        var5 = std::move(var3);
        EXPECT_EQ(var5.is<std::string>(), true);
        EXPECT_EQ(var5.get<std::string>(), std::string("123"));
        EXPECT_EQ(var3.is<void>(), true);
    }
}

TEST(VariantTest, move)
{
    {
        TestNoMove t;
        base::Variant<TestNoMove> var1{ t };
        base::Variant<TestNoMove> var2(std::move(var1));
        EXPECT_EQ(var1.is<void>(), true);
        EXPECT_EQ(var2.is<TestNoMove>(), true);
        EXPECT_EQ(no_move_count, 1);

        base::Variant<TestNoMove> var3;
        var3 = std::move(var2);
        EXPECT_EQ(var2.is<void>(), true);
        EXPECT_EQ(var3.is<TestNoMove>(), true);
        EXPECT_EQ(no_move_count, 2);
    }
    {
        TestMove t;
        base::Variant<TestMove> var1{ t };
        base::Variant<TestMove> var2(std::move(var1));
        EXPECT_EQ(var1.is<void>(), true);
        EXPECT_EQ(var2.is<TestMove>(), true);
        EXPECT_EQ(move_count, 0);

        base::Variant<TestMove> var3;
        var3 = std::move(var2);
        EXPECT_EQ(var2.is<void>(), true);
        EXPECT_EQ(var3.is<TestMove>(), true);
        EXPECT_EQ(move_count, 0);
    }
}

TEST(VariantTest, type)
{
    base::Variant<int, double, std::string> v = 13;
    EXPECT_EQ(v.indexOf<std::string>(), 2);
    EXPECT_EQ(v.indexOf<double>(), 1);
    EXPECT_EQ(v.indexOf<int>(), 0);

    EXPECT_TRUE((base::detail::index_of<int, int, double, std::string>::value == 0));
    EXPECT_TRUE((base::detail::index_of<double, int, double, std::string>::value == 1));
    EXPECT_TRUE((base::detail::index_of<float, int, double, std::string>::value == -1));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
