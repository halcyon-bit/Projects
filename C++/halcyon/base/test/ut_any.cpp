#include "base/any/any.h"

#include "gtest/gtest.h"

#include <map>
#include <string>
#include <vector>

using namespace halcyon;

TEST(AnyTest, constructor)
{
    base::Any any1;
    EXPECT_EQ(true, any1.isNull());

    // string
    std::string s{ "string" };
    base::Any any2 = s;
    EXPECT_EQ(true, any2.is<std::string>());
    EXPECT_EQ(s, any2.anyCast<std::string>());

    // integer
    base::Any any3 = 1;
    EXPECT_EQ(true, any3.is<int32_t>());
    EXPECT_EQ(1, any3.anyCast<int32_t>());

    // double
    base::Any any4 = (double)10.0;
    EXPECT_EQ(true, any4.is<double>());
    EXPECT_EQ(10.0, any4.anyCast<double>());

    // chars
    char ch[] = "chars";
    base::Any any5 = ch;
    EXPECT_EQ(true, any5.is<char*>());
    EXPECT_EQ(ch, any5.anyCast<char*>());

    // std::vector
    std::vector<int32_t> vec{ 1, 2, 3, 4, 5 };
    base::Any any6 = vec;
    EXPECT_EQ(true, any6.is<std::vector<int32_t>>());
    EXPECT_EQ(vec, any6.anyCast<std::vector<int32_t>>());

    // std::map
    std::map<int32_t, std::string> m{ {1, "a"}, {2, "b"}, {3, "c"} };
    base::Any any7 = m;
    bool b = any7.is<std::map<int32_t, std::string>>();
    EXPECT_EQ(true, b);
    auto res = any7.anyCast<std::map<int32_t, std::string>>();
    EXPECT_EQ(m, res);

    // copy
    base::Any any8 = any6;
    EXPECT_EQ(true, any8.is<std::vector<int32_t>>());
    EXPECT_EQ(vec, any8.anyCast<std::vector<int32_t>>());
    EXPECT_EQ(true, any6.is<std::vector<int32_t>>());
    EXPECT_EQ(vec, any6.anyCast<std::vector<int32_t>>());

    // move
    base::Any any9 = std::move(any8);
    EXPECT_EQ(true, any9.is<std::vector<int32_t>>());
    EXPECT_EQ(vec, any9.anyCast<std::vector<int32_t>>());
    EXPECT_EQ(true, any8.is<void>());
    EXPECT_EQ(true, any8.isNull());
}

TEST(AnyTest, assignment)
{
    // string
    std::string s{ "string" };
    base::Any any1 = s;
    EXPECT_EQ(true, any1.is<std::string>());
    EXPECT_EQ(s, any1.anyCast<std::string>());

    // std::map
    std::map<int32_t, std::string> m{ {1, "a"}, {2, "b"}, {3, "c"} };
    base::Any any2 = m;
    bool b = any2.is<std::map<int32_t, std::string>>();
    EXPECT_EQ(true, b);
    auto res = any2.anyCast<std::map<int32_t, std::string>>();
    EXPECT_EQ(m, res);

    base::Any any3 = (int)100;
    EXPECT_EQ(true, any3.is<int>());
    EXPECT_EQ(100, any3.anyCast<int>());

    any3 = any1;
    EXPECT_EQ(true, any3.is<std::string>());
    EXPECT_EQ(s, any3.anyCast<std::string>());
    EXPECT_EQ(true, any1.is<std::string>());
    EXPECT_EQ(s, any1.anyCast<std::string>());

    any1 = std::move(any2);
    b = any1.is<std::map<int32_t, std::string>>();
    EXPECT_EQ(true, b);
    res = any1.anyCast<std::map<int32_t, std::string>>();
    EXPECT_EQ(m, res);
    EXPECT_EQ(true, any2.is<void>());
    EXPECT_EQ(true, any2.isNull());

    int n = 100;
    any3 = n;
    EXPECT_EQ(true, any3.is<int>());
    EXPECT_EQ(100, any3.anyCast<int>());

    any3 = m;
    b = any3.is<std::map<int32_t, std::string>>();
    EXPECT_EQ(true, b);
    res = any3.anyCast<std::map<int32_t, std::string>>();
    EXPECT_EQ(m, res);
}

TEST(AnyTest, memberFunc)
{
    // void
    base::Any any1;
    EXPECT_EQ(true, any1.isNull());
    EXPECT_EQ(true, any1.is<void>());

    // chars
    char ch[] = "chars";
    base::Any any2 = ch;
    EXPECT_EQ(false, any2.isNull());
    EXPECT_EQ(true, any2.is<char*>());
    EXPECT_EQ(ch, any2.anyCast<char*>());

    // std::vector
    std::vector<int32_t> vec{ 1, 2, 3, 4, 5 };
    base::Any any3 = vec;
    EXPECT_EQ(false, any3.isNull());
    EXPECT_EQ(true, any3.is<std::vector<int32_t>>());
    EXPECT_EQ(vec, any3.anyCast<std::vector<int32_t>>());

    // std::map
    std::map<int32_t, std::string> m{ {1, "a"}, {2, "b"}, {3, "c"} };
    base::Any any4 = m;
    EXPECT_EQ(false, any4.isNull());
    bool b = any4.is<std::map<int32_t, std::string>>();
    EXPECT_EQ(true, b);
    auto res = any4.anyCast<std::map<int32_t, std::string>>();
    EXPECT_EQ(m, res);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
