#ifndef BASE_BIND_H
#define BASE_BIND_H

#include <base/utility/utility.h>

// 模拟 std::bind
#if defined USE_CPP11 || defined USE_CPP14

BASE_BEGIN_NAMESPACE

template<size_t I>
struct Placeholder
{};

Placeholder<1> _1; Placeholder<2> _2; Placeholder<3> _3; Placeholder<4> _4; Placeholder<5> _5;
Placeholder<6> _6; Placeholder<7> _7; Placeholder<8> _8; Placeholder<9> _9; Placeholder<10> _10;

template<typename T, typename Tuple>
inline auto select(T&& val, Tuple&) -> T&&
{
    return std::forward<T>(val);
}

template<size_t I, typename Tuple>
inline auto select(Placeholder<I>&, Tuple& t) -> decltype(std::get<I - 1>(t))
{
    return std::get<I - 1>(t);
}

template<typename F, typename... Args>
class Bind_t
{
    using function_type = std::decay_t<F>;
    using arg_type = std::tuple<std::decay_t<Args>...>;
    using return_type = typename function_traits<function_type>::return_type;

public:
    template<typename F1, typename... Args1>
    Bind_t(F1& f, Args1&... args)
        : func_(f)
        , args_(args...)
    {}

    template<typename F1, typename... Args1>
    Bind_t(F1&& f, Args1&&... args)
        : func_(std::forward<F1>(f))
        , args_(std::forward<Args1>(args)...)
    {}

    template<typename... Args1>
    return_type operator()(Args1&&... args)
    {
        return do_call(make_index_sequence_t<std::tuple_size<arg_type>::value>(),
            std::forward_as_tuple(std::forward<Args1>(args)...));
    }

private:
    template<typename Tuple, size_t... Indexes>
    return_type do_call(const std::index_sequence<Indexes...>&, Tuple&& args)
    {
        return base::invoke(func_, select(std::get<Indexes>(args_), args)...);
    }

private:
    function_type func_;
    arg_type args_;
};

template<typename F, typename... Args>
inline Bind_t<F, Args...> bind(F&& f, Args&&... args)
{
    using type = typename function_traits<F>::return_type;
    return Bind_t<F, Args...>(std::forward<F>(f), std::forward<Args>(args)...);
}

template<typename F, typename... Args>
inline Bind_t<F, Args...> bind(F& f, Args&... args)
{
    return Bind_t<F, Args...>(f, args...);
}

BASE_END_NAMESPACE

#endif

#endif