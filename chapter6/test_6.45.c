#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define N 15000
#define MEM_SIZE (sizeof(int) * N * N)
#define LOOP 10

void init_data(int *dst, int *src, int *expect) {
  int i, j;
  memset(dst, 0, MEM_SIZE);
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      src[i * N + j] = i;
      expect[i * N + j] = j;
    }
  }
}

void time_count(void (*f)(int *, int *, int), int block) {
  // data init
  int *dst = malloc(MEM_SIZE);
  int *src = malloc(MEM_SIZE);
  int *expect = malloc(MEM_SIZE);
  init_data(dst, src, expect);
  // main func test
  struct timeval start, end;
  gettimeofday(&start, NULL);
  for (size_t i = 0; i < LOOP; ++i)
    f(dst, src, block);
  gettimeofday(&end, NULL);
  // result check
  assert(!memcmp(dst, expect, MEM_SIZE));
  // time print
  double timeuse =
      1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  printf("Run %d, avg time =%f ms\n", LOOP, timeuse / 1000 / LOOP);
  // data free
  free(dst);
  free(src);
  free(expect);
}

void transpose(int *dst, int *src, int unuse) {
  int i, j;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      dst[j * N + i] = src[i * N + j];
}

void perf_transpose(int *dst, int *src, int block) {
  int i, j, a, b;
  int limit = N - N % block;

  for (i = 0; i < limit; i += block)
    for (j = 0; j < limit; j += block)
      for (a = i; a < i + block; a += 8) {
        int a0 = a * N;
        int a1 = (a + 1) * N;
        int a2 = (a + 2) * N;
        int a3 = (a + 3) * N;
        int a4 = (a + 4) * N;
        int a5 = (a + 5) * N;
        int a6 = (a + 6) * N;
        int a7 = (a + 7) * N;
        for (b = j; b < j + block; b++) {
          int bN = b * N;
          dst[bN + a] = src[a0 + b];
          dst[bN + a + 1] = src[a1 + b];
          dst[bN + a + 2] = src[a2 + b];
          dst[bN + a + 3] = src[a3 + b];
          dst[bN + a + 4] = src[a4 + b];
          dst[bN + a + 5] = src[a5 + b];
          dst[bN + a + 6] = src[a6 + b];
          dst[bN + a + 7] = src[a7 + b];
        }
      }

  int offset = i;
  // tail
  for (i = offset; i <= N - 1; i++)
    for (j = 0; j < offset; j += block)
      for (b = j; b < j + block; b++)
        dst[b * N + i] = src[i * N + b];

  for (i = 0; i < N; i++)
    for (j = offset; j < N; j++)
      dst[j * N + i] = src[i * N + j];
}

int main(int argc, char const *argv[]) {
  printf("transpose:\n");
  time_count(transpose, -1);
  printf("perf_transpose_128_8\n");
  time_count(perf_transpose, 128);
}
