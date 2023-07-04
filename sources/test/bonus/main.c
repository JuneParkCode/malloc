#include "malloc_bonus.h"
#include "test_bonus.h"
// #include <pthread.h>
#include <unistd.h>

int main() {
  void *a = malloc(2);
  (void)a;
  // const int thread_size = 1;
  // pthread_t thread_t[thread_size];
  // t_arg arg[thread_size];
  // int status;
  // void *ret;

  // for (int i = 0; i < thread_size; ++i) {
  //   arg[i].count = 16;
  //   if (pthread_create(&thread_t[i], NULL, test_tiny_size, (void *)&arg) < 0)
  //   {
  //     perror("thread create error:");
  //     return (-1);
  //   }
  // }
  // for (int i = 0; i < thread_size; ++i) {
  //   status = pthread_join(thread_t[i], &ret);
  //   if (status)
  //     printf("%d Thread End with error %d\n", i, status);

  //   printf("%p\n", ret);
  //   show_alloc_mem();
  //   // if (ret)
  //   free(ret); // it allocated by another thread. check error happens.
  // }
  // show_alloc_mem();
}