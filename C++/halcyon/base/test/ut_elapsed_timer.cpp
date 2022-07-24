#include "base/time/elapsed_timer.h"

#include <thread>
#include <iostream>

using namespace halcyon::base;
using std::chrono::seconds;

int main(int argc, char* argv[])
{
    std::cout << "sleep 5s" << std::endl;
    ElapsedTimer timer;
    std::this_thread::sleep_for(seconds(5));
    std::cout << "speed: " << timer.elapsed() << "ms" << std::endl;
    std::cout << timer.elapsedMicro() << std::endl;

    std::cout << "sleep 5s again" << std::endl;
    timer.reset();
    std::this_thread::sleep_for(seconds(5));
    std::cout << "speed: " << timer.elapsed() << "ms" << std::endl;
    std::cout << timer.elapsedNano() << std::endl;
    std::cout << timer.elapsedSecond() << std::endl;
    return 0;
}
