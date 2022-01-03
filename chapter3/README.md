* 3.61
```c++
long cread_alt(long *xp) { return (!xp ? 0 : *xp); }

/*
cread_alt:
  movl $0, %eax
  testq %rdi, %rdi
  cmovne (%rdi), %rax
*/
```
* 3.62
```c++
typedef enum { MODE_A, MODE_B, MODE_C, MODE_D, MODE_E } mode_t;

long switch3(long* p1, long* p2, mode_t action) {
  // p1 in %rdi, p2 in %rsi, action in %edx
  long result = 0;
  switch (action) {
    case MODE_A:
      result = *p2;
      *p2 = *p1;
      break;
    case MODE_B:
      result = *p1 + *p2;
      *p1 = result;
      break;
    case MODE_C:
      *p1 = 59;
      result = *p2;
      break;
    case MODE_D:
      *p1 = *p2;
      result = 27;
      break;
    case MODE_E:
      result = 27;
      break;
    default:
      result = 12;
  }
  return result;
}
```
* 3.63
```c++
long switch_prob(long x, long n) {
  // x in %rdi, n in %rsi
  long result = x;
  switch (n) {
    case 60:
    case 62:
      result = 8 * x;
      break;
    case 63:
      result = x >> 3;
      break;
    case 64:
      x = x * 15;  // x << 4 - x = x * 15
    case 65:
      x *= x;
    default:
      result = x + 0x4b;
  }
  return result;
}
```
* 3.64<br>
A:&A[i][j][k]=xA + L(i×S×T + j×T + k)<br>
B:<br>
根据汇编指令, dest=A+8(65i+13j+k), 即S×T=65, T=13<br>
又根据sizeof(A)=3640, 即8×R×S×T=3640<br>
化简可得R=7, S=5, T=13<br>
* 3.65<br>
A:%rdx<br>
B:%rax<br>
C:M×8=120, M=15<br>
* 3.66<br>
```c++
#define NR(n) 3 * n
#define NC(n) 4 * n + 1
```
* 3.67<br>
A:

| 寄存器 | 地址 | 值  |
| ------ | ---- | --- |
|        | 96   |     |
|        | ...  |     |
| %rdi-> | 64   |     |
|        | ...  |     |
|        | 24   | z   |
|        | 16   | &z  |
|        | 8    | y   |
| %rsp-> | 0    | x   |

B: 传递了%rsp+64地址<br>
C: 通过%rsp加偏移量的方式访问。这里在调用process函数时要先将返回地址入栈，rsp+8，因此访问s元素时与eval函数中的rsp偏移量相差8<br>
D: 通过%rdi加偏移量方式设置结果<br>
E:

| 寄存器        | 地址 | 值         |
| ------------- | ---- | ---------- |
|               | 96   |            |
|               | ...  |            |
|               | 80   | z          |
|               | 72   | x          |
| %rax->        | 64   | y          |
|               | ...  |            |
|               | 24   | z          |
|               | 16   | &z         |
|               | 8    | y          |
| %rsp(eval)    | 0    | x          |
| %rsp(process) | -8   | next instr |

