* 7.6<br>
     | 符号  | swap.o.symtab条目? | 符号类型 | 定义符号的模块 | 节    |
     | ----- | ------------------ | -------- | -------------- | ----- |
     | buf   | 是                 | 外部     | m.o            | .data |
     | bufp0 | 是                 | 全局     | swap.o         | .data |
     | bufp1 | 是                 | 局部     | swap.o         | .bss  |
     | swap  | 是                 | 全局     | swap.o         | .text |
     | temp  | 否                 | -        | -              | -     |
     | incr  | 是                 | 局部     | swap.o         | .text |
     | count | 是                 | 局部     | swap.o         | .bss  |
* 7.7<br>
  ```c++
  // bar5.c
  static double x;
  void f() { X = -0.0; }
  ```
* 7.8<br>
  ```c++
  // A
  /* Module 1 */
  int main() {}
  /* Module 2 */
  static int main = 1;
  int p2() {}
  (a) REF(main.1) → DEF(main.1)
  (b) REF(main.2) → DEF(main.2)

  // B
  /* Module 1 */
  int x;
  void main() {}
  /* Module 2 */
  double x;
  int p2() {}
  (a) REF(x.1) → DEF(未知)
  (b) REF(x.2) → DEF(未知)

  // C
  /* Module 1 */
  int x = 1;
  void main() {}
  /* Module 2 */
  double x = 1.0;
  int p2() {}
  (a) REF(x.1) → DEF(错误)
  (b) REF(x.2) → DEF(错误)
  ```
* 7.9<br>
  在foo6.c中，main是强符号(函数)，在bar6.c中，main是弱符号(未定义变量)，根据链接器规则，会选择foo6.c中的符号main，打印出main函数地址处的值48(在我的测试机器上为55，对应push指令)
* 7.10<br>
  A. p.o->libx.a->p.o<br>
  `gcc p.o libx.a`<br>
  B. p.o->libx.a->liby.a和liby.a->libx.a<br>
  `gcc p.o libx.a liby.a libx.a`<br>
  C. p.o->libx.a->liby.a->libz.a和liby.a->libx.a->libz.a<br>
  `gcc p.o libx.a liby.a libx.a libz.a`<br>
* 7.11<br>
  剩下的8个字节对应于运行时将被初始化为0的`.bss`数据<br>
* 7.12<br>
  A:
  ```
  refaddr = ADDR(s) + r.offset = 0x4004e0 + 0xa = 0x4004ea
  *refptr = (unsigned)(ADDR(r.symbol) + r.addend - refaddr)
          = (unsigned) (0x4004f8 + (-4) - 0x4004ea)
          = (unsigned) (0xa)
  ```
  B:
  ```
  refaddr = ADDR(s) + r.offset = 0x4004d0 + 0xa = 0x4004da
  *refptr = (unsigned)(ADDR(r.symbol) + r.addend - refaddr)
          = (unsigned) (0x400500 + (-4) - 0x4004da)
          = (unsigned) (0x22)
  ```
* 7.13<br>
  A:
  ```shell
  $ ar t /usr/lib/x86_64-linux-gnu/libc.a |wc -l
  1597
  $ ar t /usr/lib/x86_64-linux-gnu/libm.a |wc -l
  471
  ```
  B:没有区别<br>
  C:
  ```shell
  $ ldd /usr/bin/gcc
    linux-vdso.so.1 =>  (0x00007ffed5bbc000)
    libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6(0x00007f63362a0000)
    /lib64/ld-linux-x86-64.so.2 (0x00007f633666a000)
  ```
