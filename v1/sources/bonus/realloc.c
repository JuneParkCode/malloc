#include "malloc_bonus.h"
#include "malloc_util_bonus.h"

void *realloc(void *ptr, size_t size) __attribute__((visibility("default")));

/**
 * @fn realloc
 * @brief reallocates a memory block
 * @details The realloc() function changes the size of the memory block pointed
 * to by ptr to size bytes. The contents will be unchanged in the range from the
 * start of the region up to the minimum of the old and new sizes. If the new
 * size is larger than the old size, the added memory will not be initialized.
 * If ptr is NULL, then the call is equivalent to malloc(size), for all values
 * of size; if size is equal to zero, and ptr is not NULL, then the call is
 * equivalent to free(ptr). Unless ptr is NULL, it must have been returned by an
 * earlier call to malloc(), calloc() or realloc(). If the area pointed to was
 * moved, a free(ptr) is done.
 * @param ptr pointer to the memory block to reallocate
 * @param size new size for the memory block, in bytes
 * @return a pointer to the newly allocated memory, or NULL if the request
 * fails or free(ptr) was called with size equal to zero.
 */
void *realloc(void *ptr, size_t size)
{
	if (ptr == NULL)
		return (malloc(size));

	t_metadata *meta_data = ptr - sizeof(size_t);
	const size_t current_block_size = GET_BLOCK_SIZE(meta_data->header);
	const size_t request_block_size = __get_request_block_size(size);
	const size_t min_size = current_block_size < request_block_size
								? current_block_size
								: request_block_size;
	void *ret;

	if (size == 0) {
		free(ptr);
		return (NULL);
	}
	if (request_block_size == current_block_size) {
		ret = ptr;
	} else {
		ret = malloc(size);
		ft_memcpy(ret, ptr, min_size);
		free(ptr);
	}
	return (ret);
}