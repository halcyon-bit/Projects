#pragma once

#include <base/thread/thread.h>

#if defined USE_CPP11 || defined USE_CPP14
#include <base/any/any.h>
#else
#include <any>
#endif
#include <unordered_map>

#ifndef WINDOWS
#define _atoi64(val)    strtoll(val, nullptr, 10)
#endif

BASE_BEGIN_NAMESPACE

#pragma warning(disable: 4477)

///  消息总线(包括同步、异步调用)
template<typename T = uint32_t>
class MessageBus : noncopyable
{
public:
    using MessageKey = T;  // 消息类型

private:  /// 通知数据
    struct NotifyValue
    {
#if defined USE_CPP11 || defined USE_CPP14
        Any func;  //! 函数
#else
        std::any func;  //! 函数
#endif
        uintptr_t identity{ 0 };  //! 唯一标识
        ThreadWPtr thd;  //! 执行线程
    };

public:  
    MessageBus()
    {
        thd_ = std::make_shared<Thread>();
    }

    ~MessageBus()
    {
        clearNotify();
        clear();
    }

public:  /// 通知相关接口(异步)，注意：通知一般是一对多的
    /**
     * @brief       注册通知(普通函数)
     * @param[in]   通知key
     * @param[in]   函数指针
     * @param[in]   调用通知函数的线程，若无，则使用 MessageBus 中的线程
     * @ps          调用者确保 genFunc != nullptr
     */
    template<typename R, typename... Args>
    void attachNotify(const MessageKey& msgKey, R(*genFunc)(Args...), ThreadSPtr thd = nullptr)
    {
        using notify_func_t = std::function<void(Args...)>;
        assert(genFunc != nullptr);
        // 通知函数
        auto func = notify_func_t([genFunc](Args... args) {
            genFunc(std::forward<Args>(args)...);
        });

        uintptr_t id = reinterpret_cast<uintptr_t>(genFunc);
        NotifyValue value{ func, id, thd == nullptr ? thd_ : thd };
        addNotify(msgKey, std::move(value));
    }

    /**
     * @brief       注册通知(成员函数)
     * @param[in]   通知key
     * @param[in]   对象指针
     * @param[in]   函数指针
     * @param[in]   调用通知函数的线程，若无，则使用 MessageBus 中的线程
     * @ps          调用者确保 memFunc != nullptr && obj != nullptr
     */
    template<typename C, typename R, typename... Args>
    void attachNotify(const MessageKey& msgKey, C* obj, R(C::*memFunc)(Args...), ThreadSPtr thd = nullptr)
    {
        using notify_func_t = std::function<void(Args...)>;
        assert(memFunc != nullptr && obj != nullptr);
        // 通知函数
        auto func = notify_func_t([obj, memFunc](Args... args) {
            (obj->*memFunc)(std::forward<Args>(args)...);
        });

        char buf[64]{ 0 };
        snprintf(buf, sizeof(buf), "%llu", memFunc);
        uintptr_t id = _atoi64(buf);
        NotifyValue value = { func, id, thd == nullptr ? thd_ : thd };
        addNotify(msgKey, std::move(value));
    }
    
    /**
     * @brief       反注册通知(普通函数)
     * @param[in]   通知key
     * @param[in]   函数指针
     * @ps          调用者确保 genFunc != nullptr
     */
    template<typename R, typename... Args>
    void detachNotify(const MessageKey& msgKey, R(*genFunc)(Args...))
    {
        uintptr_t identity = reinterpret_cast<uintptr_t>(genFunc);
        delNotify(msgKey, identity);
    }

    /**
     * @brief       反注册通知(成员函数)
     * @param[in]   通知key
     * @param[in]   对象指针
     * @param[in]   函数指针
     * @ps          调用者确保 memFunc != nullptr && obj != nullptr
     */
    template<typename C, typename R, typename... Args>
    void detachNotify(const MessageKey& msgKey, C* obj, R(C::*memFunc)(Args...))
    {
        char buf[64]{ 0 };
        snprintf(buf, sizeof(buf), "%llu", memFunc);
        uintptr_t identity = _atoi64(buf);
        delNotify(msgKey, identity);
    }

    /**
     * @brief       反注册某个通知（清除该通知下的所有函数）
     * @param[in]   通知key
     */
    void detachNotify(const MessageKey& msgKey)
    {
        std::lock_guard<std::mutex> lock(notify_mutex_);
        auto range = notify_.equal_range(msgKey);
        notify_.erase(range.first, range.second);
    }

