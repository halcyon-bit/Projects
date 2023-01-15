#include "base/time/timestamp.h"

#include <mutex>
#include <thread>
#include <iostream>
#include <condition_variable>

using namespace halcyon;

int main(int argc, char* argv[])
{
    base::Timestamp time = base::now();
    std::cout << "time now: " << base::toFormatString(time) << std::endl;
    std::cout << "time now: " << base::toString(time) << std::endl;

    std::cout << "waiting 10s\n";
    base::sleep(10 * 1000);

    time = base::now();
    std::cout << "time now: " << base::toFormatString(time) << std::endl;

    std::cout << "microSecond: " << base::microSecondsSinceEpoch(time) << std::endl;
    std::cout << "second: " << base::secondsSinceEpoch(time) << std::endl;

    std::cout << "add 60s\n";
    base::Timestamp ntime = base::addTime(time, 60 * 1000);
    std::cout << "time: " << base::toFormatString(ntime) << std::endl;

    std::cout << "dec 30s\n";
    ntime = base::decTime(ntime, 30 * 1000);
    std::cout << "time: " << base::toFormatString(ntime) << std::endl;

    std::cout << "time difference\n";
    std::cout << base::timeDifference(ntime, time) << std::endl;

    time = base::now();
    std::cout << "time now: " << base::toFormatString(time) << std::endl;
    time = base::addTime(base::now(), 5000);

    std::mutex mx;
    std::condition_variable cv;
    std::unique_lock<std::mutex> lock(mx);
    cv.wait_until(lock, time);
    time = base::now();
    std::cout << "time now: " << base::toFormatString(time) << std::endl;
    
    return 0;
}
