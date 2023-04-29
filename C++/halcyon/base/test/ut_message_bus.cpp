#include "base/message/message_bus.h"
#include "base/time/timestamp.h"
#include "base/singleton/singleton.h"

#include <string>
#include <sstream>
#include <iostream>

using namespace halcyon;

base::MessageBus<>& msgBus = base::Singleton<base::MessageBus<>>::instance();

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
        thd_ = std::make_shared<base::Thread>();
    }

    void init()
    {
        msgBus.attach(AFFAIRS_1, this, &TestX::pascalTriangle);
        msgBus.attachNotify(NOTIFY_1, this, &TestX::dealNotify1);
        msgBus.attachNotify(NOTIFY_2, this, &TestX::dealNotify2, thd_);
        msgBus.attachNotify(NOTIFY_3, this, &TestX::dealNotify3, thd_);
    }

    void uninit()
    {
        msgBus.detach(AFFAIRS_1);
        msgBus.detachNotify(NOTIFY_1, this, &TestX::dealNotify1);
        msgBus.detachNotify(NOTIFY_2, this, &TestX::dealNotify2);
        msgBus.detachNotify(NOTIFY_3, this, &TestX::dealNotify3);

        thd_->join();
    }

    int32_t pascalTriangle(int32_t n) const 
    {
        std::string info;
        int32_t a[35][35];
        int32_t i, j;
        for (i = 1; i <= n; i++) {
            info.append("\t");
            for (j = 1; j <= i; j++) {
                if (i == j || j == 1) {
                    a[i][j] = 1;
                } else {
                    a[i][j] = a[i - 1][j] + a[i - 1][j - 1];
                }
                info.append(" ").append(std::to_string(a[i][j]));
            }
            info.append("\n");
        }
        std::cout << info;
        return 1000;
    }

    void dealNotify1(const std::string& str) const
    {
        std::cout << "\trecv notify1 in TestX::dealNotify1, threadId: " 
            << std::this_thread::get_id() << ", info: " << str << "\n\n";
    }

    void dealNotify2(const std::string& str, int32_t n) volatile
    {
        std::cout << "\trecv notify2 in TestX::dealNotify2, threadId: " 
            << std::this_thread::get_id() << ", info: " << str << ", " << n << "\n\n";
    }

    void dealNotify3(const std::string& str, int32_t n, double d)
    {
        std::cout << "\trecv notify3 in TestX::dealNotify3, threadId: "
            << std::this_thread::get_id() << ", info: " << str << ", "
            << n << ", " << d << "\n\n";
    }

    void notify1()
    {
        const std::string str{ "notify_1" };
        msgBus.notify<const std::string&>(NOTIFY_1, str);
    }

    void notify2()
    {
        const std::string str{ "notify_2" };
        const int32_t i{ 2 };
        msgBus.notify<const std::string&, int32_t>(NOTIFY_2, str, i);
    }

    void notify3()
    {
        const std::string str{ "notify_3" };
        msgBus.notify<const std::string&, int32_t, double>(NOTIFY_3, str, 3, 3.3);
    }

private:
    base::ThreadSPtr thd_;
};

int32_t globalDealNoitfy1(const std::string& str)
{
    std::cout << "\trecv notify1 in globalDealNoitfy1, threadId: "
        << std::this_thread::get_id() << ", info: " << str << "\n\n";
    return str.size();
}

static void globalDealNoitfy2(const std::string& str, int32_t n)
{
    std::cout << "\trecv notify2 in globalDealNoitfy2, threadId: "
        << std::this_thread::get_id() << ", info: " << str << ", " << n << "\n\n";
}

void globalDealNoitfy3(const std::string& str, int32_t n, double d)
{
    std::cout << "\trecv notify3 in globalDealNoitfy3, threadId: "
        << std::this_thread::get_id() << ", info: " << str << ", "
        << n << ", " << d << "\n\n";
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
    msgBus.attach(AFFAIRS_2, &fib);

    std::cout << "\tcall affair_2\n";
    int32_t ff(20);
    int32_t res = msgBus.runSync<int32_t, int32_t>(AFFAIRS_2, ff);
    std::cout << "\tfib(20): " << res << std::endl;

    std::cout << "\n\tdetach affair_2\n";
    //MSG_DETACH(AFFAIRS_2);
    std::cout << "\tcall affair_2\n";
    try
    {
        res = msgBus.runSync<int32_t, int32_t>(AFFAIRS_2, ff);
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
        msgBus.runSync<int32_t, int32_t>(AFFAIRS_1, 10);
        t.uninit();
    }

    std::cout << "\n\ntest notify:\n";

    msgBus.attachNotify(NOTIFY_1, &globalDealNoitfy1);
    msgBus.attachNotify(NOTIFY_2, &globalDealNoitfy2);
    msgBus.attachNotify(NOTIFY_3, &globalDealNoitfy3);
    
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
    msgBus.notify<const std::string&>(NOTIFY_1, "notify_1");
    std::cout << "\tnotify2\n";
    msgBus.notify<const std::string&, int32_t>(NOTIFY_2, "notify_2", 9);
    std::cout << "\tnotify3\n";
    msgBus.notify<const std::string&, int32_t, double>(NOTIFY_3, "notify_3", 9, 9.9);

    base::sleep(1000);
    std::cout << "\n\tdetach all notify\n";
    msgBus.detachNotify(NOTIFY_1, &globalDealNoitfy1);
    msgBus.detachNotify(NOTIFY_2, &globalDealNoitfy2);
    msgBus.detachNotify(NOTIFY_3, &globalDealNoitfy3);

    std::cout << "\tnotify1\n";
    msgBus.notify<const std::string&>(NOTIFY_1, "notify_1");
    std::cout << "\tnotify2\n";
    msgBus.notify<const std::string&, int32_t>(NOTIFY_2, "notify_2", 12);
    std::cout << "\tnotify3\n";
    msgBus.notify<const std::string&, int32_t, double>(NOTIFY_3, "notify_3", 12, 12.12);
    return 0;
}
