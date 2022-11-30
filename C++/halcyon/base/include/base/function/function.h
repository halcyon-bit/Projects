#ifndef BASE_FUNCTION_H
#define BASE_FUNCTION_H

#include <base/common/base_define.h>

#include <type_traits>  // std::is_void

/// 类似于 std::function 和 std::bind，但不需要占位符

// Fuction0
#define FUNCTION_NUMBER 0  // 类名 Function0
#define FUNCTION_CLASS_TYPES typename R  // 模板定义(R返回值)
#define FUNCTION_TYPES  // 模板参数类型
#define FUNCTION_TYPE_ARGS  // 模板参数
#define FUNCTION_ARGS  // 参数
#include "function_template.h"
#undef  FUNCTION_NUMBER
#undef  FUNCTION_CLASS_TYPES
#undef  FUNCTION_TYPES
#undef  FUNCTION_TYPE_ARGS
#undef  FUNCTION_ARGS

// Fuction1
#define FUNCTION_NUMBER 1  // 类名 Function1
#define FUNCTION_CLASS_TYPES typename R, typename T1
#define FUNCTION_TYPES T1
#define FUNCTION_TYPE_ARGS T1 t1
#define FUNCTION_ARGS t1
#include "function_template.h"
#undef  FUNCTION_NUMBER
#undef  FUNCTION_CLASS_TYPES
#undef  FUNCTION_TYPES
#undef  FUNCTION_TYPE_ARGS
#undef  FUNCTION_ARGS

// Fuction2
#define FUNCTION_NUMBER 2
#define FUNCTION_CLASS_TYPES typename R, typename T1, typename T2
#define FUNCTION_TYPES T1, T2
#define FUNCTION_TYPE_ARGS T1 t1, T2 t2
#define FUNCTION_ARGS t1, t2
#include "function_template.h"
#undef  FUNCTION_NUMBER
#undef  FUNCTION_CLASS_TYPES
#undef  FUNCTION_TYPES
#undef  FUNCTION_TYPE_ARGS
#undef  FUNCTION_ARGS

// Fuction3
#define FUNCTION_NUMBER 3
#define FUNCTION_CLASS_TYPES typename R, typename T1, typename T2, typename T3
#define FUNCTION_TYPES T1, T2, T3
#define FUNCTION_TYPE_ARGS T1 t1, T2 t2, T3 t3
#define FUNCTION_ARGS t1, t2, t3
#include "function_template.h"
#undef  FUNCTION_NUMBER
#undef  FUNCTION_CLASS_TYPES
#undef  FUNCTION_TYPES
#undef  FUNCTION_TYPE_ARGS
#undef  FUNCTION_ARGS

// Fuction4
#define FUNCTION_NUMBER 4
#define FUNCTION_CLASS_TYPES typename R, typename T1, typename T2, typename T3, typename T4
#define FUNCTION_TYPES T1, T2, T3, T4
#define FUNCTION_TYPE_ARGS T1 t1, T2 t2, T3 t3, T4 t4
#define FUNCTION_ARGS t1, t2, t3, t4
#include "function_template.h"
#undef  FUNCTION_NUMBER
#undef  FUNCTION_CLASS_TYPES
#undef  FUNCTION_TYPES
#undef  FUNCTION_TYPE_ARGS
#undef  FUNCTION_ARGS

// Fuction5
#define FUNCTION_NUMBER 5
#define FUNCTION_CLASS_TYPES typename R, typename T1, typename T2, typename T3, typename T4, typename T5
#define FUNCTION_TYPES T1, T2, T3, T4, T5
#define FUNCTION_TYPE_ARGS T1 t1, T2 t2, T3 t3, T4 t4, T5 t5
#define FUNCTION_ARGS t1, t2, t3, t4, t5
#include "function_template.h"
#undef  FUNCTION_NUMBER
#undef  FUNCTION_CLASS_TYPES
#undef  FUNCTION_TYPES
#undef  FUNCTION_TYPE_ARGS
#undef  FUNCTION_ARGS

// Fuction6
#define FUNCTION_NUMBER 6
#define FUNCTION_CLASS_TYPES typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6
#define FUNCTION_TYPES T1, T2, T3, T4, T5, T6
#define FUNCTION_TYPE_ARGS T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6
#define FUNCTION_ARGS t1, t2, t3, t4, t5, t6
#include "function_template.h"
#undef  FUNCTION_NUMBER
#undef  FUNCTION_CLASS_TYPES
#undef  FUNCTION_TYPES
#undef  FUNCTION_TYPE_ARGS
#undef  FUNCTION_ARGS

// Fuction7
#define FUNCTION_NUMBER 7
#define FUNCTION_CLASS_TYPES typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7
#define FUNCTION_TYPES T1, T2, T3, T4, T5, T6, T7
#define FUNCTION_TYPE_ARGS T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7
#define FUNCTION_ARGS t1, t2, t3, t4, t5, t6, t7
#include "function_template.h"
#undef  FUNCTION_NUMBER
#undef  FUNCTION_CLASS_TYPES
#undef  FUNCTION_TYPES
#undef  FUNCTION_TYPE_ARGS
#undef  FUNCTION_ARGS

#endif