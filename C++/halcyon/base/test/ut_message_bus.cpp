#include "base/time/timestamp.h"
#include "base/singleton/singleton.h"
#include "base/message/message_bus.h"
#include "fmt/format.h"

#include <string>
#include <sstream>
#include <iostream>

using namespace halcyon;

#define MSG_ATTACH_NOTIFY   base::Singleton<base::MessageBus<>>::instance().attachNotify
#define MSG_DETACH_NOTIFY   base::Singleton<base::MessageBus<>>::instance().detachNotify
#define MSG_NOTIFY          base::Singleton<base::MessageBus<>>::instance().notify
#define MSG_ATTACH          base::Singleton<base::MessageBus<>>::instance().attach
#define MSG_DETACH          base::Singleton<base::MessageBus<>>::instance().detach
#define MSG_SYNC            base::Singleton<base::MessageBus<>>::instance().runSync

enum {
    NOTIFY_1,
    NOTIFY_2,
    NOTIFY_3,
};

enum {
    AFFAIRS_1,
    AFFAIRS_2,
    AFFAIRS_3,
};

std::string getThreadId()
{
    std::ostringstream oss;
    oss << std::this_thread::get_id();
    return oss.str();
}

class TestX
{
public:
    TestX()
    {
        worker_ = std::make_shared<base::Thread>();
    }

    void init()
    {
        MSG_ATTACH(AFFAIRS_1, this, &TestX::pascalTriangle);
        MSG_ATTACH_NOTIFY(NOTIFY_1, this, &TestX::dealNotify1);
        MSG_ATTACH_NOTIFY(NOTIFY_2, this, &TestX::dealNotify2, worker_);
        MSG_ATTACH_NOTIFY(NOTIFY_3, this, &TestX::dealNotify3, worker_);
    }

    void uninit()
    {
        MSG_DETACH(AFFAIRS_1);
        MSG_DETACH_NOTIFY(NOTIFY_1, this, &TestX::dealNotify1);
        MSG_DETACH_NOTIFY(NOTIFY_2, this, &TestX::dealNotify2);
        MSG_DETACH_NOTIFY(NOTIFY_3, this, &TestX::dealNotify3);

        if (worker_) {
            worker_->join();
        }
    }

    int32_t pascalTriangle(int32_t n)
    {
        std::string info;  // 防止控制台输出混乱
        int32_t a[35][35];
        int32_t i, j;
        for (i = 1; i <= n; i++) {
            info.append("\t");
            for (j = 1; j <= i; j++) {
                if (i == j || j == 1) {
                    a[i][j] = 1;
                }
                else {
                    a[i][j] = a[i - 1][j] + a[i - 1][j - 1];
                }
                info.append(" ").append(std::to_string(a[i][j]));
            }
            info.append("\n");
        }
        std::cout << info;
        return 1000;
    }

    void dealNotify1(const std::string& str)
    {
        std::string info = fmt::format("\trecv notify1 in TestX::dealNotify1, threadId: {}, info: {}\n\n", getThreadId(), str);
        std::cout << info;
    }

    void dealNotify2(const std::string& str, int32_t n)
    {
        std::string info = fmt::format("\trecv notify2 in TestX::dealNotify2, threadId: {}, info: {}, {}\n\n", getThreadId(), str, n);
        std::cout << info;
    }

    void dealNotify3(const std::string& str, int32_t n, double d)
    {
        std::string info = fmt::format("\trecv notify3 in TestX::dealNotify3, threadId: {}, info: {}, {}, {}\n\n", getThreadId(), str, n, d);
        std::cout << info;
    }

    void notify1()
    {
        const std::string str{ "notify_1" };
        MSG_NOTIFY<const std::string&>(NOTIFY_1, str);
    }

    void notify2()
    {
        const std::string str{ "notify_2" };
        const int32_t i{ 2 };
        MSG_NOTIFY<const std::string&, int32_t>(NOTIFY_2, str, i);
    }

    void notify3()
    {
        const std::string str{ "notify_3" };
        MSG_NOTIFY<const std::string&, int32_t, double>(NOTIFY_3, str, 3, 3.3);
    }

private:
    base::ThreadSPtr worker_;
};

