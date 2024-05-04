#include "tinker/core/init.h"

#include "tinker/core/config.h"



namespace ti::core
{
    void initalize()
    {
        printf("[TI_DEBUGGING = %d]", TI_DEBUGGING);
        printf("[TI_PROFILING = %d]", TI_PROFILING);
    }

    void terminate()
    {

    }
}