F: 调用者在栈帧中创建结构体并将其地址作为参数传给被调用者，被调用者直接修改该地址处的结构体，并返回此地址
* 3.68<br>
根据内存对齐要求分析<br>
str2中, t为int类型, 地址按4对齐, 因此array大小应满足4<B<=8;<br>
u为long类型, 地址按8对齐, s的起始位置为8+4=12, 因此s的大小应满足12<A*2<=20,即6<A<=10<br>
str1中, y为long类型, 地址按8对齐, 因此s的大小应满足176<4×A×B<=184, 即44<A×B<=46<br>
综合以上各式得到A=9, B=5
* 3.69<br>
```c++
typedef struct {
  int first;
  a_struct a[CNT];
  int last;
} b_struct;

void test(long i, b_struct *bp) {
  int n = bp->first + bp->last;
  a_struct *ap = &bp->a[i];
  ap->x[ap->idx] = n;
}

//i in rdi, bp in rsi
mov	0x120(%rsi), %ecx // bp->last 0x120=288
add (rsi), %ecx // bp->first + bp->last
lea	(%rdi,%rdi,4), %rax // rax = 5*i
lea	(%rsi,%rax,8), %rax // rax = 40*i + bp, sizeof(a_struct)=40
mov	0x8(%rax), %rdx  // ap->idx = rax + 8(offset of a, aligned by 8)
// a start from 8, end at 288, sizeof(a)=280, CNT=280/40=7
movslq	%ecx, %rcx  // n is long => ap->x is long
mov	%rcx, 0x10(%rax,%rdx,8) // ap->x[ap->idx] = ap->idx+ap->idx*8+0x8, 0x10=16(offset of a + offset of a->idx).
// x type is long. Count of x is (40-8)/8=4

typedef struct {
  long idx;
  long x[4];
} a_struct;
```
A: CNT=7<br>
B: 见上
* 3.70<br>
A:<br>
e1.p: 0<br>
e1.y: 8<br>
e2.x: 0<br>
e2.next 8<br>
B: 16<br>
C:
```c++
# up in %rdi
proc:
  movq 8(%rdi), %rax  // rax maybe e1.y or e2.next
  movq (%rax), %rdx   // rax must be a pointer, rax is e2.next
                      // rdx maybe e1.p or e2.x
  movq (%rdx), %rdx   // rdx must be a pointer, rdx is e1.p
  subq 8(%rax), %rdx  // rax+8 in subq should be a number, so it is e2.next->e1.y
  movq %rdx, (%rdi)   // rdi should be a number, so it is up->e2.x
  ret

void proc(union ele *up) {
  up->e2.x = *(up->e2.next->e1.p) - up->e2.next->e1.y;
}
```
* 3.71<br>
```c++
#include <stdio.h>
#define N 1024
void good_echo() {
  char buffer[N];
  while (true) {
    char* ret = fgets(buffer, N, stdin);
    if (!ret) break;
    printf("%s", ret);
  }
  return;
}
```
* 3.72<br>
A: s2=s1-(8×n+30)&FFFFFFF0<br>
B: p=(s2+15)&FFFFFFF0<br>
C: 当n为偶数, e1+e2=16, s1地址满足s1%16=15时, e2最大为15, 此时e1=1<br>
   当n为奇数, e1+e2=24, s1地址满足s1%16=0时, e2最小为0, 此时e1=24<br>
D: 按照16对齐<br>
* 3.73<br>
参考https://dreamanddead.github.io/CSAPP-3e-Solutions/chapter3/3.73/
```c++
range_t find_range(float x) {
  __asm__(
      "vxorps %xmm1, %xmm1, %xmm1\n\t"
      "vucomiss %xmm1, %xmm0\n\t"
      "jp .P\n\t"
      "ja .A\n\t"
      "jb .B\n\t"
      "je .E\n\t"
      ".A:\n\t"
      "movl $2, %eax\n\t"
      "jmp .Done\n\t"
      ".B:\n\t"
      "movl $0, %eax\n\t"
      "jmp .Done\n\t"
      ".E:\n\t"
      "movl $1, %eax\n\t"
      "jmp .Done\n\t"
      ".P:\n\t"
      "movl $3, %eax\n\t"
      ".Done:\n\t"
      "ret\n\t");
}
```
* 3.74<br>
注意cmovbl与cmovpl的顺序。当x为nan时, vucomiss会将PF，CF，ZF均置1，因此需要将cmovbl判断放在cmovpl判断之前，反之cmovpl的结果会被cmovbl覆盖
```c++
range_t find_range(float x) {
  __asm__(
      "vxorps %xmm1, %xmm1, %xmm1\n\t"
      "movl  $1, %eax\n\t"
      "movl  $0, %r8d\n\t"
      "movl  $3, %r9d\n\t"
      "movl  $2, %r10d\n\t"
      "vucomiss %xmm1, %xmm0\n\t"
      "cmovbl %r8d, %eax\n\t"
      "cmovpl %r9d, %eax\n\t"
      "cmoval %r10d, %eax\n\t");
}
```
* 3.75<br>
A: 复数参数传递时, 第一个参数的实部虚部分别通过xmm0, xmm1传递, 第二个参数的实部虚部分别通过xmm2, xmm3传递, 以此类推<br>
B: 返回值的实部虚部分别通过xmm0, xmm1传递