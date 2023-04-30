#include "base/string/string_view.h"

#include "gtest/gtest.h"

using namespace halcyon::base;

TEST(StringViewTest, constructor)
{
    string_view sv1;

    std::string str1{ "123456" };
    string_view sv2("123456");

    std::string str2{ "qwert" };
    string_view sv3(str2);
    string_view sv4(sv3);
    string_view sv5(str2.data(), str2.size());
    string_view sv6;
    sv6 = sv4;
    EXPECT_EQ(str1.compare(sv2.data()), 0);
    EXPECT_EQ(str2.compare(sv3.data()), 0);
    EXPECT_EQ(str2.compare(sv4.data()), 0);
    EXPECT_EQ(str2.compare(sv5.data()), 0);
    EXPECT_EQ(str2.compare(sv6.data()), 0);
}

TEST(StringViewTest, iterator)
{
    std::string str1("123456");
    string_view sv1("123456");
    EXPECT_EQ(str1.compare(sv1.begin()), 0);
    EXPECT_EQ(str1.compare(sv1.cbegin()), 0);
    std::string str2(sv1.begin(), sv1.end());
    EXPECT_EQ(str1, str2);
    std::string str3(sv1.cbegin(), sv1.cend());
    EXPECT_EQ(str1, str3);

    std::string str5("654321");
    std::string str6(sv1.rbegin(), sv1.rend());
    EXPECT_EQ(str5, str6);
    std::string str7(sv1.crbegin(), sv1.crend());
    EXPECT_EQ(str5, str7);

    string_view sv2;
    EXPECT_EQ(sv2.begin(), nullptr);
    EXPECT_EQ(sv2.end(), nullptr);
    EXPECT_EQ(sv2.cbegin(), nullptr);
    EXPECT_EQ(sv2.cend(), nullptr);
}

TEST(StringViewTest, capacity)
{
    string_view sv1;
    EXPECT_EQ(sv1.size(), 0);
    EXPECT_EQ(sv1.length(), 0);
    EXPECT_EQ(sv1.max_size(), 0);
    EXPECT_EQ(sv1.empty(), true);

    std::string str{ "123456" };
    sv1 = str;
    EXPECT_EQ(sv1.size(), str.size());
    EXPECT_EQ(sv1.length(), str.length());
    EXPECT_EQ(sv1.max_size(), str.size());
    EXPECT_EQ(sv1.empty(), false);
}

TEST(StringViewTest, modify)
{
    std::string str{ "123456" };
    string_view sv1(str);

    for (size_t i = 0; i < str.size(); ++i) {
        EXPECT_EQ(str[i], sv1[i]);
        EXPECT_EQ(str.at(i), sv1.at(i));
    }

    EXPECT_EQ(str.front(), sv1.front());
    EXPECT_EQ(str.back(), sv1.back());
    EXPECT_EQ(str.compare(sv1.data()), 0);
}

TEST(StringViewTest, access)
{
    std::string str1{ "123456" };
    std::string str2{ "123" };
    std::string str3{ "456" };
    string_view sv1(str1);
    string_view sv2(str1);

    sv1.remove_prefix(3);
    EXPECT_EQ(str3.compare(0, str3.size(), sv1.begin(), sv1.size()), 0);

    sv2.remove_suffix(3);
    EXPECT_EQ(str2.compare(0, str2.size(), sv2.begin(), sv2.size()), 0);

    sv1.swap(sv2);
    EXPECT_EQ(str2.compare(0, str2.size(), sv1.begin(), sv1.size()), 0);
    EXPECT_EQ(str3.compare(0, str3.size(), sv2.begin(), sv2.size()), 0);
}

TEST(StringViewTest, conversion)
{
    std::string str1("123456");
    string_view sv("123456");

    std::string str2 = sv.to_string();
    std::string str3 = std::string(sv);
    EXPECT_EQ(str2, str1);
    EXPECT_EQ(str3, str1);
}

TEST(StringViewTest, operation1)
{
    char v[16]{ 0 };
    std::string str1("123456");
    string_view sv("123456");

    str1.copy(v, 16, 0);
    EXPECT_EQ(str1.compare(v), 0);

    memset(v, 0, 16);
    str1.copy(v, 16, 3);
    EXPECT_EQ(str1.compare(3, str1.size(), v), 0);

    char v1[4]{ 0 };
    str1.copy(v1, 3, 0);
    std::string str2 = str1.substr(0, 3);
    EXPECT_EQ(str2.compare(v1), 0);

    string_view sv1 = sv.substr(0, 3);
    EXPECT_EQ(str2.compare(0, str2.size(), sv1.begin(), sv1.size()), 0);

    string_view sv2 = sv.substr(3);
    EXPECT_EQ(str1.compare(3, str2.size(), sv2.begin(), sv2.size()), 0);

    string_view sv3 = sv.substr(0);
    EXPECT_EQ(str1.compare(sv3.data()), 0);
}

