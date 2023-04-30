#include "base/thread/count_down_latch.h"

#include <random>
#include <thread>
#include <sstream>
#include <iostream>

using std::chrono::microseconds;
using namespace halcyon::base;

class Test
{
public:
    Test(int32_t num_threads)
        : latch_(num_threads)
    {
        for (int32_t i = 0; i < num_threads; ++i) {
            threads_.emplace_back(new std::thread(&Test::threadFunc, this));
        }
    }

    void run()
    {
        std::cout << "wait thread....." << std::endl;
        latch_.waitFor(5000);
        std::cout << "all thread is reading" << std::endl;
    }

    void joinAll()
    {
        for (auto& thd : threads_) {
            thd->join();
        }
    }

private:
    void threadFunc()
    {
        std::default_random_engine random((uint32_t)time(nullptr));
        std::uniform_int_distribution<int32_t> intDis(400, 900);
        int32_t tm = intDis(random);

        // 使用 sleep 模式初始化
        std::this_thread::sleep_for(microseconds(tm));
        std::stringstream ss;
        ss << "thread[" << std::this_thread::get_id() << "] ok" << std::endl;
        std::cout << ss.str();

        latch_.countDown();
    }

private:
    CountDownLatch latch_;
    std::vector<std::unique_ptr<std::thread>> threads_;
};

int main(int argc, char* argv[])
{
    Test t(5);
    t.run();
    t.joinAll();
    return 0;
}
