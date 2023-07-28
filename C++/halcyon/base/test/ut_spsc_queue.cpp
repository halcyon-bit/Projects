#include "base/queue/spsc_queue.h"
#include "base/time/timestamp.h"

#include "gtest/gtest.h"

#include <thread>
#include <random>

using namespace halcyon;

class TestMove : public base::noncopyable
{
public:
    TestMove(size_t i)
        : value_(i)
    {
        str_ = new char[128];
    }

    ~TestMove()
    {
        if (str_ != nullptr)
            delete[] str_;
    }

    TestMove(TestMove&& rhs) noexcept
    {
        if (this != &rhs) {
            str_ = rhs.str_;
            rhs.str_ = nullptr;
            value_ = rhs.value_;
            ++move_count_;
        }
    }

    TestMove& operator=(TestMove&& rhs) noexcept
    {
        if (this != &rhs) {
            delete[] str_;
            str_ = rhs.str_;
            rhs.str_ = nullptr;
            value_ = rhs.value_;
            ++oper_count_;
        }
        return *this;
    }

    operator size_t() const
    {
        return value_;
    }

    static size_t move_count_;
    static size_t oper_count_;
    size_t value_{ 0 };
    char* str_{ nullptr };
};
size_t TestMove::move_count_ = 0;
size_t TestMove::oper_count_ = 0;

class TestNoMove
{
public:
    TestNoMove(size_t i)
        : value_(i)
    {
        str_ = new char[128];
    }

    ~TestNoMove()
    {
        if (str_ != nullptr)
            delete[] str_;
    }

    TestNoMove(const TestNoMove& rhs)
    {
        if (this != &rhs) {
            str_ = new char[128];
            value_ = rhs.value_;
            ++copy_count_;
        }
    }

    TestNoMove& operator=(const TestNoMove& rhs)
    {
        if (this != &rhs) {
            delete[] str_;
            str_ = new char[128];
            value_ = rhs.value_;
            ++oper_count_;
        }
        return *this;
    }

    operator size_t() const
    {
        return value_;
    }

    static size_t copy_count_;
    static size_t oper_count_;
    size_t value_{ 0 };
    char* str_{ nullptr };
};
size_t TestNoMove::copy_count_ = 0;
size_t TestNoMove::oper_count_ = 0;

size_t producer_thing_count = 0;
size_t consumer_thing_count = 0;

void producerThing_1(base::SPSCQueue<size_t, 1024>& queue)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 2);

    producer_thing_count = 0;
    for (size_t i = 0; i < 2048; ++i) {
        base::sleep(distrib(gen));
        if (queue.push(i))
            ++producer_thing_count;
    }
}

void consumerThing_1(base::SPSCQueue<size_t, 1024>& queue)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 6);

    consumer_thing_count = 0;
    for (size_t i = 0; i < 2048; ++i) {
        base::sleep(distrib(gen));
        size_t res;
        if (queue.take(res))
            ++consumer_thing_count;
    }

    while (!queue.empty()) {
        size_t res;
        if (queue.take(res))
            ++consumer_thing_count;
    }
}

void producerThing_2(base::SPSCQueue<size_t>& queue)
{
    producer_thing_count = 0;
    for (size_t i = 0; i < 2048; ++i) {
        if (queue.push(i))
            ++producer_thing_count;
    }
}

void consumerThing_2(base::SPSCQueue<size_t>& queue)
{
    consumer_thing_count = 0;
    for (size_t i = 0; i < 2048; ++i) {
        size_t res;
        if (queue.take(res))
            ++consumer_thing_count;
    }
    base::sleep(1);
    while (!queue.empty()) {
        size_t res;
        if (queue.take(res))
            ++consumer_thing_count;
    }
}

TEST(SPSCQueue, normal)
{
    {
        base::SPSCQueue<size_t, 100> queue;
        EXPECT_EQ(queue.empty(), true);

        for (size_t i = 0; i < 100; ++i) {
            bool ret = queue.push(i);
            EXPECT_EQ(ret, true);
        }
        bool ret = queue.push(100);
        EXPECT_EQ(ret, false);

        EXPECT_EQ(queue.empty(), false);

        for (size_t i = 0; i < 100; ++i) {
            size_t value{ size_t(-1) };
            bool ret = queue.take(value);
            EXPECT_EQ(ret, true);
            EXPECT_EQ(value, i);
        }
        size_t value{ size_t(-1) };
        ret = queue.take(value);
        EXPECT_EQ(ret, false);
        EXPECT_EQ(value, -1);

        EXPECT_EQ(queue.empty(), true);
    }

    {
        base::SPSCQueue<size_t> queue(100);
        EXPECT_EQ(queue.empty(), true);

        for (size_t i = 0; i < 100; ++i) {
            bool ret = queue.push(i);
            EXPECT_EQ(ret, true);
        }
        bool ret = queue.push(100);
        EXPECT_EQ(ret, false);

        EXPECT_EQ(queue.empty(), false);

        for (size_t i = 0; i < 100; ++i) {
            size_t value{ size_t(-1) };
            bool ret = queue.take(value);
            EXPECT_EQ(ret, true);
            EXPECT_EQ(value, i);
        }
        size_t value{ size_t(-1) };
        ret = queue.take(value);
        EXPECT_EQ(ret, false);
        EXPECT_EQ(value, -1);

        EXPECT_EQ(queue.empty(), true);
    }
}

