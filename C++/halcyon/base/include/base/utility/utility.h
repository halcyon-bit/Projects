#ifndef BASE_UTILITY_H
#define BASE_UTILITY_H

#include <base/utility/type.h>

#include <utility>
#include <stdexcept>

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

BASE_BEGIN_NAMESPACE
/////////////////////////////// invoke ///////////////////////////////
// C++17 有 invoke
#if defined USE_CPP11 || defined USE_CPP14
template<typename F, typename... Args>
inline std::enable_if_t<is_pointer_noref<F>::value, typename function_traits<std::decay_t<F>>::return_type> invoke(F&& f, Args&&... args)
{
    return (*std::forward<F>(f))(std::forward<Args>(args)...);
}

template<typename F, typename... Args>
inline std::enable_if_t<!is_pointer_noref<F>::value && !is_memfunc_noref<F>::value, typename function_traits<std::decay_t<F>>::return_type> invoke(F&& f, Args&&... args)
{
    return std::forward<F>(f)(std::forward<Args>(args)...);
}

template<typename F, typename C, typename... Args>
inline std::enable_if_t<is_memfunc_noref<F>::value && is_pointer_noref<C>::value, typename function_traits<std::decay_t<F>>::return_type> invoke(F&& f, C&& obj, Args&&... args)
{
    return (std::forward<C>(obj)->*std::forward<F>(f))(std::forward<Args>(args)...);
}

template<typename F, typename C, typename... Args>
inline std::enable_if_t<is_memfunc_noref<F>::value && !is_pointer_noref<C>::value, typename function_traits<std::decay_t<F>>::return_type> invoke(F&& f, C&& obj, Args... args)
{
    return (std::forward<C>(obj).*std::forward<F>(f))(std::forward<Args>(args)...);
}
#endif


/////////////////////////////// tuple helper ///////////////////////////////

/////////////////////////////// std::tuple 根据元素值获取索引位置
// find_tuple_index
namespace detail
{
    template<size_t N, typename Tuple, typename T>
    static typename std::enable_if<std::is_same<std::tuple_element_t<N, Tuple>, T>::value, bool>::type
        compare(const Tuple& t, const T& v)
    {
        return std::get<N>(t) == v;
    }
    template<size_t N, typename Tuple, typename T>
    static typename std::enable_if<!std::is_same<std::tuple_element_t<N, Tuple>, T>::value, bool>::type
        compare(const Tuple& t, const T& v)
    {
        return false;
    }

    template<size_t I, typename T, typename... Args>
    struct find_tuple_index_helper
    {
        static int find(const std::tuple<Args...>& t, T&& val)
        {
            using U = typename std::remove_reference<typename std::remove_cv<T>::type>::type;
            using V = typename std::tuple_element<I - 1, std::tuple<Args...>>::type;
            return (std::is_same<U, V>::value && compare<I - 1>(t, val)) ? I - 1
                : find_tuple_index_helper<I - 1, T, Args...>::find(t, std::forward<T>(val));
        }
    };

    template<typename T, typename... Args>
    struct find_tuple_index_helper<0, T, Args...>
    {
        static int find(const std::tuple<Args...>& t, T&& val)
        {
            using U = typename std::remove_reference<typename std::remove_cv<T>::type>::type;
            using V = typename std::tuple_element<0, std::tuple<Args...>>::type;
            return (std::is_convertible<U, V>::value && compare<0>(t, val)) ? 0 : -1;
        }
    };
}

/**
 * @brief       获取 val 在 tuple 中的索引位置
 * @param[in]   tuple
 * @param[in]   value, 必要时需要显示指明类型
 * @return      索引位置，没找到返回 -1
 */
template<typename T, typename... Args>
constexpr int find_tuple_index(const std::tuple<Args...>& t, T&& val)
{
    return detail::find_tuple_index_helper<sizeof...(Args), T, Args...>::find(t, std::forward<T>(val));
}


/////////////////////////////// 遍历 tuple
// tuple_for_each
namespace detail
{
    template<typename F, size_t... Indexes, typename... Args>
    constexpr void for_each_helper(F&& f, std::index_sequence<Indexes...>, const std::tuple<Args...>& t)
    {
#if defined USE_CPP11 || defined USE_CPP14
        int _[] = { (f(std::get<Indexes>(t)), 0)... };  // 逗号运算符，解包
#else
        (f(std::get<Indexes>(t)), ...);  // C++17 折叠表达式
#endif
    }

    template<typename F, size_t... Indexes, typename... Args>
    constexpr void for_each_helper(F&& f, std::index_sequence<Indexes...>, std::tuple<Args...>& t)
    {
#if defined USE_CPP11 || defined USE_CPP14
        int _[] = { (f(std::get<Indexes>(t)), 0)... };  // 逗号运算符，解包
#else
        (f(std::get<Indexes>(t)), ...);  // C++17 折叠表达式
#endif
    }
}

