#include "malloc.h"
#include "test.h"
#include <stdio.h>

void test_tiny_size(int count) {
  printf("====%s START====\n", __FUNCTION__);
  int *ptr[count];

  for (int i = 0; i < count; ++i) {
    ptr[i] = malloc(sizeof(int) * ((i % 100) + 1));
    *ptr[i] = i;
  }
  show_alloc_mem();
  for (int i = 0; i < count; ++i) {
    if (i != *ptr[i])
      printf("ERROR ON %s:%d\n", __FUNCTION__, __LINE__);
    free(ptr[i]);
  }
  show_alloc_mem();
  printf("====%s DONE====\\n", __FUNCTION__);
}

void test_small_size(int count) {
  printf("====%s START====\n", __FUNCTION__);
  int *ptr[count];

  for (int i = 0; i < count; ++i) {
    ptr[i] = malloc(512 * ((i % 16) + 1));
    *ptr[i] = i;
  }
  show_alloc_mem();
  for (int i = 0; i < count; ++i) {
    if (i != *ptr[i])
      printf("ERROR ON %s:%d\n", __FUNCTION__, __LINE__);
    free(ptr[i]);
  }
  printf("AFTER FREE\n");
  show_alloc_mem();
  printf("====%s DONE====\n", __FUNCTION__);
}

void test_large_size(int count) {
  printf("====%s START====\n", __FUNCTION__);
  int *ptr[count];

  for (int i = 0; i < count; ++i) {
    ptr[i] = malloc(4096 * 4 * ((i % 16) + 1));
    *ptr[i] = i;
  }
  show_alloc_mem();
  for (int i = 0; i < count; ++i) {
    if (i != *ptr[i])
      printf("ERROR ON %s:%d\n", __FUNCTION__, __LINE__);
    free(ptr[i]);
  }
  printf("AFTER FREE\n");
  show_alloc_mem();
  printf("====%s DONE====\n", __FUNCTION__);
}

void test_fail_case() {
  // int *ptr[count];

  // for (int i = 0; i < count; ++i) {
  //   ptr[i] = malloc(sizeof(int) * ((i % 100)));
  //   *ptr[i] = i;
  // }
  // show_alloc_mem();
  // for (int i = 0; i < count; ++i) {
  //   if (i != *ptr[i])
  //     printf("ERROR ON %s:%d\n", __FUNCTION__, __LINE__);
  //   free(ptr[i]);
  // }
  // show_alloc_mem();
}