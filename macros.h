//
// Created by WYZ on 2024/4/14.
//

#ifndef THREADPOOLS_MACROS_H
#define THREADPOOLS_MACROS_H

#endif //THREADPOOLS_MACROS_H
#pragma once
#define ARROW_UNUSED(x) (void)(x)

#define ARROW_PREDICT_FALSE(x) (__builtin_expect(!!(x), 0))

#define DCHECK_GE(val1, val2) \
  do { \
    if (!((val1) >= (val2))) { \
      std::abort(); \
    } \
  } while (false)

#define DCHECK_EQ(val1, val2) \
  do { \
    if (!((val1) == (val2))) { \
      std::abort(); \
    } \
  } while (false)

#define DCHECK_OK(val) \
  do { \
    if (!((val).ok())) { \
      std::abort(); \
    } \
  } while (false)

#define DCHECK_NOT_OK(val) \
  do { \
    if (((val).ok())) { \
      std::abort(); \
    } \
  } while (false)
