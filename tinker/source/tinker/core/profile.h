#pragma once

#include "tinker/core/config.h"



#ifdef TI_PROFILE
    #define PROFILE_SCOPE(name) \
        // ProfileTimer timer##__LINE__(name)
    #define PROFILE_FUNCTION() \
        PROFILE_SCOPE(__FUNCTION__)
#else
    #define PROFILE_SCOPE(name)
    #define PROFILE_FUNCTION()
#endif