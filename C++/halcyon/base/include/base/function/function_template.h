#include <base/utility/type.h>
#include <base/utility/utility.h>  // std::exchange

#include <memory>
#include <stdexcept>

#define DD_JOIN(a, b)       DD_DO_JOIN(a, b)
#define DD_DO_JOIN(a, b)    a ## b

#define FUNCTION_NAME   DD_JOIN(Function, FUNCTION_NUMBER)

#if FUNCTION_NUMBER == 0
#  define FUNCTION_COMMA
#else
#  define FUNCTION_COMMA ,
#endif

#define PTR_MEM_FUNCTION_INVOKER    DD_JOIN(smemFuncInvoker, FUNCTION_NUMBER)
#define MEM_FUNCTION_INVOKER        DD_JOIN(memFuncInvoker, FUNCTION_NUMBER)
#define GEN_FUNCTION_INVOKER        DD_JOIN(genFuncInvoker, FUNCTION_NUMBER)

BASE_BEGIN_NAMESPACE

/**
 * @brief   类的成员函数, 类对象为 std::shared_ptr<x> 指针
 *          模板参数：返回值是否为空
 */
template<bool>
struct PTR_MEM_FUNCTION_INVOKER
{
    template<FUNCTION_CLASS_TYPES>
    struct inner
    {
        // C 为某个类，F 为类的成员函数
        template<typename C, typename F>
        static R invoke(std::shared_ptr<C>& obj, F f FUNCTION_COMMA FUNCTION_TYPE_ARGS)
        {
            return (obj.get()->*f)(FUNCTION_ARGS);
        }
    };
};

// 无返回值
template<>
struct PTR_MEM_FUNCTION_INVOKER<true>
{
    template<FUNCTION_CLASS_TYPES>
    struct inner
    {
        template<typename C, typename F>
        static R invoke(std::shared_ptr<C>& obj, F f FUNCTION_COMMA FUNCTION_TYPE_ARGS)
        {
            (obj.get()->*f)(FUNCTION_ARGS);
        }
    };
};

/*
 * @brief   类的成员函数, 类对象为 X, 普通对象
 *          模板参数：返回值是否为空
 */
template<bool>
struct MEM_FUNCTION_INVOKER
{
    template<FUNCTION_CLASS_TYPES>
    struct inner
    {
        template<typename C, typename F>
        static R invoke(C* obj, F f FUNCTION_COMMA FUNCTION_TYPE_ARGS)
        {
            return (obj->*f)(FUNCTION_ARGS);
        }
    };
};

template<>
struct MEM_FUNCTION_INVOKER<true>
{
    template<FUNCTION_CLASS_TYPES>
    struct inner
    {
        template<typename C, typename F>
        static R invoke(C* obj, F f FUNCTION_COMMA FUNCTION_TYPE_ARGS)
        {
            (obj->*f)(FUNCTION_ARGS);
        }
    };
};

/*
 * @brief   普通函数
 *          模板参数：返回值是否为空
 */
template<bool>
struct GEN_FUNCTION_INVOKER
{
    template<FUNCTION_CLASS_TYPES>
    struct inner
    {
        template<typename F>
        static R invoke(F f FUNCTION_COMMA FUNCTION_TYPE_ARGS)
        {
            return f(FUNCTION_ARGS);
        }
    };
};

template<>
struct GEN_FUNCTION_INVOKER<true>
{
    template<FUNCTION_CLASS_TYPES>
    struct inner
    {
        template<typename F>
        static R invoke(F f FUNCTION_COMMA FUNCTION_TYPE_ARGS)
        {
            f(FUNCTION_ARGS);
        }
    };
};

/*
 * @brief   类似于 std::function 的函数封装
 */
template<FUNCTION_CLASS_TYPES>
class FUNCTION_NAME
{
    class X {};

    //! 类的成员函数
    using MEM_FUNCTION = R(X::*)(FUNCTION_TYPES);

    //! 普通函数
    using GEN_FUNCTION = R(*)(FUNCTION_TYPES);

