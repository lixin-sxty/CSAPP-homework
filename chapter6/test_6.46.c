#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define N 15000
#define MEM_SIZE (sizeof(int) * N * N)
#define LOOP 10

void init_data(int *src, int *expect) {
  int i, j;
  memset(expect, 0, MEM_SIZE);
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      int tmp = rand() % 2;
      src[i * N + j] = tmp;
      if (tmp) {
        expect[i * N + j] = expect[j * N + i] = tmp;
      }
    }
  }
}

void time_count(void (*f)(int *, int), int block) {
  // data init
  int *src = malloc(MEM_SIZE);
  int *expect = malloc(MEM_SIZE);
  init_data(src, expect);
  // main func test
  struct timeval start, end;
  gettimeofday(&start, NULL);
  for (size_t i = 0; i < LOOP; ++i)
    f(src, block);
  gettimeofday(&end, NULL);
  // result check
  assert(!memcmp(src, expect, MEM_SIZE));
  // time print
  double timeuse =
      1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  printf("Run %d, avg time =%f ms\n", LOOP, timeuse / 1000 / LOOP);
  // data free
  free(src);
  free(expect);
}

void convert(int *G, int unuse) {
  int i, j;
  for (i = 0; i <= N - 1; i++)
    for (j = 0; j <= N - 1; j++)
      G[j * N + i] = G[i * N + j] || G[j * N + i];
}

void perf_convert(int *src, int block) {
  int i, j, a, b, tmp;

  for (i = 0; i <= N - block; i += block)
    /* no need for j<i */
    for (j = i; j <= N - block; j += block)
      for (a = i; a < i + block; a += 16) {
        int a0 = a * N;
        int a1 = (a + 1) * N;
        int a2 = (a + 2) * N;
        int a3 = (a + 3) * N;
        int a4 = (a + 4) * N;
        int a5 = (a + 5) * N;
        int a6 = (a + 6) * N;
        int a7 = (a + 7) * N;
        int a8 = (a + 8) * N;
        int a9 = (a + 9) * N;
        int a10 = (a + 10) * N;
        int a11 = (a + 11) * N;
        int a12 = (a + 12) * N;
        int a13 = (a + 13) * N;
        int a14 = (a + 14) * N;
        int a15 = (a + 15) * N;
        for (b = j; b < j + block; b++) {
          int bN = b * N;
          src[bN + a] = src[a0 + b] = src[bN + a] || src[a0 + b];
          src[bN + a + 1] = src[a1 + b] = src[bN + a + 1] || src[a1 + b];
          src[bN + a + 2] = src[a2 + b] = src[bN + a + 2] || src[a2 + b];
          src[bN + a + 3] = src[a3 + b] = src[bN + a + 3] || src[a3 + b];
          src[bN + a + 4] = src[a4 + b] = src[bN + a + 4] || src[a4 + b];
          src[bN + a + 5] = src[a5 + b] = src[bN + a + 5] || src[a5 + b];
          src[bN + a + 6] = src[a6 + b] = src[bN + a + 6] || src[a6 + b];
          src[bN + a + 7] = src[a7 + b] = src[bN + a + 7] || src[a7 + b];
          src[bN + a + 8] = src[a8 + b] = src[bN + a + 8] || src[a8 + b];
          src[bN + a + 9] = src[a9 + b] = src[bN + a + 9] || src[a9 + b];
          src[bN + a + 10] = src[a10 + b] = src[bN + a + 10] || src[a10 + b];
          src[bN + a + 11] = src[a11 + b] = src[bN + a + 11] || src[a11 + b];
          src[bN + a + 12] = src[a12 + b] = src[bN + a + 12] || src[a12 + b];
          src[bN + a + 13] = src[a13 + b] = src[bN + a + 13] || src[a13 + b];
          src[bN + a + 14] = src[a14 + b] = src[bN + a + 14] || src[a14 + b];
          src[bN + a + 15] = src[a15 + b] = src[bN + a + 15] || src[a15 + b];
        }
      }

  int offset = i;

  for (i = offset; i <= N - 1; i++)
    for (j = 0; j < offset; j += block)
      for (b = j; b < j + block; b++) {
        src[b * N + i] = src[i * N + b] = src[b * N + i] || src[i * N + b];
      }

  for (i = offset; i <= N - 1; i++)
    for (j = i; j <= N - 1; j++) {
      src[j * N + i] = src[i * N + j] = src[j * N + i] || src[i * N + j];
    }
}

int main(int argc, char const *argv[]) {
  printf("convert:\n");
  time_count(convert, -1);
  printf("perf_convert_256_16\n");
  time_count(perf_convert, 256);
}
