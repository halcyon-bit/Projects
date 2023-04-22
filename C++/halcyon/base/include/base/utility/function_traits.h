#pragma once

#include <base/common/base_define.h>

#include <tuple>
#include <functional>

BASE_BEGIN_NAMESPACE

/**
 * @brief   获取函数的实际类型、返回类型、参数个数和参数的具体类型
 * @ps      通过模板特化支持
 */
template<typename T>
struct function_traits;

/// 普通函数
template<typename R, typename... Args>
struct function_traits<R(Args...)>
{
    // 参数个数
    enum
    {
        arity = sizeof...(Args)
    };
    using return_type = R;  // 返回类型
    using function_type = R(Args...);  // 函数类型
    using function_pointer = R(*)(Args...);

    // 参数类型
    template<size_t I>
    struct args
    {
        static_assert(I < arity, "index is out of range");
        using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
    };

    template<size_t I>
    using arg_type = typename args<I>::type;
};

/// 函数指针
template<typename R, typename... Args>
struct function_traits<R(*)(Args...)> : function_traits<R(Args...)>
{};

/// 函数对象
template<typename Callable>
struct function_traits : function_traits<decltype(&Callable::operator())>
{};

/// member function
template<typename R, typename C, typename... Args>
struct function_traits<R(C::*)(Args...)> : function_traits<R(Args...)>
{};

/// member function const
template<typename R, typename C, typename... Args>
struct function_traits<R(C::*)(Args...) const> : function_traits<R(Args...)>
{};

/// member function volatile
template<typename R, typename C, typename... Args>
struct function_traits<R(C::*)(Args...) volatile> : function_traits<R(Args...)>
{};

/// member function const volatile
template<typename R, typename C, typename... Args>
struct function_traits<R(C::*)(Args...) const volatile> : function_traits<R(Args...)>
{};

/// std::function
template<typename R, typename... Args>
struct function_traits<std::function<R(Args...)>> : function_traits<R(Args...)>
{};

BASE_END_NAMESPACE