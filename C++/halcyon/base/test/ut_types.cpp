#include "base/utility/types.h"

#include <string>
#include <iostream>

using namespace halcyon;

template<typename... Args>
struct TestA
{
    size_t type_size = halcyon::base::integer_max<sizeof(Args)...>::value;
};

int main()
{
    // 迭代器判断
    std::cout << "char is iterator: " << base::is_iterator<char>::value << std::endl;
    std::cout << "char* is iterator: " << base::is_iterator<char*>::value << std::endl;

    std::cout << "std::string::iterator is iterator: " << base::is_iterator<std::string::iterator>::value << std::endl;
    std::cout << "std::string::const_iterator is iterator: " << base::is_iterator<std::string::const_iterator>::value << std::endl;


    std::cout << std::endl << std::endl;
    // 获取整数序列中最大值
    TestA<int, double> t;

    std::cout << "<int, double> max size: " << t.type_size << std::endl;

    std::cout << "<1, 2, 3, 10, 20, 4, 2, 5, 99> max value: " << halcyon::base::integer_max<1, 2, 3, 10, 20, 4, 2, 5, 99>::value << std::endl;

    return 0;
}