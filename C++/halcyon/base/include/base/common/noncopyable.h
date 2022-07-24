#pragma once

#include <base/common/base_define.h>

BASE_BEGIN_NAMESPACE

/**
 * @brief   对象语义的基类
 */
class noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

BASE_END_NAMESPACE