    enum {
        emPMemFunc,  // 类的成员函数，shared_ptr 指针(类对象)
        emMemFunc,  // 类的成员函数，普通类对象
        emGenFunc,  // 普通函数
        emInvalidFunc
    };

public:
    /**
     * @brief   默认构造函数
     */
    FUNCTION_NAME()
        : type_(emInvalidFunc), object_(nullptr)
    {
        func_.mem_func = nullptr;
    }

    /**
     * @brief       构造函数
     * @param[in]   类的成员函数
     * @param[in]   类对象(共享)
     */
    template<typename T>
    FUNCTION_NAME(R(T::*f)(FUNCTION_TYPES), const std::shared_ptr<T>& object)
        : type_(emPMemFunc), object_(nullptr)
        , sptr_(object, base::horrible_cast<X*, T*>(object.get()))
    {
        func_.mem_func = base::horrible_cast<MEM_FUNCTION>(f);
    }

    /**
     * @brief       构造函数
     * @param[in]   类的成员函数
     * @param[in]   类对象
     */
    template<typename T>
    FUNCTION_NAME(R(T::*f)(FUNCTION_TYPES), T* object)
        : type_(emMemFunc)
        , object_(base::horrible_cast<X*, T*>(object))
    {
        func_.mem_func = base::horrible_cast<MEM_FUNCTION>(f);
    }

    /**
     * @brief       构造函数
     * @param[in]   普通函数
     */
    FUNCTION_NAME(R(*f)(FUNCTION_TYPES))
        : type_(emGenFunc)
        , object_(nullptr)
    {
        func_.gen_func = base::horrible_cast<GEN_FUNCTION>(f);
    }

    /**
     * @brief   拷贝构造
     */
    FUNCTION_NAME(const FUNCTION_NAME& rhs)
        : type_(rhs.type_)
        , object_(nullptr)
    {
        if (emPMemFunc == type_) {
            sptr_ = rhs.sptr_;
            func_.mem_func = rhs.func_.mem_func;
        }
        else if (emMemFunc == type_) {
            object_ = rhs.object_;
            func_.mem_func = rhs.func_.mem_func;
        }
        else if (emGenFunc == type_) {
            func_.gen_func = rhs.func_.gen_func;
        }
    }

    /**
     * @brief   移动构造
     */
    FUNCTION_NAME(FUNCTION_NAME&& rhs) noexcept
        : object_(nullptr)
    {
        type_ = std::exchange(rhs.type_, emInvalidFunc);
        if (emPMemFunc == type_) {
            sptr_ = std::exchange(rhs.sptr_, nullptr);
            func_.mem_func = std::exchange(rhs.func_.mem_func, nullptr);
        }
        else if (emMemFunc == type_) {
            object_ = std::exchange(rhs.object_, nullptr);
            func_.mem_func = std::exchange(rhs.func_.mem_func, nullptr);
        }
        else if (emGenFunc == type_) {
            func_.gen_func = std::exchange(rhs.func_.gen_func, nullptr);
        }
    }

    /**
     * @brief   拷贝赋值操作符
     */
    FUNCTION_NAME& operator=(const FUNCTION_NAME& rhs)
    {
        if (this != &rhs) {
            type_ = rhs.type_;
            if (emPMemFunc == type_) {
                sptr_ = rhs.sptr_;
                func_.mem_func = rhs.func_.mem_func;
            }
            else if (emMemFunc == type_) {
                object_ = rhs.object_;
                func_.mem_func = rhs.func_.mem_func;
            }
            else if (emGenFunc == type_) {
                func_.gen_func = rhs.func_.gen_func;
            }
        }
        return *this;
    }