    /**
     * @brief       通知消息
     * @param[in]   通知key
     * @param[in]   消息所需参数
     */
    template<typename... Args>
    void notify(const MessageKey& msgKey, Args... args)
    {
        using notify_func_t = std::function<void(Args...)>;
        std::lock_guard<std::mutex> lock(notify_mutex_);
        auto range = notify_.equal_range(msgKey);
        for (auto iter = range.first; iter != range.second; ++iter) {
            NotifyValue& value = iter->second;
            auto thd = value.thd.lock();
            if (thd == nullptr) {
                // 无处理线程
                continue;
            }
#if defined USE_CPP11 || defined USE_CPP14
            auto func = value.func.anyCast<notify_func_t>();
#else
            auto func = std::any_cast<notify_func_t>(value.func);
#endif
            thd->run(std::bind(std::move(func), std::forward<Args>(args)...));
        }
    }

    /**
     * @brief   清除所有通知信息
     */
    void clearNotify()
    {
        std::lock_guard<std::mutex> lock(notify_mutex_);
        notify_.clear();
    }

public:  /// 事务相关调用(同步)，注意：事务是一对一的
    /**
     * @brief       注册事务(普通函数)
     * @param[in]   事务key
     * @param[in]   函数指针
     * @ps          调用者确保 genFunc != nullptr
     */
    template<typename R, typename... Args>
    void attach(const MessageKey& msgKey, R(*genFunc)(Args...))
    {
        using affair_func_t = std::function<R(Args...)>;
        assert(genFunc != nullptr);
        auto func = affair_func_t([genFunc](Args... args) -> R {
            return genFunc(std::forward<Args>(args)...);
        });
        {
            std::lock_guard<std::mutex> lock(affair_mutex_);
            affair_[msgKey] = func;
        }
    }

    /**
     * @brief       注册事务(成员函数)
     * @param[in]   事务key
     * @param[in]   对象指针
     * @param[in]   函数指针
     * @ps          调用者确保 memFunc != nullptr && obj != nullptr
     */
    template<typename C, typename R, typename... Args>
    void attach(const MessageKey& msgKey, C* obj, R(C::*memFunc)(Args...))
    {
        using affair_func_t = std::function<R(Args...)>;
        assert(memFunc != nullptr && obj != nullptr);
        auto func = affair_func_t([obj, memFunc](Args... args) -> R {
            return (obj->*memFunc)(std::forward<Args>(args)...);
        });
        {
            std::lock_guard<std::mutex> lock(affair_mutex_);
            affair_[msgKey] = func;
        }
    }

    /**
     * @brief       反注册某个事务
     * @param[in]   通知key
     */
    void detach(const MessageKey& msgKey)
    {
        std::lock_guard<std::mutex> lock(affair_mutex_);
        affair_.erase(msgKey);
    }

    /**
     * @brief       调用某个事务(同步)
     * @param[in]   事务key
     * @param[in]   参数
     * @return      调用结果
     * @ps          调用者确保 msgKey 已经 attach
     */
    template<typename R, typename... Args>
    R runSync(const MessageKey& msgKey, Args... args)
    {
        using affair_func_t = std::function<R(Args...)>;
        affair_func_t func;
        {
            std::lock_guard<std::mutex> lock(affair_mutex_);
            auto iter = affair_.find(msgKey);
            assert(iter != affair_.end());
#if defined USE_CPP11 || defined USE_CPP14
            // 使用 gcc 编译有问题?
            // func = iter->second.anyCast<function_type>();
            Any& any = iter->second;
            func = any.anyCast<affair_func_t>();
#else
            func = std::any_cast<affair_func_t>(iter->second);
#endif
        }
        return func(std::forward<Args>(args)...);
    }

    /**
     * @brief   清空所有事务
     */
    void clear()
    {
        std::lock_guard<std::mutex> lock(affair_mutex_);
        affair_.clear();
    }

private:
    /**
     * @brief   添加通知
     */
    void addNotify(const MessageKey& notifyKey, NotifyValue&& notifyValue)
    {
        std::lock_guard<std::mutex> lock(notify_mutex_);
        notify_.emplace(std::move(notifyKey), std::forward<NotifyValue>(notifyValue));
    }

    /**
     * @brief   删除通知
     */
    void delNotify(const MessageKey& notifyKey, uintptr_t identity)
    {
        std::lock_guard<std::mutex> lock(notify_mutex_);
        auto range = notify_.equal_range(notifyKey);
        for (auto iter = range.first; iter != range.second; ++iter) {
            NotifyValue& value = iter->second;
            if (value.identity == identity) {
                notify_.erase(iter);
                break;
            }
        }
    }

private:  /// 通知数据
    ThreadSPtr thd_;  //! 调用消息函数的线程
    mutable std::mutex notify_mutex_;  //! for notify_
    std::unordered_multimap<MessageKey, NotifyValue> notify_;  //! 通知表

private:  /// 事务数据
    mutable std::mutex affair_mutex_;  //! for affair_
#if defined USE_CPP11 || defined USE_CPP14
    std::unordered_map<MessageKey, Any> affair_;  //! 事务表
#else
    std::unordered_map<MessageKey, std::any> affair_;  //! 事务表
#endif
};

#pragma warning(default: 4477)

BASE_END_NAMESPACE