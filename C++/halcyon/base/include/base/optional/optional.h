#pragma once

#include <base/utility/types.h>  // std::aligned_storage_t

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
            copy(rhs.data_);
        }
    }

    Optional(Optional&& rhs) noexcept
    {
        if (rhs.isInit()) {
            move(std::move(rhs.data_));
            rhs.destroy();
        }
    }

    ~Optional()
    {
        destroy();
    }

    Optional& operator=(const Optional& rhs)
    {
        if (this == &rhs) {
            return *this;
        }
        destroy();  // ��������
        if (rhs.isInit()) {
            copy(rhs.data_);
        }
        return *this;
    }

    Optional& operator=(Optional&& rhs) noexcept
    {
        if (this == &rhs) {
            return *this;
        }
        destroy();  // ��������
        if (rhs.isInit()) {
            move(std::move(rhs.data_));
            rhs.destroy();
        }
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
        destroy();
        create(std::forward<Args>(args)...);
    }

public:
    T& operator*()
    {
        if (isInit()) {
            return *((T*)(&data_));
        }
        else {
            throw std::logic_error("Optional not initialized");
        }
    }

    const T& operator*() const
    {
        if (isInit()) {
            return *((T*)(&data_));
        }
        else {
            throw std::logic_error("Optional not initialized");
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

    void destroy()
    {
        if (init_) {
            ((T*)(&data_))->~T();
            init_ = false;
        }
    }

    void copy(const value_type& val)
    {
        new (&data_) T(*((T*)(&val)));
        init_ = true;
    }

    void move(value_type&& val)
    {
        new (&data_) T(std::move(*((T*)(&val))));
        init_ = true;
    }

private:
    //! �Ƿ��ʼ��
    bool init_{ false };
    value_type data_;
};

BASE_END_NAMESPACE