TEST(StringViewTest, compare)
{
    string_view sv1("123456");
    string_view sv2("123456");
    string_view sv3("123");
    string_view sv4("456");

    // compare(basic_string_view)
    EXPECT_EQ(sv1.compare(string_view()), 1);
    EXPECT_EQ(sv1.compare(sv2), 0);
    EXPECT_EQ(sv1.compare(sv3), 1);
    EXPECT_LT(sv1.compare(sv4), 0);

    // compare(size_type, size_type, basic_string_view)
    EXPECT_EQ(sv1.compare(0, 3, sv3), 0);
    EXPECT_GT(sv1.compare(3, 3, sv3), 0);
    EXPECT_LT(sv1.compare(0, 3, sv4), 0);
    EXPECT_EQ(sv1.compare(3, 3, sv4), 0);

    // compare(size_type, size_type, basic_string_view, size_type, size_type)
    EXPECT_EQ(sv1.compare(0, 3, sv2, 0, 3), 0);
    EXPECT_LT(sv1.compare(0, 3, sv2, 3, 3), 0);
    EXPECT_EQ(sv1.compare(3, 3, sv2, 3, 3), 0);
    EXPECT_GT(sv1.compare(3, 3, sv2, 0, 3), 0);

    // compare(const value_type*)
    EXPECT_EQ(sv1.compare(""), 1);
    EXPECT_EQ(sv1.compare(sv2.data()), 0);
    EXPECT_EQ(sv1.compare(sv3.data()), 1);
    EXPECT_LT(sv1.compare(sv4.data()), 0);

    // compare(size_type, size_type, const value_type*)
    EXPECT_EQ(sv1.compare(0, 3, sv3.data()), 0);
    EXPECT_GT(sv1.compare(3, 3, sv3.data()), 0);
    EXPECT_LT(sv1.compare(0, 3, sv4.data()), 0);
    EXPECT_EQ(sv1.compare(3, 3, sv4.data()), 0);

    // compare(size_type, size_type, const value_type*, size_type)
    EXPECT_EQ(sv1.compare(0, 3, sv2.data(), 3), 0);
    EXPECT_LT(sv1.compare(0, 3, sv2.data() + 3, 3), 0);
    EXPECT_EQ(sv1.compare(3, 3, sv2.data() + 3, 3), 0);
    EXPECT_GT(sv1.compare(3, 3, sv2.data(), 3), 0);
}

TEST(StringViewTest, operation2)
{
    string_view sv1("123456");

    EXPECT_EQ(sv1.starts_with('1'), true);
    EXPECT_EQ(sv1.starts_with('2'), false);

    EXPECT_EQ(sv1.starts_with("123"), true);
    EXPECT_EQ(sv1.starts_with("23"), false);
    EXPECT_EQ(sv1.starts_with(""), true);
    EXPECT_EQ(sv1.starts_with("123456"), true);

    EXPECT_EQ(sv1.ends_with('6'), true);
    EXPECT_EQ(sv1.ends_with('5'), false);

    EXPECT_EQ(sv1.ends_with("456"), true);
    EXPECT_EQ(sv1.ends_with("45"), false);
    EXPECT_EQ(sv1.ends_with(""), true);
    EXPECT_EQ(sv1.ends_with("123456"), true);
}

TEST(StringViewTest, find)
{
    string_view sv1("123456");
    std::string str("123456");

    // find(basic_string_view, size_type) const
    EXPECT_EQ(sv1.find(string_view()), 0);
    EXPECT_EQ(sv1.find(string_view()), str.find(std::string()));
    EXPECT_EQ(sv1.find(string_view("23")), 1);
    EXPECT_EQ(sv1.find(string_view("6")), 5);
    EXPECT_EQ(sv1.find(string_view("23")), str.find(std::string("23")));
    EXPECT_EQ(sv1.find(string_view("6")), str.find(std::string("6")));
    EXPECT_EQ(sv1.find(string_view("1234567")), string_view::npos);
    EXPECT_EQ(sv1.find(string_view("1234567")), str.find(std::string("1234567")));

    EXPECT_EQ(sv1.find(string_view("23"), 1), 1);
    EXPECT_EQ(sv1.find(string_view("23"), 1), str.find(std::string("23"), 1));
    EXPECT_EQ(sv1.find(string_view("23"), 4), string_view::npos);
    EXPECT_EQ(sv1.find(string_view("23"), 4), str.find(std::string("23"), 4));
    EXPECT_EQ(sv1.find(string_view("23"), 10), string_view::npos);
    EXPECT_EQ(sv1.find(string_view("23"), 10), str.find(std::string("23"), 10));

    // find(value_type, size_type) const
    EXPECT_EQ(sv1.find('2'), 1);
    EXPECT_EQ(sv1.find('2'), str.find('2'));
    EXPECT_EQ(sv1.find('7'), string_view::npos);
    EXPECT_EQ(sv1.find('7'), str.find('7'));

    EXPECT_EQ(sv1.find('2', 4), string_view::npos);
    EXPECT_EQ(sv1.find('2', 4), str.find('2', 4));
    EXPECT_EQ(sv1.find('4', 2), 3);
    EXPECT_EQ(sv1.find('4', 2), str.find('4', 2));
    EXPECT_EQ(sv1.find('2', 10), string_view::npos);
    EXPECT_EQ(sv1.find('2', 10), str.find('2', 10));

    // find(const value_type*, size_type pos)
    EXPECT_EQ(sv1.find("23"), 1);
    EXPECT_EQ(sv1.find("6"), 5);
    EXPECT_EQ(sv1.find("23"), str.find(std::string("23")));
    EXPECT_EQ(sv1.find("6"), str.find(std::string("6")));
    EXPECT_EQ(sv1.find("1234567"), string_view::npos);
    EXPECT_EQ(sv1.find("1234567"), str.find(std::string("1234567")));

    EXPECT_EQ(sv1.find("23", 1), 1);
    EXPECT_EQ(sv1.find("23", 1), str.find(std::string("23"), 1));
    EXPECT_EQ(sv1.find("23", 4), string_view::npos);
    EXPECT_EQ(sv1.find("23", 4), str.find(std::string("23"), 4));
    EXPECT_EQ(sv1.find("23", 10), string_view::npos);
    EXPECT_EQ(sv1.find("23", 10), str.find(std::string("23"), 10));

    // find(const value_type*, size_type, size_type) const
    EXPECT_EQ(sv1.find("23", 1, 2), 1);
    EXPECT_EQ(sv1.find("23", 1, 2), str.find(std::string("23"), 1));
    EXPECT_EQ(sv1.find("23", 4, 2), string_view::npos);
    EXPECT_EQ(sv1.find("23", 4, 2), str.find(std::string("23"), 4));
    EXPECT_EQ(sv1.find("23", 10, 2), string_view::npos);
    EXPECT_EQ(sv1.find("23", 10, 2), str.find(std::string("23"), 10));
}

