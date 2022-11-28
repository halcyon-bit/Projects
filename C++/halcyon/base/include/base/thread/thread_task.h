#pragma once

#include <base/task/task.h>

#include <future>
#include <functional>

BASE_BEGIN_NAMESPACE

/// 线程任务
template<typename T>
class ThreadTask final : public Task
{
public:
    ThreadTask(std::function<void()>&& func, std::future<T>&& result)
        : func_(std::move(func))
        , result_(std::move(result))
    {}
    ~ThreadTask() = default;

public:
#if defined USE_CPP11 || defined USE_CPP14
    bool result(Any& value, uint64_t timeout = 0) override
#else
    bool result(std::any& value, uint64_t timeout = 0) override
#endif
    {
        if (timeout == 0) {
            resultAux(value, std::is_same<T, void>());
            //value = result_.get();
            return true;
        }
        std::chrono::milliseconds span(timeout);
        if (result_.wait_for(span) == std::future_status::timeout)
            return false;
        else {
            resultAux(value, std::is_same<T, void>());
            //value = result_.get();
            return true;
        }
    }

private:
#if defined USE_CPP11 || defined USE_CPP14
    void resultAux(Any& value, std::true_type)
#else
    void resultAux(std::any& value, std::true_type)
#endif
    {}

#if defined USE_CPP11 || defined USE_CPP14
    void resultAux(Any& value, std::false_type)
#else
    void resultAux(std::any& value, std::false_type)
#endif
    {
        value = result_.get();
    }

private:
    /**
     * @brief   运行任务
     */
    void run() override
    {
        if (!setRunning())  // 设置运行态
            return;
        func_();
        setFinished();  // 设置完成态
        callback_(this);  // 通知结果
    }

private:
    std::function<void()> func_;  //! 任务函数
    std::future<T> result_;  //! 任务结果
};

BASE_END_NAMESPACE