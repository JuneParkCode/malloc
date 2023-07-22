#ifndef MALLOC_UTIL_H
#define MALLOC_UTIL_H

// -- COMPILER CHECKED INTERNAL LOCK --
// Using lock with this definition ensures that lock and unlock are
// always performed internally in the function. If UNLOCK_INTERNAL is not
// performed after LOCK_INTERNAL, a compiler error will be raised.
#include <pthread.h>
#define LOCK_INTERNAL(mutex)                                                   \
	do {                                                                       \
		pthread_mutex_lock(mutex);
#define UNLOCK_INTERNAL(mutex)                                                 \
	pthread_mutex_unlock(mutex);                                               \
	}                                                                          \
	while (0)                                                                  \
		;

#endif