TEST(StringViewTest, rfind)
{
    string_view sv1("123456");
    std::string str("123456");

    // rfind(basic_string_view, size_type) const
    EXPECT_EQ(sv1.rfind(string_view()), sv1.size());
    EXPECT_EQ(sv1.rfind(string_view()), str.rfind(std::string()));
    EXPECT_EQ(sv1.rfind(string_view("23")), 1);
    EXPECT_EQ(sv1.rfind(string_view("6")), 5);
    EXPECT_EQ(sv1.rfind(string_view("23")), str.rfind(std::string("23")));
    EXPECT_EQ(sv1.rfind(string_view("6")), str.rfind(std::string("6")));
    EXPECT_EQ(sv1.rfind(string_view("1234567")), string_view::npos);
    EXPECT_EQ(sv1.rfind(string_view("1234567")), str.rfind(std::string("1234567")));

    EXPECT_EQ(sv1.rfind(string_view("23"), 1), 1);
    EXPECT_EQ(sv1.rfind(string_view("23"), 1), str.rfind(std::string("23"), 1));
    EXPECT_EQ(sv1.rfind(string_view("45"), 2), string_view::npos);
    EXPECT_EQ(sv1.rfind(string_view("45"), 2), str.rfind(std::string("45"), 2));
    EXPECT_EQ(sv1.rfind(string_view("23"), 10), 1);
    EXPECT_EQ(sv1.rfind(string_view("23"), 10), str.rfind(std::string("23"), 10));

    // rfind(value_type, size_type) const
    EXPECT_EQ(sv1.rfind('2'), 1);
    EXPECT_EQ(sv1.rfind('2'), str.rfind('2'));
    EXPECT_EQ(sv1.rfind('7'), string_view::npos);
    EXPECT_EQ(sv1.rfind('7'), str.rfind('7'));

    EXPECT_EQ(sv1.rfind('2', 1), 1);
    EXPECT_EQ(sv1.rfind('2', 1), str.rfind('2', 1));
    EXPECT_EQ(sv1.rfind('4', 2), string_view::npos);
    EXPECT_EQ(sv1.rfind('4', 2), str.rfind('4', 2));
    EXPECT_EQ(sv1.rfind('2', 10), 1);
    EXPECT_EQ(sv1.rfind('2', 10), str.rfind('2', 10));

    // rfind(const value_type*, size_type pos)
    EXPECT_EQ(sv1.rfind("23"), 1);
    EXPECT_EQ(sv1.rfind("6"), 5);
    EXPECT_EQ(sv1.rfind("23"), str.rfind(std::string("23")));
    EXPECT_EQ(sv1.rfind("6"), str.rfind(std::string("6")));
    EXPECT_EQ(sv1.rfind("1234567"), string_view::npos);
    EXPECT_EQ(sv1.rfind("1234567"), str.rfind(std::string("1234567")));

    EXPECT_EQ(sv1.rfind("23", 1), 1);
    EXPECT_EQ(sv1.rfind("23", 1), str.rfind(std::string("23"), 1));
    EXPECT_EQ(sv1.rfind("56", 3), string_view::npos);
    EXPECT_EQ(sv1.rfind("56", 3), str.rfind(std::string("56"), 3));
    EXPECT_EQ(sv1.rfind("23", 10), 1);
    EXPECT_EQ(sv1.rfind("23", 10), str.rfind(std::string("23"), 10));

    // rfind(const value_type*, size_type, size_type) const
    EXPECT_EQ(sv1.rfind("23", 1, 2), 1);
    EXPECT_EQ(sv1.rfind("23", 1, 2), str.rfind(std::string("23"), 1));
    EXPECT_EQ(sv1.rfind("56", 3, 2), string_view::npos);
    EXPECT_EQ(sv1.rfind("56", 3, 2), str.rfind(std::string("56"), 3));
    EXPECT_EQ(sv1.rfind("23", 10, 2), 1);
    EXPECT_EQ(sv1.rfind("23", 10, 2), str.rfind(std::string("23"), 10));
}