int32_t globalDealNoitfy1(const std::string& str)
{
    std::string info = fmt::format("\trecv notify1 in globalDealNoitfy1, threadId: {}, info: {}\n\n", getThreadId(), str);
    std::cout << info;
    return 100;
}

void globalDealNoitfy2(const std::string& str, int32_t n)
{
    std::string info = fmt::format("\trecv notify2 in globalDealNoitfy2, threadId: {}, info: {}, {}\n\n", getThreadId(), str, n);
    std::cout << info;
}

void globalDealNoitfy3(const std::string& str, int32_t n, double d)
{
    std::string info = fmt::format("\trecv notify3 in globalDealNoitfy3, threadId: {}, info: {}, {}, {}\n\n", getThreadId(), str, n, d);
    std::cout << info;
}

int32_t fib(int32_t n)
{
    if (n <= 0)
        return 0;
    else if (n == 1)
        return 1;
    else
        return fib(n - 1) + fib(n - 2);
}

int main(int argc, char* argv[])
{
    std::cout << "test affair:\n";
    std::cout << "\tattach affair_2\n";
    MSG_ATTACH(AFFAIRS_2, &fib);

    std::cout << "\tcall affair_2\n";
    int32_t ff(20);
    int32_t res = MSG_SYNC<int32_t, int32_t>(AFFAIRS_2, ff);
    std::string info = fmt::format("\tfib(20): {}\n", res);
    std::cout << info;

    std::cout << "\n\tdetach affair_2\n";
    //MSG_DETACH(AFFAIRS_2);
    std::cout << "\tcall affair_2\n";
    try
    {
        res = MSG_SYNC<int32_t, int32_t>(AFFAIRS_2, ff);
    }
    catch (...)
    {
        std::cout << "\texception\n";
    }

    std::cout << std::endl;

    {
        std::cout << "\tattach affair_1(in class)\n";
        TestX t;
        t.init();
        std::cout << "\tcall affair_1\n";
        std::cout << "\tPascalTriangle(10):\n";
        MSG_SYNC<int32_t, int32_t>(AFFAIRS_1, 10);
        t.uninit();
    }

    std::cout << "\n\ntest notify:\n";

    MSG_ATTACH_NOTIFY(NOTIFY_1, &globalDealNoitfy1);
    MSG_ATTACH_NOTIFY(NOTIFY_2, &globalDealNoitfy2);
    MSG_ATTACH_NOTIFY(NOTIFY_3, &globalDealNoitfy3);
    
    {
        TestX tt;
        tt.init();
        std::cout << "\tnotify1\n";
        tt.notify1();
        std::cout << "\tnotify2\n";
        tt.notify2();
        std::cout << "\tnotify3\n";
        tt.notify3();
        tt.uninit();
    }

    base::sleep(1000);
    std::cout << "\n\tdetach all notify in TestX\n";
    std::cout << "\tnotify1\n";
    MSG_NOTIFY<const std::string&>(NOTIFY_1, "notify_1");
    std::cout << "\tnotify2\n";
    MSG_NOTIFY<const std::string&, int32_t>(NOTIFY_2, "notify_2", 9);
    std::cout << "\tnotify3\n";
    MSG_NOTIFY<const std::string&, int32_t, double>(NOTIFY_3, "notify_3", 9, 9.9);

    base::sleep(1000);
    std::cout << "\n\tdetach all notify\n";
    MSG_DETACH_NOTIFY(NOTIFY_1, &globalDealNoitfy1);
    MSG_DETACH_NOTIFY(NOTIFY_2, &globalDealNoitfy2);
    MSG_DETACH_NOTIFY(NOTIFY_3, &globalDealNoitfy3);

    std::cout << "\tnotify1\n";
    MSG_NOTIFY<const std::string&>(NOTIFY_1, "notify_1");
    std::cout << "\tnotify2\n";
    MSG_NOTIFY<const std::string&, int32_t>(NOTIFY_2, "notify_2", 12);
    std::cout << "\tnotify3\n";
    MSG_NOTIFY<const std::string&, int32_t, double>(NOTIFY_3, "notify_3", 12, 12.12);
    return 0;
}
