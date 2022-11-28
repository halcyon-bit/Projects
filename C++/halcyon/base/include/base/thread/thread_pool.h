#pragma once

#include <base/common/noncopyable.h>
#include <base/thread/thread_task.h>

#include <list>
#include <mutex>
#include <queue>
#include <thread>

BASE_BEGIN_NAMESPACE

/// 线程池(调整任务类型为 ThreadTask)
class HALCYON_BASE_API ThreadPool : noncopyable
{
public:
    /**
     * @brief   构造函数
     */
    explicit ThreadPool() noexcept;

    /**
     * @brief   析构函数
     */
    ~ThreadPool();

    /**
     * @brief       启动线程池
     * @param[in]   线程数量
     * @return      是否启动成功
     */
    bool start(int32_t num_threads = std::thread::hardware_concurrency());

    /**
     * @brief   获取正在处于等待状态的线程个数
     */
    size_t getWaitingThreadNum() const
    {
        return waiting_threads_.load(std::memory_order_acquire);
    }

    /**
     * @brief   获取线程池中当前线程的总个数
     */
    size_t getTotalThreadNum() const
    {
        return threads_.size();
    }

    /**
     * @brief       添加任务
     * @param[in]   任务函数
     * @param[in]   任务参数
     * @return      任务对象，失败返回 nullptr(例如线程池已经停止)
     */
    template <typename F, typename... Args>
    TaskSPtr addTask(F&& func, Args&&... args)
    {
        if (shutdown_.load(std::memory_order_acquire)) {
            // 处于关闭状态
            return nullptr;
        }

#if defined USE_CPP11 || defined USE_CPP14
        using return_type = std::result_of_t<F(Args...)>;
#else
        using return_type = std::invoke_result_t<F, Args...>;
#endif
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(func), std::forward<Args>(args)...));

        TaskSPtr result = std::make_shared<ThreadTask<return_type>>(
            [task]() { (*task)(); },
            task->get_future());
        {
            // 添加任务
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.emplace(result);
        }
        cv_.notify_one();
        return result;
    }

    /**
     * @brief   停止线程池，还没有执行的任务会继续执行
     *        直到任务全部执行完成
     */
    void shutDown()
    {
        shutDown(false);
    }

    /**
     * @brief   停止线程池，还没有执行的任务直接取消，不会再执行
     */
    void shutDownNow()
    {
        shutDown(true);
    }

private:
    /**
     * @brief       停止线程池
     * @param[in]   是否立即结束（不在运行剩余任务）
     */
    void shutDown(bool stop_now);

    /**
     * @brief   线程函数
     */
    void threadProc();

private:
    std::list<std::thread> threads_;  //! 所有的任务线程

    // 这里并没有用 BlockingQueue, 是因为其多线程退出不好处理
    std::mutex mutex_;  //! 任务队列锁
    std::queue<TaskSPtr> tasks_;  //! 任务队列
    std::condition_variable cv_;  //! 任务队列条件变量

    std::atomic<size_t> waiting_threads_{ 0 };  //! 处于等待状态的线程

    std::atomic<bool> shutdown_{ false };  //! 中止线程池(等待剩余任务运行完成)
    std::atomic<bool> shutdown_now_{ false };  //! 中止线程池(放弃任务的运行)
    bool started_{ false };  //! 是否启动
};

BASE_END_NAMESPACE