TEST(StringViewTest, find_first_of)
{
    string_view sv1("123123");
    std::string str1("123123");

    EXPECT_EQ(sv1.find_first_of(string_view()), string_view::npos);
    EXPECT_EQ(sv1.find_first_of(string_view()), str1.find_first_of(std::string()));
    EXPECT_EQ(sv1.find_first_of(string_view("23")), 1);
    EXPECT_EQ(sv1.find_first_of(string_view("23")), str1.find_first_of(std::string("23")));
    EXPECT_EQ(sv1.find_first_of(string_view("39")), 2);
    EXPECT_EQ(sv1.find_first_of(string_view("39")), str1.find_first_of(std::string("39")));
    EXPECT_EQ(sv1.find_first_of(string_view("93")), 2);
    EXPECT_EQ(sv1.find_first_of(string_view("93")), str1.find_first_of(std::string("93")));
    EXPECT_EQ(sv1.find_first_of(string_view("9")), string_view::npos);
    EXPECT_EQ(sv1.find_first_of(string_view("9")), str1.find_first_of(std::string("9")));

    EXPECT_EQ(sv1.find_first_of(string_view("23"), 10), string_view::npos);
    EXPECT_EQ(sv1.find_first_of(string_view("23"), 10), str1.find_first_of(std::string("23"), 10));
    EXPECT_EQ(sv1.find_first_of(string_view("23"), 2), 2);
    EXPECT_EQ(sv1.find_first_of(string_view("23"), 2), str1.find_first_of(std::string("23"), 2));
    EXPECT_EQ(sv1.find_first_of(string_view("23"), 3), 4);
    EXPECT_EQ(sv1.find_first_of(string_view("23"), 3), str1.find_first_of(std::string("23"), 3));

    EXPECT_EQ(sv1.find_first_of('2', 1), 1);
    EXPECT_EQ(sv1.find_first_of('2', 1), str1.find_first_of('2', 1));
    EXPECT_EQ(sv1.find_first_of('2', 2), 4);
    EXPECT_EQ(sv1.find_first_of('2', 2), str1.find_first_of('2', 2));
    EXPECT_EQ(sv1.find_first_of('2', 5), string_view::npos);
    EXPECT_EQ(sv1.find_first_of('2', 5), str1.find_first_of('2', 5));

    EXPECT_EQ(sv1.find_first_of("23", 2), 2);
    EXPECT_EQ(sv1.find_first_of("23", 2), str1.find_first_of("23", 2));
    EXPECT_EQ(sv1.find_first_of("23", 3), 4);
    EXPECT_EQ(sv1.find_first_of("23", 3), str1.find_first_of("23", 3));
    EXPECT_EQ(sv1.find_first_of("23", 10), string_view::npos);
    EXPECT_EQ(sv1.find_first_of("23", 10), str1.find_first_of("23", 10));

    EXPECT_EQ(sv1.find_first_of("23", 2, 2), 2);
    EXPECT_EQ(sv1.find_first_of("23", 2, 2), str1.find_first_of("23", 2, 2));
    EXPECT_EQ(sv1.find_first_of("23", 3, 2), 4);
    EXPECT_EQ(sv1.find_first_of("23", 3, 2), str1.find_first_of("23", 3, 2));
    EXPECT_EQ(sv1.find_first_of("23", 2, 1), 4);
    EXPECT_EQ(sv1.find_first_of("23", 2, 1), str1.find_first_of("23", 2, 1));
    EXPECT_EQ(sv1.find_first_of("23", 5, 1), string_view::npos);
    EXPECT_EQ(sv1.find_first_of("23", 5, 1), str1.find_first_of("23", 5, 1));
    EXPECT_EQ(sv1.find_first_of("23", 10, 2), string_view::npos);
    EXPECT_EQ(sv1.find_first_of("23", 10, 2), str1.find_first_of("23", 10, 2));
}