    /**
     * @brief   移动赋值操作符
     */
    FUNCTION_NAME& operator=(FUNCTION_NAME&& rhs) noexcept
    {
        if (this != &rhs) {
            type_ = std::exchange(rhs.type_, emInvalidFunc);
            if (emPMemFunc == type_) {
                sptr_ = std::exchange(rhs.sptr_, nullptr);
                func_.mem_func = std::exchange(rhs.func_.mem_func, nullptr);
            }
            else if (emMemFunc == type_) {
                object_ = std::exchange(rhs.object_, nullptr);
                func_.mem_func = std::exchange(rhs.func_.mem_func, nullptr);
            }
            else if (emGenFunc == type_) {
                func_.gen_func = std::exchange(rhs.func_.gen_func, nullptr);
            }
        }
        return *this;
    }

    /**
     * @brief   判断是否相等
     */
    bool operator==(const FUNCTION_NAME& rhs) const
    {
        if (emPMemFunc == type_) {
            return (sptr_.get() == rhs.sptr_.get()) && (func_.mem_func == rhs.func_.mem_func);
        }
        else if (emMemFunc == type_) {
            return (object_ == rhs.object_) && (func_.mem_func == rhs.func_.mem_func);
        }
        else if (emGenFunc == type_) {
            return (func_.gen_func == rhs.func_.gen_func);
        }
        return false;
    }

    /**
     * @brief   判断有效性
     */
    operator bool() const
    {
        if (emPMemFunc == type_) {
            return nullptr != func_.mem_func && 0 != sptr_.use_count();
        }
        else if (emMemFunc == type_) {
            return nullptr != func_.mem_func && nullptr != object_;
        }
        else if (emGenFunc == type_) {
            return nullptr != func_.gen_func;
        }
        return false;
    }

    /**
     * @brief       函数调用(调用相应的函数)
     * @param[in]   函数所需参数
     * @ps          若函数不正常，则抛出异常
     */
    inline R operator()(FUNCTION_TYPE_ARGS)
    {
        if (emPMemFunc == type_) {
            using Invoker = typename PTR_MEM_FUNCTION_INVOKER<std::is_void<R>::value>::
                template inner<R FUNCTION_COMMA FUNCTION_TYPES>;
            //typedef typename PTR_MEM_FUNCTION_INVOKER<std::is_void<R>::value
            //>::template inner<R FUNCTION_COMMA FUNCTION_TYPES> Invoker;
            return Invoker::invoke(sptr_, func_.mem_func FUNCTION_COMMA FUNCTION_ARGS);
        }
        else if (emMemFunc == type_) {
            using Invoker = typename MEM_FUNCTION_INVOKER<std::is_void<R>::value>::
                template inner<R FUNCTION_COMMA FUNCTION_TYPES>;
            //typedef typename MEM_FUNCTION_INVOKER<std::is_void<R>::value
            //>::template inner<R FUNCTION_COMMA FUNCTION_TYPES> Invoker;
            return Invoker::invoke(object_, func_.mem_func FUNCTION_COMMA FUNCTION_ARGS);
        }
        else if (emGenFunc == type_) {
            using Invoker = typename GEN_FUNCTION_INVOKER<std::is_void<R>::value>::
                template inner<R FUNCTION_COMMA FUNCTION_TYPES>;
            //typedef typename GEN_FUNCTION_INVOKER<std::is_void<R>::value
            //>::template inner<R FUNCTION_COMMA FUNCTION_TYPES> Invoker;
            return Invoker::invoke(func_.gen_func FUNCTION_COMMA FUNCTION_ARGS);
        }
        else {
            throw std::logic_error("bad function call");
        }
    }

private:
    //! 类型
    int32_t type_;

    //! 函数指针
    union {
        MEM_FUNCTION mem_func;
        GEN_FUNCTION gen_func;
    }func_;

    //! 类对象
    X* object_;
    //! 类对象的shared_ptr
    std::shared_ptr<X> sptr_;
};

#undef FUNCTION_RETURN
#undef FUNCTION_NAME
#undef FUNCTION_COMMA
#undef GEN_FUNCTION_INVOKER
#undef MEM_FUNCTION_INVOKER
#undef PTR_MEM_FUNCTION_INVOKER

BASE_END_NAMESPACE