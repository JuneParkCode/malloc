#include "malloc.h"

/**
 * @fn free
 * @brief free memory
 * @param ptr memory address
 * @return void
 *
 * @note we have to abort on double free but we can not use abort() in this
 * project so, just return it. if you want to use abort(), just replace return
 * to abort() if (!IS_ALLOCATED(meta_data->header)
 */
void free(void *ptr)
{
}