TEST(StringViewTest, find_last_of)
{
    string_view sv1("123123");
    std::string str1("123123");

    EXPECT_EQ(sv1.find_last_of(string_view()), string_view::npos);
    EXPECT_EQ(sv1.find_last_of(string_view()), str1.find_last_of(std::string()));
    EXPECT_EQ(sv1.find_last_of(string_view("23")), 5);
    EXPECT_EQ(sv1.find_last_of(string_view("23")), str1.find_last_of(std::string("23")));
    EXPECT_EQ(sv1.find_last_of(string_view("39")), 5);
    EXPECT_EQ(sv1.find_last_of(string_view("39")), str1.find_last_of(std::string("39")));
    EXPECT_EQ(sv1.find_last_of(string_view("93")), 5);
    EXPECT_EQ(sv1.find_last_of(string_view("93")), str1.find_last_of(std::string("93")));
    EXPECT_EQ(sv1.find_last_of(string_view("9")), string_view::npos);
    EXPECT_EQ(sv1.find_last_of(string_view("9")), str1.find_last_of(std::string("9")));

    EXPECT_EQ(sv1.find_last_of(string_view("23"), 10), 5);
    EXPECT_EQ(sv1.find_last_of(string_view("23"), 10), str1.find_last_of(std::string("23"), 10));
    EXPECT_EQ(sv1.find_last_of(string_view("23"), 3), 2);
    EXPECT_EQ(sv1.find_last_of(string_view("23"), 3), str1.find_last_of(std::string("23"), 3));
    EXPECT_EQ(sv1.find_last_of(string_view("23"), 1), 1);
    EXPECT_EQ(sv1.find_last_of(string_view("23"), 1), str1.find_last_of(std::string("23"), 1));
    EXPECT_EQ(sv1.find_last_of(string_view("23"), 0), string_view::npos);
    EXPECT_EQ(sv1.find_last_of(string_view("23"), 0), str1.find_last_of(std::string("23"), 0));

    EXPECT_EQ(sv1.find_last_of('2', 2), 1);
    EXPECT_EQ(sv1.find_last_of('2', 2), str1.find_last_of('2', 2));
    EXPECT_EQ(sv1.find_last_of('2', 4), 4);
    EXPECT_EQ(sv1.find_last_of('2', 4), str1.find_last_of('2', 4));

    EXPECT_EQ(sv1.find_last_of("23", 10), 5);
    EXPECT_EQ(sv1.find_last_of("23", 10), str1.find_last_of("23", 10));
    EXPECT_EQ(sv1.find_last_of("23", 3), 2);
    EXPECT_EQ(sv1.find_last_of("23", 3), str1.find_last_of("23", 3));
    EXPECT_EQ(sv1.find_last_of("23", 0), string_view::npos);
    EXPECT_EQ(sv1.find_last_of("23", 0), str1.find_last_of("23", 0));

    EXPECT_EQ(sv1.find_last_of("23", 10, 2), 5);
    EXPECT_EQ(sv1.find_last_of("23", 10, 2), str1.find_last_of("23", 10, 2));
    EXPECT_EQ(sv1.find_last_of("23", 3, 2), 2);
    EXPECT_EQ(sv1.find_last_of("23", 3, 2), str1.find_last_of("23", 3, 2));
    EXPECT_EQ(sv1.find_last_of("23", 2, 1), 1);
    EXPECT_EQ(sv1.find_last_of("23", 2, 1), str1.find_last_of("23", 2, 1));
}

TEST(StringViewTest, find_first_not_of)
{
    string_view sv1("123123");
    std::string str1("123123");

    EXPECT_EQ(sv1.find_first_not_of(string_view()), 0);
    EXPECT_EQ(sv1.find_first_not_of(string_view()), str1.find_first_not_of(std::string()));
    EXPECT_EQ(sv1.find_first_not_of(string_view("23")), 0);
    EXPECT_EQ(sv1.find_first_not_of(string_view("23")), str1.find_first_not_of(std::string("23")));
    EXPECT_EQ(sv1.find_first_not_of(string_view("39")), 0);
    EXPECT_EQ(sv1.find_first_not_of(string_view("39")), str1.find_first_not_of(std::string("39")));
    EXPECT_EQ(sv1.find_first_not_of(string_view("93")), 0);
    EXPECT_EQ(sv1.find_first_not_of(string_view("93")), str1.find_first_not_of(std::string("93")));

    EXPECT_EQ(sv1.find_first_not_of(string_view("23"), 10), string_view::npos);
    EXPECT_EQ(sv1.find_first_not_of(string_view("23"), 10), str1.find_first_not_of(std::string("23"), 10));
    EXPECT_EQ(sv1.find_first_not_of(string_view("23"), 3), 3);
    EXPECT_EQ(sv1.find_first_not_of(string_view("23"), 3), str1.find_first_not_of(std::string("23"), 3));
    EXPECT_EQ(sv1.find_first_not_of(string_view("23"), 1), 3);
    EXPECT_EQ(sv1.find_first_not_of(string_view("23"), 1), str1.find_first_not_of(std::string("23"), 1));
    EXPECT_EQ(sv1.find_first_not_of(string_view("23"), 4), string_view::npos);
    EXPECT_EQ(sv1.find_first_not_of(string_view("23"), 4), str1.find_first_not_of(std::string("23"), 4));

    EXPECT_EQ(sv1.find_first_not_of('2', 2), 2);
    EXPECT_EQ(sv1.find_first_not_of('2', 2), str1.find_first_not_of('2', 2));
    EXPECT_EQ(sv1.find_first_not_of('2', 4), 5);
    EXPECT_EQ(sv1.find_first_not_of('2', 4), str1.find_first_not_of('2', 4));

    EXPECT_EQ(sv1.find_first_not_of("23", 10), string_view::npos);
    EXPECT_EQ(sv1.find_first_not_of("23", 10), str1.find_first_not_of("23", 10));
    EXPECT_EQ(sv1.find_first_not_of("23", 3), 3);
    EXPECT_EQ(sv1.find_first_not_of("23", 3), str1.find_first_not_of("23", 3));

    EXPECT_EQ(sv1.find_first_not_of("23", 10, 2), string_view::npos);
    EXPECT_EQ(sv1.find_first_not_of("23", 10, 2), str1.find_first_not_of("23", 10, 2));
    EXPECT_EQ(sv1.find_first_not_of("23", 3, 2), 3);
    EXPECT_EQ(sv1.find_first_not_of("23", 3, 2), str1.find_first_not_of("23", 3, 2));
    EXPECT_EQ(sv1.find_first_not_of("23", 2, 1), 2);
    EXPECT_EQ(sv1.find_first_not_of("23", 2, 1), str1.find_first_not_of("23", 2, 1));
}

