#pragma once

#include <base/common/base_define.h>

#ifdef USE_CPP11
#include <utility>

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
