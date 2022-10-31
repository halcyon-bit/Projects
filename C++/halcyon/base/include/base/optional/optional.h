#pragma once

#include <base/common/base_define.h>

#ifdef USE_CPP11
#include <base/utility/types.h>  // std::aligned_storage_t
#endif

#include <stdexcept>

BASE_BEGIN_NAMESPACE

/**
 * @brief   Optional ����һ��ֵ���ȿ��Դ���Ҳ���Բ����ڵ�ֵ��ֻ�е� Optional
 *      ����ʼ��֮����� Optional ������Ч�ġ�
 *
 * @ps      C++17 ������ std::optional ����
 */
template<typename T>
class Optional
{
    using value_type = std::aligned_storage_t<sizeof(T), std::alignment_of<T>::value>;
public:  // ���캯��
    Optional()
    {}

    Optional(const T& val)
    {
        create(val);
    }

    Optional(T&& val)
    {
        create(std::forward<T>(val));
    }

    Optional(const Optional& rhs)
    {
        if (rhs.isInit()) {
            assign(rhs);
        }
    }

    Optional(Optional&& rhs)
    {
        if (rhs.isInit()) {
            assign(std::forward<Optional>(rhs));
            rhs.destory();
        }
    }

    ~Optional()
    {
        destory();
    }

    Optional& operator=(const Optional& rhs)
    {
        assign(rhs);
        return *this;
    }

    Optional& operator=(Optional&& rhs)
    {
        assign(std::forward<Optional>(rhs));
        return *this;
    }

public:
    /**
     * @brief   �Ƿ��ʼ�� 
     */
    bool isInit() const
    {
        return init_;
    }

    template<typename... Args>
    void emplace(Args&&... args)
    {
        destory();
        create(std::forward<Args>(args)...);
    }

public:
    T& operator*()
    {
        if (isInit()) {
            return *((T*)(&data_));
        }
        else {
            throw std::logic_error("");
        }
    }

    const T& operator*() const
    {
        if (isInit()) {
            return *((T*)(&data_));
        }
        else {
            throw std::logic_error("");
        }
    }

    T* operator->()
    {
        return &operator*();
    }
    const T* operator->() const
    {
        return &operator*();
    }

public:
    operator bool() const
    {
        return init_;
    }

private:
    template<typename... Args>
    void create(Args&&... args)
    {
        new (&data_) T(std::forward<Args>(args)...);
        init_ = true;
    }

    void destory()
    {
        if (init_) {
            init_ = false;
            ((T*)(&data_))->~T();
        }
    }

    void assign(const Optional& rhs)
    {
        if (rhs.isInit()) {
            copy(rhs.data_);
            init_ = true;
        }
        else {
            destory();
        }
    }

    void assign(Optional&& rhs)
    {
        if (rhs.isInit()) {
            move(std::move(rhs.data_));
            init_ = true;
            rhs.destory();
        }
        else {
            destory();
        }
    }

    void copy(const value_type& val)
    {
        destory();
        new (&data_) T(*((T*)(&val)));
    }

    void move(value_type&& val)
    {
        destory();
        new (&data_) T(std::move(*((T*)(&val))));
    }

private:
    //! �Ƿ��ʼ��
    bool init_{ false };
    value_type data_;
};

BASE_END_NAMESPACE