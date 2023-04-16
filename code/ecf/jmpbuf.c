#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  printf("sizeof(jmpbuf)=%ld\n", sizeof(jmp_buf));
  exit(0);
}