TEST(SPSCQueue, obj_with_move)
{
    {
        base::SPSCQueue<TestMove, 100> queue;
        EXPECT_EQ(queue.empty(), true);

        for (size_t i = 0; i < 100; ++i) {
            bool ret = queue.push(TestMove(i));
            EXPECT_EQ(ret, true);
        }
        bool ret = queue.push(TestMove(100));
        EXPECT_EQ(ret, false);

        EXPECT_EQ(queue.empty(), false);

        EXPECT_EQ(TestMove::move_count_, 100);

        for (size_t i = 0; i < 100; ++i) {
            TestMove value(-1);
            bool ret = queue.take(value);
            EXPECT_EQ(ret, true);
            EXPECT_EQ(value, i);
        }
        TestMove value(-1);
        ret = queue.take(value);
        EXPECT_EQ(ret, false);
        EXPECT_EQ(value, -1);

        EXPECT_EQ(queue.empty(), true);

        EXPECT_EQ(TestMove::oper_count_, 100);
    }

    {
        base::SPSCQueue<TestMove> queue(100);
        EXPECT_EQ(queue.empty(), true);

        for (size_t i = 0; i < 100; ++i) {
            bool ret = queue.push(TestMove(i));
            EXPECT_EQ(ret, true);
        }
        bool ret = queue.push(TestMove(100));
        EXPECT_EQ(ret, false);

        EXPECT_EQ(queue.empty(), false);
        
        EXPECT_EQ(TestMove::move_count_, 200);

        for (size_t i = 0; i < 100; ++i) {
            TestMove value(-1);
            bool ret = queue.take(value);
            EXPECT_EQ(ret, true);
            EXPECT_EQ(value, i);
        }
        TestMove value(-1);
        ret = queue.take(value);
        EXPECT_EQ(ret, false);
        EXPECT_EQ(value, -1);

        EXPECT_EQ(queue.empty(), true);

        EXPECT_EQ(TestMove::oper_count_, 200);
    }

    {
        base::SPSCQueue<TestMove, 100> queue;
        queue.push(TestMove(66));
    }

    {
        base::SPSCQueue<TestMove> queue(100);
        queue.push(TestMove(66));
    }
}

TEST(SPSCQueue, obj_no_move)
{
    {
        base::SPSCQueue<TestNoMove, 100> queue;
        EXPECT_EQ(queue.empty(), true);

        for (size_t i = 0; i < 100; ++i) {
            TestNoMove t(i);
            bool ret = queue.push(t);
            EXPECT_EQ(ret, true);
        }
        TestNoMove t(100);
        bool ret = queue.push(t);
        EXPECT_EQ(ret, false);

        EXPECT_EQ(queue.empty(), false);

        EXPECT_EQ(TestNoMove::copy_count_, 100);

        for (size_t i = 0; i < 100; ++i) {
            TestNoMove value(-1);
            bool ret = queue.take(value);
            EXPECT_EQ(ret, true);
            EXPECT_EQ(value, i);
        }
        TestNoMove value(-1);
        ret = queue.take(value);
        EXPECT_EQ(ret, false);
        EXPECT_EQ(value, -1);

        EXPECT_EQ(queue.empty(), true);

        EXPECT_EQ(TestNoMove::oper_count_, 100);
    }

    {
        base::SPSCQueue<TestNoMove> queue(100);
        EXPECT_EQ(queue.empty(), true);

        for (size_t i = 0; i < 100; ++i) {
            TestNoMove t(i);
            bool ret = queue.push(t);
            EXPECT_EQ(ret, true);
        }
        TestNoMove t(100);
        bool ret = queue.push(t);
        EXPECT_EQ(ret, false);

        EXPECT_EQ(queue.empty(), false);

        EXPECT_EQ(TestNoMove::copy_count_, 200);

        for (size_t i = 0; i < 100; ++i) {
            TestNoMove value(-1);
            bool ret = queue.take(value);
            EXPECT_EQ(ret, true);
            EXPECT_EQ(value, i);
        }
        TestNoMove value(-1);
        ret = queue.take(value);
        EXPECT_EQ(ret, false);
        EXPECT_EQ(value, -1);

        EXPECT_EQ(queue.empty(), true);

        EXPECT_EQ(TestNoMove::oper_count_, 200);
    }

    {
        base::SPSCQueue<TestNoMove, 100> queue;
        TestNoMove t(66);
        queue.push(t);
    }

    {
        base::SPSCQueue<TestNoMove> queue(100);
        TestNoMove t(66);
        queue.push(t);
    }
}

TEST(SPSCQueue, thread_test_1)
{
    {
        base::SPSCQueue<size_t, 1024> queue;

        std::thread producer(&producerThing_1, std::ref(queue));
        std::thread consumer(&consumerThing_1, std::ref(queue));

        producer.join();
        consumer.join();

        EXPECT_EQ(consumer_thing_count, producer_thing_count);
        EXPECT_EQ(queue.empty(), true);
    }
}

TEST(SPSCQueue, thread_test_2)
{
    {
        base::SPSCQueue<size_t> queue(1024);

        std::thread producer(&producerThing_2, std::ref(queue));
        std::thread consumer(&consumerThing_2, std::ref(queue));

        producer.join();
        consumer.join();

        EXPECT_EQ(consumer_thing_count, producer_thing_count);
        EXPECT_EQ(queue.empty(), true);
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
