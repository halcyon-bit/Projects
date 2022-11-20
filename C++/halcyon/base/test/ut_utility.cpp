#define _BASE_TEST_
#include "base/utility/utility.h"

#include <map>
#include <iostream>
using namespace halcyon;

template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, std::string>::type toString(T& t)
{
    return std::to_string(t);
}

template<typename T>
typename std::enable_if<std::is_same<T, std::string>::value, std::string>::type toString(T& t)
{
    return t;
}


int main()
{
    std::cout << base::type_name<double>() << std::endl;
    std::cout << base::type_name<std::string>() << std::endl;
    std::cout << base::type_name<std::map<int, int>>() << std::endl;
    return 0;
}