/**
 * @brief       遍历 tuple
 * @param[in]   函数，用于处理 tuple 内的每一个值，仅接受一个参数
 * @param[in]   tuple
 */
template<typename F, typename... Args>
constexpr typename std::enable_if_t<(sizeof...(Args) > 0)> tuple_for_each(F&& f, const std::tuple<Args...>& t)
{
    detail::for_each_helper(std::forward<F>(f), make_index_sequence_t<sizeof...(Args)>(), t);
}
template<typename F, typename... Args>
constexpr typename std::enable_if_t<(sizeof...(Args) > 0)> tuple_for_each(F&& f, std::tuple<Args...>& t)
{
    detail::for_each_helper(std::forward<F>(f), make_index_sequence_t<sizeof...(Args)>(), t);
}
template<typename F, typename... Args>
constexpr typename std::enable_if_t<(sizeof...(Args) == 0)> tuple_for_each(F&& f, const std::tuple<Args...>& t)
{}


/////////////////////////////// 反转 std::tuple
// reverse_tuple
namespace detail
{
    template<typename... Args, size_t... Indexes>
    auto reverse_tuple_impl(const std::tuple<Args...>& t, std::index_sequence<Indexes...>&&) -> decltype(std::make_tuple(std::get<Indexes>((t))...))
    {
        return std::make_tuple(std::get<Indexes>((t))...);
    }
}

/**
 * @brief       反转 tuple
 * @param[in]   原始 tuple
 * @return      反转后的 tuple
 */
template<typename... Args>
auto reverse_tuple(const std::tuple<Args...>& t) -> decltype(detail::reverse_tuple_impl((t), make_reverse_index_sequence_t<sizeof...(Args)>()))
{
    return detail::reverse_tuple_impl(t, make_reverse_index_sequence_t<sizeof...(Args)>());
}


/////////////////////////////// std::tuple 应用于函数，C++17 有该函数
// apply
#if defined USE_CPP11 || defined USE_CPP14
namespace detail
{
    template<typename F, typename Tuple, size_t... Indexes>
#ifdef USE_CPP11
    auto apply_helper(F&& f, Tuple&& t, index_sequence<Indexes...>) -> decltype(std::forward<F>(f)(std::get<Indexes>(std::forward<Tuple>(t))...))
#else
    decltype(auto) apply_helper(F&& f, Tuple&& t, std::index_sequence<Indexes...>)
#endif
    {
        return base::invoke(std::forward<F>(f), std::get<Indexes>(std::forward<Tuple>(t))...);
    }
}

/**
 * @brief       应用于函数，将 tuple 中的元素作为函数参数，进行调用
 * @param[in]   函数
 * @param[in]   tuple
 * @return      函数结果
 */
template<typename F, typename Tuple>
#ifdef USE_CPP11
auto apply(F&& f, Tuple&& t) -> decltype(detail::apply_helper((f), (t), base::make_index_sequence<std::tuple_size<typename std::remove_reference<Tuple>::type>::value>::type()))
#else
decltype(auto) apply(F&& f, Tuple&& t)
#endif
{
    return detail::apply_helper(std::forward<F>(f), std::forward<Tuple>(t), 
        make_index_sequence_t<std::tuple_size<std::remove_reference_t<Tuple>>::value>());
}
#endif


/////////////////////////////// 合并 tuple
template<size_t N, typename T1, typename T2>
using pair_type = std::pair<typename std::tuple_element<N, T1>::type, typename std::tuple_element<N, T2>::type>;

template<size_t N, typename T1, typename T2>
pair_type<N, T1, T2> pair(const T1& t1, const T2& t2)
{
    return std::make_pair(std::get<N>(t1), std::get<N>(t2));
}

template<size_t... Indexes, typename T1, typename T2>
auto pairs_helper(std::index_sequence<Indexes...>, const T1& t1, const T2& t2) -> decltype(std::make_tuple(pair<Indexes>(t1, t2)...))
{
    return std::make_tuple(pair<Indexes>(t1, t2)...);
}

template<typename... Args1, typename... Args2>
auto zip(const std::tuple<Args1...>& t1, const std::tuple<Args2...>& t2) -> decltype(pairs_helper(make_index_sequence_t<sizeof...(Args1)>(), t1, t2))
{
    static_assert(sizeof...(Args1) == sizeof...(Args2), "tuples should be the same size.");
    return pairs_helper(make_index_sequence_t<sizeof...(Args1)>(), t1, t2);
}

BASE_END_NAMESPACE


#ifdef _BASE_TEST_

#include <string>
#include <memory>

BASE_BEGIN_NAMESPACE

template<typename T>
std::string type_name()
{
    using type = typename std::remove_reference<T>::type;
    std::unique_ptr<char, void(*)(void*)> own(nullptr, std::free);
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

#endif