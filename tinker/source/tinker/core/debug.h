#pragma once

#include "tinker/core/config.h"

#include <cassert>
#include <iostream>
#include <string.h>



/**
 * @brief Used to provide correctness of code (not robustness).
*/
#define TI_ASSERT(exp, msg) assert(exp && msg)
#define TI_STATIC_ASSERT(exp, msg) static_assert(exp, msg)

#ifdef TI_DEBUGGING
    /**
     * @brief Used to signal errors
    */
    #define TI_ERROR(...)                                                   \
    {                                                                       \
        fprintf(stderr, "[ERROR] ");                                        \
        fprintf(stderr, __VA_ARGS__);                                       \
        fprintf(stderr, " [FILE: %s] [LINE: %d]\n", __FILE__, __LINE__);    \
        fflush(stderr);                                                     \
    }

    /**
     * @brief Used to signal errors if condition is false
    */
    #define TI_REQUIRE(cond, ...)   \
    if(!cond)                       \
    {                               \
        TI_ERROR(__VA_ARGS__);      \
    }
#else
    #define TI_ERROR(...)
    #define TI_REQUIRE(cond, ...)
#endif