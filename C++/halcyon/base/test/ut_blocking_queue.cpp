#include "base/queue/blocking_queue.h"
#include "base/thread/count_down_latch.h"

#include <vector>
#include <thread>
#include <sstream>
#include <iostream>

using namespace halcyon::base;

class Test
{
public:
    Test(int32_t numThreads)
        : latch_(numThreads)
    {
        for (int32_t i = 0; i < numThreads; ++i) {
            threads_.emplace_back(std::move(std::thread(&Test::threadFunc, this)));
        }
    }

    void run(int32_t times)
    {
        latch_.wait();
        for (int32_t i = 0; i < times; ++i) {
            std::stringstream ss;
            ss << "hello " << i;
            queue_.push(std::move(ss.str()));
        }
    }

    void joinAll()
    {
        size_t size = threads_.size();
        for (size_t i = 0; i < size; ++i) {
            // 因 BlockingQueue 暂未支持多线程退出，所以 push "stop" 多次(线程数)
            queue_.push("stop");
        }

        for (auto& thd : threads_) {
            thd.join();
        }
    }

private:
    void threadFunc()
    {
        latch_.countDown();
        bool running = true;
        while (running) {
            std::string d(queue_.take());
            std::stringstream ss;
            ss << "tid: " << std::this_thread::get_id() << ", get data = " << d << ", size = " << queue_.size() << std::endl;
            std::cout << ss.str();
            // 停止标记 "stop"
            running = (d != "stop");
        }
    }

private:
    BlockingQueue<std::string> queue_;
    CountDownLatch latch_;
    std::vector<std::thread> threads_;
};

void testMove()
{
    BlockingQueue<std::unique_ptr<int32_t>> queue;
    queue.put(std::unique_ptr<int32_t>(new int32_t(42)));
    std::unique_ptr<int32_t> x = queue.take();
    std::cout << "take " << *x << std::endl;
    *x = 123;
    queue.put(std::move(x));
    std::unique_ptr<int32_t> y = queue.take();
    std::cout << "take " << *y << std::endl;
}

int main(int argc, char* argv[])
{
    Test t(5);
    t.run(100);
    t.joinAll();

    testMove();
    return 0;
}
