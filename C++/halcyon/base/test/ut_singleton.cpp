#include "base/singleton/singleton.h"

#include <thread>
#include <string>
#include <iostream>
using namespace halcyon::base;

class Test
{
public:
    Test()
    {
        std::cout << "tid=" << std::this_thread::get_id()
            << ", constructing Test\n\n";
    }

    ~Test()
    {
        std::cout << "tid=" << std::this_thread::get_id()
            << ", destructing Test.\n\n";
    }

    const std::string& name() const
    {
        return name_;
    }

    void setName(const std::string& name)
    {
        name_ = name;
    }

private:
    std::string name_;
};

class TestNoDestroy
{
public:
    void no_destroy();

    TestNoDestroy()
    {
        std::cout << "tid=" << std::this_thread::get_id()
            << ", constructing TestNoDestroy\n\n";
    }

    ~TestNoDestroy()
    {
        std::cout << "tid=" << std::this_thread::get_id()
            << ", destructing TestNoDestroy\n\n";
    }
};

void threadFunc()
{
    std::cout << "tid=" << std::this_thread::get_id()
        << ", name=" << Singleton<Test>::instance().name()
        << std::endl << std::endl;
    Singleton<Test>::instance().setName("only one, changed");
}

int main(int argc, char* argv[])
{
    Singleton<Test>::instance().setName("only one");

    std::thread t1(threadFunc);

    t1.join();
    std::cout << "tid=" << std::this_thread::get_id()
        << ", name=" << Singleton<Test>::instance().name()
        << std::endl << std::endl;
    Singleton<TestNoDestroy>::instance();
    std::cout << "with valgrind, you should see " << sizeof(TestNoDestroy) << "-byte memory leak.\n\n";
    return 0;
}
