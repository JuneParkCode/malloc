#ifndef TEST_BONUS_H
#define TEST_BONUS_Hs

typedef struct s_arg {
  int count;
  int fd;
} t_arg;

void *test_tiny_size(void *arg);
void *test_small_size(void *arg);
void *test_large_size(void *arg);

#endif