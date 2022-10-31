#pragma once

#include <base/optional/optional.h>

#include <functional>
#include <type_traits>

BASE_BEGIN_NAMESPACE

/// 惰性求值
template<typename T>
class Lazy
{
public:
    Lazy()
    {}

    template<typename F, typename... Args>
    Lazy(F&& f, Args&&... args)
    {
        func_ = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    }

    T& value()
    {
        if (!value_.isInit()) {
            value_ = func_();
        }
        return *value_;
    }

    bool hasValue()
    {
        return value_.isInit();
    }
private:
    std::function<T()> func_;
    Optional<T> value_;
};

/// 辅助函数，创建 Lazy
#if defined USE_CPP11 || defined USE_CPP14
template<typename F, typename... Args>
Lazy<typename std::result_of<F(Args...)>::type> lazy(F&& f, Args&&... args)
{
    return Lazy<typename std::result_of<F(Args...)>::type>(std::forward<F>(f), std::forward<Args>(args)...);
}
#else
template<typename F, typename... Args>
Lazy<std::invoke_result_t<F, Args...>> lazy(F&& f, Args&&... args)
{
    return Lazy<std::invoke_result_t<F, Args...>>(std::forward<F>(f), std::forward<Args>(args)...);
}
#endif

BASE_END_NAMESPACE