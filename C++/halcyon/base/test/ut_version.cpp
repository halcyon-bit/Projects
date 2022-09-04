#include "base/version.h"

#include <iostream>

using namespace halcyon;

int main(int argc, char* argv[])
{
    std::cout << "base version: " << base::version() << std::endl;
}