TEST(StringViewTest, find_last_not_of)
{
    string_view sv1("123123");
    std::string str1("123123");

    EXPECT_EQ(sv1.find_last_not_of(string_view()), 5);
    EXPECT_EQ(sv1.find_last_not_of(string_view()), str1.find_last_not_of(std::string()));
    EXPECT_EQ(sv1.find_last_not_of(string_view("23")), 3);
    EXPECT_EQ(sv1.find_last_not_of(string_view("23")), str1.find_last_not_of(std::string("23")));
    EXPECT_EQ(sv1.find_last_not_of(string_view("39")), 4);
    EXPECT_EQ(sv1.find_last_not_of(string_view("39")), str1.find_last_not_of(std::string("39")));
    EXPECT_EQ(sv1.find_last_not_of(string_view("93")), 4);
    EXPECT_EQ(sv1.find_last_not_of(string_view("93")), str1.find_last_not_of(std::string("93")));

    EXPECT_EQ(sv1.find_last_not_of(string_view("23"), 10), 3);
    EXPECT_EQ(sv1.find_last_not_of(string_view("23"), 10), str1.find_last_not_of(std::string("23"), 10));
    EXPECT_EQ(sv1.find_last_not_of(string_view("23"), 3), 3);
    EXPECT_EQ(sv1.find_last_not_of(string_view("23"), 3), str1.find_last_not_of(std::string("23"), 3));
    EXPECT_EQ(sv1.find_last_not_of(string_view("23"), 1), 0);
    EXPECT_EQ(sv1.find_last_not_of(string_view("23"), 1), str1.find_last_not_of(std::string("23"), 1));
    EXPECT_EQ(sv1.find_last_not_of(string_view("23"), 0), 0);
    EXPECT_EQ(sv1.find_last_not_of(string_view("23"), 0), str1.find_last_not_of(std::string("23"), 0));

    EXPECT_EQ(sv1.find_last_not_of('2', 2), 2);
    EXPECT_EQ(sv1.find_last_not_of('2', 2), str1.find_last_not_of('2', 2));
    EXPECT_EQ(sv1.find_last_not_of('2', 4), 3);
    EXPECT_EQ(sv1.find_last_not_of('2', 4), str1.find_last_not_of('2', 4));
    EXPECT_EQ(sv1.find_last_not_of('1', 0), string_view::npos);
    EXPECT_EQ(sv1.find_last_not_of('1', 0), str1.find_last_not_of('1', 0));

    EXPECT_EQ(sv1.find_last_not_of("23", 10), 3);
    EXPECT_EQ(sv1.find_last_not_of("23", 10), str1.find_last_not_of("23", 10));
    EXPECT_EQ(sv1.find_last_not_of("23", 2), 0);
    EXPECT_EQ(sv1.find_last_not_of("23", 2), str1.find_last_not_of("23", 2));

    EXPECT_EQ(sv1.find_last_not_of("23", 10, 2), 3);
    EXPECT_EQ(sv1.find_last_not_of("23", 10, 2), str1.find_last_not_of("23", 10, 2));
    EXPECT_EQ(sv1.find_last_not_of("23", 3, 2), 3);
    EXPECT_EQ(sv1.find_last_not_of("23", 3, 2), str1.find_last_not_of("23", 3, 2));
    EXPECT_EQ(sv1.find_last_not_of("23", 2, 1), 2);
    EXPECT_EQ(sv1.find_last_not_of("23", 2, 1), str1.find_last_not_of("23", 2, 1));
}

