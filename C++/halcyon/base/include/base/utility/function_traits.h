#pragma once

#include <base/common/base_define.h>

#include <tuple>
#include <functional>

BASE_BEGIN_NAMESPACE

/**
 * @brief   ��ȡ������ʵ�����͡��������͡����������Ͳ����ľ�������
 * @ps      ͨ��ģ���ػ�֧��
 */
template<typename T>
struct function_traits;

/// ��ͨ����
template<typename R, typename... Args>
struct function_traits<R(Args...)>
{
    // ��������
    enum
    {
        arity = sizeof...(Args)
    };
    using return_type = R;  // ��������
    using function_type = R(Args...);  // ��������
    using function_pointer = R(*)(Args...);

    // ��������
    template<size_t I>
    struct args
    {
        static_assert(I < arity, "index is out of range");
        using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
    };

    template<size_t I>
    using arg_type = typename args<I>::type;
};

/// ����ָ��
template<typename R, typename... Args>
struct function_traits<R(*)(Args...)> : function_traits<R(Args...)>
{};

/// ��������
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