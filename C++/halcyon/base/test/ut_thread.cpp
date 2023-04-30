#include "base/thread/thread.h"
#include "base/time/timestamp.h"

#include "gtest/gtest.h"
#include <ctime>
#include <iostream>

using namespace halcyon;

// 类成员函数
class TestX
{
public:
    void testc() const
    {
        std::cout << "TestX::testc\n";
    }

    void test()
    {
        std::cout << "TestX::test: sleep(2s)\n";
        base::sleep(2000);
    }
};

// 无参的普通函数
int64_t test1()
{
    int64_t res = 0;
    for (int32_t i = 0; i < 100000; ++i) {
        res += i;
    }
    std::cout << "test1: add(0 ~ 99999): " << res << std::endl;
    return res;
}

// 带参数的普通函数
int test2(int32_t i, const std::string& str)
{
    std::cout << "test2(int, const string): " << i << " " << str << std::endl;
    return 0;
}

void test3(const std::string& str)
{
    std::cout << "test3(const string): " << str << std::endl;
}

int test4(const std::vector<int>& height)
{
    size_t n = height.size();
    if (n == 0) {
        return 0;
    }
    std::vector<int> leftMax(n);
    leftMax[0] = height[0];
    for (size_t i = 1; i < n; ++i) {
        leftMax[i] = std::max(leftMax[i - 1], height[i]);
    }

    std::vector<int> rightMax(n);
    rightMax[n - 1] = height[n - 1];
    for (int i = (int)n - 2; i >= 0; --i) {
        rightMax[i] = std::max(rightMax[i + 1], height[i]);
    }

    int ans = 0;
    for (size_t i = 0; i < n; ++i) {
        ans += std::min(leftMax[i], rightMax[i]) - height[i];
    }
    return ans;
}

// 可调用对象
struct TestY
{
    int operator()(double, const std::string&) const
    {
        std::cout << "TestY::operator\n";
        return 0;
    }
};

void result_recv(base::Task* task)
{
    EXPECT_EQ(task->cancelled(), false);
#ifdef USE_HALCYON_ANY
    base::Any sum;
    task->result(sum, 5000);
    std::cout << "test4 result: " << sum.anyCast<int>() << std::endl;
    EXPECT_EQ(6, sum.anyCast<int>());
#else
    std::any sum;
    task->result(sum);
    std::cout << "test4 result: " << std::any_cast<int>(sum) << std::endl;
    EXPECT_EQ(6, std::any_cast<int>(sum));
#endif
}

TEST(ThreadTest, constructor)
{
    base::Thread thd;
    EXPECT_EQ(true, thd.joinable());
    EXPECT_EQ(true, thd.started());
}

TEST(ThreadTest, push)
{
    base::Thread thd;
    // 类成员函数
    TestX x;
    thd.push(&TestX::testc, &x);
    thd.push(&TestX::test, &x);

    {
        // 全局函数
        auto result = thd.push(&test1);
#ifdef USE_HALCYON_ANY
        base::Any sum;
        result->result(sum, 5000);
        std::cout << "test1 result: " << sum.anyCast<int64_t>() << std::endl;
        EXPECT_EQ(4999950000, sum.anyCast<int64_t>());
#else
        std::any sum;
        result->result(sum, 5000);
        std::cout << "test1 result: " << std::any_cast<int64_t>(sum) << std::endl;
        EXPECT_EQ(4999950000, std::any_cast<int64_t>(sum));
#endif
    }
    // std::bind
    auto func2 = std::bind(&test2, 10, "hello");
    thd.push(std::move(func2));

    thd.push(&test2, 1000, "world");

    // 仿函数
    TestY y;
    thd.push(y, 100.0, "C++");

    // std::function
    std::function<void()> funcv;
    thd.push(funcv);

    funcv = std::bind(&test1);
    thd.push(funcv);

    //thd.push(nullptr);   // nullptr_t

    // lambda
    auto lambda_func = []() {
        std::cout << "lambda_1\n";
    };
    thd.push(lambda_func);

    thd.push([]() {
        std::cout << "lambda_2: sleep(1s)\n";
    base::sleep(1000);
        });

    thd.push([](int a, int b) {
        std::cout << "lambda_3(int, int): ";
    std::cout << a << " " << b << std::endl;
        }, 1, 3);

    // 回调
    {
        std::vector<int> vec{ 0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1 };
        auto result = thd.push(&test4, vec);
        result->setDoneCallback(std::bind(&result_recv, std::placeholders::_1));
    }
    {
        std::vector<int> vec{ 0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1 };
        auto result = thd.push(&test4, vec);
        result->cancel();
        base::sleep(1000);
        EXPECT_EQ(result->cancelled(), true);
    }
    thd.join();
}

TEST(ThreadTest, memfunc)
{
    base::Thread thd;
    thd.join();
    thd.join();
    EXPECT_EQ(false, thd.joinable());
    EXPECT_EQ(false, thd.started());
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}