TEST(StringViewTest, operation3)
{
    string_view sv1{ "123456" };
    string_view sv2{ "123" };
    string_view sv3{ "456" };
    string_view sv4;
    // operator ==
    EXPECT_EQ(sv1 == string_view("123456"), true);
    EXPECT_EQ(sv4 == string_view(), true);
    EXPECT_EQ(sv1 == sv2, false);
    EXPECT_EQ(sv4 == sv2, false);

    // operator !=
    EXPECT_EQ(sv1 != string_view("123456"), false);
    EXPECT_EQ(sv4 != string_view(), false);
    EXPECT_EQ(sv1 != sv2, true);
    EXPECT_EQ(sv4 != sv2, true);

    // operator <
    EXPECT_EQ(sv1 < string_view("123456"), false);
    EXPECT_EQ(sv4 < string_view(), false);
    EXPECT_EQ(sv1 < sv2, false);
    EXPECT_EQ(sv1 < sv3, true);
    EXPECT_EQ(sv4 < sv2, true);
    EXPECT_EQ(sv4 < sv3, true);

    // operator >
    EXPECT_EQ(sv1 > string_view("123456"), false);
    EXPECT_EQ(sv4 > string_view(), false);
    EXPECT_EQ(sv1 > sv2, true);
    EXPECT_EQ(sv1 > sv3, false);
    EXPECT_EQ(sv4 > sv2, false);
    EXPECT_EQ(sv4 > sv3, false);

    // operator <=
    EXPECT_EQ(sv1 <= string_view("123456"), true);
    EXPECT_EQ(sv4 <= string_view(), true);
    EXPECT_EQ(sv1 <= sv2, false);
    EXPECT_EQ(sv1 <= sv3, true);
    EXPECT_EQ(sv4 <= sv2, true);
    EXPECT_EQ(sv4 <= sv3, true);

    // operator >=
    EXPECT_EQ(sv1 >= string_view("123456"), true);
    EXPECT_EQ(sv4 >= string_view(), true);
    EXPECT_EQ(sv1 >= sv2, true);
    EXPECT_EQ(sv1 >= sv3, false);
    EXPECT_EQ(sv4 >= sv2, false);
    EXPECT_EQ(sv4 >= sv3, false);

    // std::string
    std::string str2{ "123" };
    std::string str3{ "456" };

    // operator ==
    EXPECT_EQ(sv1 == std::string("123456"), true);
    EXPECT_EQ(std::string("123456") == sv1, true);
    EXPECT_EQ(sv4 == std::string(), true);
    EXPECT_EQ(std::string() == sv4, true);
    EXPECT_EQ(sv1 == str2, false);
    EXPECT_EQ(str2 == sv1, false);
    EXPECT_EQ(sv1 == str3, false);
    EXPECT_EQ(str3 == sv1, false);
    EXPECT_EQ(sv4 == str2, false);
    EXPECT_EQ(str2 == sv4, false);
    EXPECT_EQ(sv4 == str3, false);
    EXPECT_EQ(str3 == sv4, false);

    // operator !=
    EXPECT_EQ(sv1 != std::string("123456"), false);
    EXPECT_EQ(std::string("123456") != sv1, false);
    EXPECT_EQ(sv4 != std::string(), false);
    EXPECT_EQ(std::string() != sv4, false);
    EXPECT_EQ(sv1 != str2, true);
    EXPECT_EQ(str2 != sv1, true);
    EXPECT_EQ(sv1 != str3, true);
    EXPECT_EQ(str3 != sv1, true);
    EXPECT_EQ(sv4 != str2, true);
    EXPECT_EQ(str2 != sv4, true);
    EXPECT_EQ(sv4 != str3, true);
    EXPECT_EQ(str3 != sv4, true);

    // operator <
    EXPECT_EQ(sv1 < std::string("123456"), false);
    EXPECT_EQ(std::string("123456") < sv1, false);
    EXPECT_EQ(sv4 < std::string(), false);
    EXPECT_EQ(std::string() < sv4, false);
    EXPECT_EQ(sv1 < str2, false);
    EXPECT_EQ(str2 < sv1, true);
    EXPECT_EQ(sv1 < str3, true);
    EXPECT_EQ(str3 < sv1, false);
    EXPECT_EQ(sv4 < str2, true);
    EXPECT_EQ(str2 < sv4, false);
    EXPECT_EQ(sv4 < str3, true);
    EXPECT_EQ(str3 < sv4, false);

    // operator >
    EXPECT_EQ(sv1 > std::string("123456"), false);
    EXPECT_EQ(std::string("123456") > sv1, false);
    EXPECT_EQ(sv4 > std::string(), false);
    EXPECT_EQ(std::string() > sv4, false);
    EXPECT_EQ(sv1 > str2, true);
    EXPECT_EQ(str2 > sv1, false);
    EXPECT_EQ(sv1 > str3, false);
    EXPECT_EQ(str3 > sv1, true);
    EXPECT_EQ(sv4 > str2, false);
    EXPECT_EQ(str2 > sv4, true);
    EXPECT_EQ(sv4 > str3, false);
    EXPECT_EQ(str3 > sv4, true);

    // operator <=
    EXPECT_EQ(sv1 <= std::string("123456"), true);
    EXPECT_EQ(std::string("123456") <= sv1, true);
    EXPECT_EQ(sv4 <= std::string(), true);
    EXPECT_EQ(std::string() <= sv4, true);
    EXPECT_EQ(sv1 <= str2, false);
    EXPECT_EQ(str2 <= sv1, true);
    EXPECT_EQ(sv1 <= str3, true);
    EXPECT_EQ(str3 <= sv1, false);
    EXPECT_EQ(sv4 <= str2, true);
    EXPECT_EQ(str2 <= sv4, false);
    EXPECT_EQ(sv4 <= str3, true);
    EXPECT_EQ(str3 <= sv4, false);

    // operator >=
    EXPECT_EQ(sv1 >= std::string("123456"), true);
    EXPECT_EQ(std::string("123456") >= sv1, true);
    EXPECT_EQ(sv4 >= std::string(), true);
    EXPECT_EQ(std::string() >= sv4, true);
    EXPECT_EQ(sv1 >= str2, true);
    EXPECT_EQ(str2 >= sv1, false);
    EXPECT_EQ(sv1 >= str3, false);
    EXPECT_EQ(str3 >= sv1, true);
    EXPECT_EQ(sv4 >= str2, false);
    EXPECT_EQ(str2 >= sv4, true);
    EXPECT_EQ(sv4 >= str3, false);
    EXPECT_EQ(str3 >= sv4, true);

    // const char*
    const char* ch2{ "123" };
    const char* ch3{ "456" };

    // operator ==
    EXPECT_EQ(sv1 == "123456", true);
    EXPECT_EQ("123456" == sv1, true);
    EXPECT_EQ(sv4 == "", true);
    EXPECT_EQ("" == sv4, true);
    EXPECT_EQ(sv1 == ch2, false);
    EXPECT_EQ(ch2 == sv1, false);
    EXPECT_EQ(sv1 == ch3, false);
    EXPECT_EQ(ch3 == sv1, false);
    EXPECT_EQ(sv4 == ch2, false);
    EXPECT_EQ(ch2 == sv4, false);
    EXPECT_EQ(sv4 == ch3, false);
    EXPECT_EQ(ch3 == sv4, false);

    // operator !=
    EXPECT_EQ(sv1 != std::string("123456"), false);
    EXPECT_EQ(std::string("123456") != sv1, false);
    EXPECT_EQ(sv4 != std::string(), false);
    EXPECT_EQ(std::string() != sv4, false);
    EXPECT_EQ(sv1 != ch2, true);
    EXPECT_EQ(ch2 != sv1, true);
    EXPECT_EQ(sv1 != ch3, true);
    EXPECT_EQ(ch3 != sv1, true);
    EXPECT_EQ(sv4 != ch2, true);
    EXPECT_EQ(ch2 != sv4, true);
    EXPECT_EQ(sv4 != ch3, true);
    EXPECT_EQ(ch3 != sv4, true);

    // operator <
    EXPECT_EQ(sv1 < std::string("123456"), false);
    EXPECT_EQ(std::string("123456") < sv1, false);
    EXPECT_EQ(sv4 < std::string(), false);
    EXPECT_EQ(std::string() < sv4, false);
    EXPECT_EQ(sv1 < ch2, false);
    EXPECT_EQ(ch2 < sv1, true);
    EXPECT_EQ(sv1 < ch3, true);
    EXPECT_EQ(ch3 < sv1, false);
    EXPECT_EQ(sv4 < ch2, true);
    EXPECT_EQ(ch2 < sv4, false);
    EXPECT_EQ(sv4 < ch3, true);
    EXPECT_EQ(ch3 < sv4, false);

    // operator >
    EXPECT_EQ(sv1 > std::string("123456"), false);
    EXPECT_EQ(std::string("123456") > sv1, false);
    EXPECT_EQ(sv4 > std::string(), false);
    EXPECT_EQ(std::string() > sv4, false);
    EXPECT_EQ(sv1 > ch2, true);
    EXPECT_EQ(ch2 > sv1, false);
    EXPECT_EQ(sv1 > ch3, false);
    EXPECT_EQ(ch3 > sv1, true);
    EXPECT_EQ(sv4 > ch2, false);
    EXPECT_EQ(ch2 > sv4, true);
    EXPECT_EQ(sv4 > ch3, false);
    EXPECT_EQ(ch3 > sv4, true);

    // operator <=
    EXPECT_EQ(sv1 <= std::string("123456"), true);
    EXPECT_EQ(std::string("123456") <= sv1, true);
    EXPECT_EQ(sv4 <= std::string(), true);
    EXPECT_EQ(std::string() <= sv4, true);
    EXPECT_EQ(sv1 <= ch2, false);
    EXPECT_EQ(ch2 <= sv1, true);
    EXPECT_EQ(sv1 <= ch3, true);
    EXPECT_EQ(ch3 <= sv1, false);
    EXPECT_EQ(sv4 <= ch2, true);
    EXPECT_EQ(ch2 <= sv4, false);
    EXPECT_EQ(sv4 <= ch3, true);
    EXPECT_EQ(ch3 <= sv4, false);

    // operator >=
    EXPECT_EQ(sv1 >= std::string("123456"), true);
    EXPECT_EQ(std::string("123456") >= sv1, true);
    EXPECT_EQ(sv4 >= std::string(), true);
    EXPECT_EQ(std::string() >= sv4, true);
    EXPECT_EQ(sv1 >= ch2, true);
    EXPECT_EQ(ch2 >= sv1, false);
    EXPECT_EQ(sv1 >= ch3, false);
    EXPECT_EQ(ch3 >= sv1, true);
    EXPECT_EQ(sv4 >= ch2, false);
    EXPECT_EQ(ch2 >= sv4, true);
    EXPECT_EQ(sv4 >= ch3, false);
    EXPECT_EQ(ch3 >= sv4, true);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
