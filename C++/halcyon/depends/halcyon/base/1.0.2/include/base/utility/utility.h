#pragma once

#include <base/common/base_define.h>

#include <utility>

#ifdef USE_CPP11

// C++11 以上会有的功能
namespace std
{
    // C++14
    template<typename T, typename Other = T>
    inline T exchange(T& val, Other&& newVal)
    {
        T oldVal = ::std::move(val);
        val = ::std::forward<Other>(newVal);
        return oldVal;
    }
}
#endif


#ifdef _BASE_TEST_

#include <string>
#include <memory>

BASE_BEGIN_NAMESPACE

template<typename T>
std::string type_name()
{
    using type = typename std::remove_reference<T>::type;
    std::unique_ptr<char, void(*)(void*)> own(
//#ifndef __GNUC__
        nullptr,
//#else
//        abi::__cxa_demangle(typeid(type).name(), nullptr, nullptr, nullptr),
//#endif
        std::free);
    std::string res = own != nullptr ? own.get() : typeid(type).name();
    if (std::is_const<type>::value)
        res += " const";
    if (std::is_volatile<type>::value)
        res += " volatile";
    if (std::is_lvalue_reference<T>::value)
        res += "&";
    else if (std::is_rvalue_reference<T>::value)
        res += "&&";
    return res;
}

BASE_END_NAMESPACE

#endif
