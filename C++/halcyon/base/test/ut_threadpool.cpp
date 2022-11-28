#include "base/thread/thread_pool.h"
#include "base/time/timestamp.h"

#include <chrono>
#include <sstream>
#include <iostream>

using namespace halcyon;

class Foo
{
public:
    void test(int32_t n)
    {
        std::cout << "class member function\n";
    }
};

void test()
{
    std::cout << "global test function: sleep 5s\n";
    base::sleep(5000);
}

int main(int argc, char* argv[])
{
    base::ThreadPool pool;
    pool.start();

    std::cout << "thread size " << pool.getTotalThreadNum() << std::endl;

    std::atomic<int32_t> index;
    index.store(0);
    std::thread t([&]() {
        for (int32_t i = 0; i < 30; ++i) {
            pool.addTask([&]() {
                index++;
                std::cout << "function " << index.load() << std::endl;
                base::sleep(2000);
            });
        }
    });
    t.detach();

    base::sleep(4000);
    std::cout << "thread size " << pool.getTotalThreadNum() << std::endl;
    std::cout << "waiting size " << pool.getWaitingThreadNum() << std::endl;

    base::sleep(4000);
    std::cout << "thread size " << pool.getTotalThreadNum() << std::endl;
    std::cout << "waiting size " << pool.getWaitingThreadNum() << std::endl;

    std::thread thd1([&pool] {
        for (int32_t i = 0; i < 10; ++i) {
            auto thdId = std::this_thread::get_id();
            pool.addTask([thdId] { 
                std::stringstream ss;
                ss << "执行任务, 添加任务线程: " << thdId
                << "\t\t执行线程：" << std::this_thread::get_id() << "\n";
                std::cout << ss.str();
            });
        }
    });

    std::thread thd2([&pool] {
        for (int32_t i = 0; i < 10; ++i) {
            auto thdId = std::this_thread::get_id();
            pool.addTask([thdId] {
                std::stringstream ss;
                ss << "执行任务, 添加任务线程: " << thdId
                << "\t\t执行线程：" << std::this_thread::get_id() << "\n";
                std::cout << ss.str();
            });
        }
    });

    thd1.join(); 
    thd2.join();

    base::sleep(10000);
    std::cout << "thread size " << pool.getTotalThreadNum() << std::endl;
    std::cout << "waiting size " << pool.getWaitingThreadNum() << std::endl;
    pool.addTask(&test);
    Foo f;
    pool.addTask(&Foo::test, &f, 100);
    pool.shutDown();
    return 0;
}
