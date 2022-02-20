# Y86模拟器安装
本章部分习题需要Y86模拟器，安装步骤如下：
1. 源码下载：http://csapp.cs.cmu.edu/public/students.html
2. 安装词法分析工具：sudo apt-get install bison flex
3. 安装Tcl/Tk 支持图形界面(可选)：sudo apt-get install tcl8.5-dev tk8.5-dev tcl8.5 tk8.5
4. 解压：tar xf sim.tar
5. 修改Makefile文件
```bash
GUIMODE=-DHAS_GUI # 如果第3步安装了GUI支持则打开此行
TKLIBS=-L/usr/lib/ -ltk8.5 -ltcl8.5 # 根据本地路径修改
TKINC=-I/usr/include/tcl8.5 # 根据本地路径修改
```
6. 编译：cd sim && make clean && make
7. 执行参考http://csapp.cs.cmu.edu/public/students.html中**simguide.pdf**
# homework
* 4.45<br>
A: 不正确, pushq %rsp期望将rsp的旧值入栈，当前代码将更新后的值入栈<br>
B:
```c++
movq REG, -8(%rsp)
subq $8, %rsp
```
* 4.46<br>
A: 不正确, popq %rsp期望将rsp初始位置的值更新到rsp中，当前代码在更新后又将rsp增加<br>
B:
```c++
addq $8, %rsp
movq 8(%rsp), REG
```
* 4.47<br>
A:
```c++
void bubble_p(long *data, long count) {
  long *i, *last;
  for (last = data + count - 1; last > data; last--) {
    for (i = data; i < last; i++)
      if (*(i + 1) < *i) {
        /* Swap adjacent elements */
        long t = *(i + 1);
        *(i + 1) = *i;
        *i = t;
      }
  }
}
```
B:
见文件4_47.ys.<br>
使用yas编译后使用yis执行结果如下：
```bash
Stopped in 405 steps at PC = 0x13.  Status 'HLT', CC Z=1 S=0 O=0
Changes to registers:
%rax:   0x0000000000000000      0x0000000000000020
%rcx:   0x0000000000000000      0x0000000000000002
%rdx:   0x0000000000000000      0x0000000000000001
%rsp:   0x0000000000000000      0x0000000000000200
%rsi:   0x0000000000000000      0x0000000000000018
%rdi:   0x0000000000000000      0x0000000000000018
%r8:    0x0000000000000000      0x0000000000000008
%r11:   0x0000000000000000      0xffffffffffffffff

Changes to memory:
0x0018: 0x0000000000000008      0x0000000000000001
0x0020: 0x0000000000000007      0x0000000000000002
0x0028: 0x0000000000000006      0x0000000000000003
0x0030: 0x0000000000000005      0x0000000000000004
0x0038: 0x0000000000000004      0x0000000000000005
0x0040: 0x0000000000000003      0x0000000000000006
0x0048: 0x0000000000000002      0x0000000000000007
0x0050: 0x0000000000000001      0x0000000000000008

0x01f0: 0x0000000000000000      0x0000000000000075
0x01f8: 0x0000000000000000      0x0000000000000013
```
**注意，输出结果中除了期望的内存改变外，栈顶的内存(0x1f8,0x1f0)也出现的变化，这是使用call或者pushq指令数据入栈后没有进行清零操作产生的.后续习题中也有此情况**<br>
可以看到输入数据已经按照升序正确排序，共执行405次指令<br>
* 4.48<br>s
见文件4_48.ys.<br>
使用yas编译后使用yis执行结果如下：
```bash
Stopped in 461 steps at PC = 0x13.  Status 'HLT', CC Z=1 S=0 O=0
Changes to registers:
%rax:   0x0000000000000000      0x0000000000000020
%rcx:   0x0000000000000000      0x0000000000000002
%rdx:   0x0000000000000000      0x0000000000000001
%rsp:   0x0000000000000000      0x0000000000000200
%rsi:   0x0000000000000000      0x0000000000000018
%rdi:   0x0000000000000000      0x0000000000000018
%r8:    0x0000000000000000      0x0000000000000008
%r11:   0x0000000000000000      0xffffffffffffffff

Changes to memory:
0x0018: 0x0000000000000008      0x0000000000000001
0x0020: 0x0000000000000007      0x0000000000000002
0x0028: 0x0000000000000006      0x0000000000000003
0x0030: 0x0000000000000005      0x0000000000000004
0x0038: 0x0000000000000004      0x0000000000000005
0x0040: 0x0000000000000003      0x0000000000000006
0x0048: 0x0000000000000002      0x0000000000000007
0x0050: 0x0000000000000001      0x0000000000000008

0x01f0: 0x0000000000000000      0x0000000000000075
0x01f8: 0x0000000000000000      0x0000000000000013
```
可以看到输入数据已经按照升序正确排序，共执行461次指令<br>
* 4.49<br>
参考：https://dreamanddead.github.io/CSAPP-3e-Solutions/chapter4/4.49/<br>
异或操作有如下特性：a^b^a=b, a^b^b=a，因此使用3次异或操作效果相当于一次swap
```bash
x = a, y = b, c = a ^ b
x = x ^ y # x=c,y=b
y = x ^ y # x=c,y=a
x = x ^ y # x=b,y=a

如果在第一次异或后修改y的值
x = a, y = b, c = a ^ b
x = x ^ y # x=c,y=b
y = a     # x=c,y=a
y = x ^ y # x=c,y=b
x = x ^ y # x=a,y=b
```
根据此方法可以只使用一次条件传送指令实现swap,具体见文件4_49.ys.
使用yas编译后使用yis执行结果如下：
```bash
Stopped in 517 steps at PC = 0x13.  Status 'HLT', CC Z=1 S=0 O=0
Changes to registers:
%rax:   0x0000000000000000      0x0000000000000020
%rcx:   0x0000000000000000      0x0000000000000002
%rdx:   0x0000000000000000      0x0000000000000001
%rsp:   0x0000000000000000      0x0000000000000200
%rsi:   0x0000000000000000      0x0000000000000018
%rdi:   0x0000000000000000      0x0000000000000018
%r8:    0x0000000000000000      0x0000000000000008
%r11:   0x0000000000000000      0xffffffffffffffff
%r12:   0x0000000000000000      0x0000000000000002

Changes to memory:
0x0018: 0x0000000000000008      0x0000000000000001
0x0020: 0x0000000000000007      0x0000000000000002
0x0028: 0x0000000000000006      0x0000000000000003
0x0030: 0x0000000000000005      0x0000000000000004
0x0038: 0x0000000000000004      0x0000000000000005
0x0040: 0x0000000000000003      0x0000000000000006
0x0048: 0x0000000000000002      0x0000000000000007
0x0050: 0x0000000000000001      0x0000000000000008

0x01f0: 0x0000000000000000      0x0000000000000075
0x01f8: 0x0000000000000000      0x0000000000000013
```
可以看到输入数据已经按照升序正确排序，共执行517次指令<br>
* 4.50<br>
见文件4_50.ys.<br>
使用yas编译后使用yis执行结果如下：
```bash
Stopped in 167 steps at PC = 0x13.  Status 'HLT', CC Z=0 S=1 O=0
Changes to registers:
%rax:   0x0000000000000000      0x0000000000000ddd
%rcx:   0x0000000000000000      0x0000000000000001
%rsp:   0x0000000000000000      0x0000000000000200
%rsi:   0x0000000000000000      0x0000000000000101
%rdi:   0x0000000000000000      0x000000000000019f
%r8:    0x0000000000000000      0x0000000000000018
%r10:   0x0000000000000000      0x0000000000000001
%r11:   0x0000000000000000      0x0000000000000005
%r12:   0x0000000000000000      0x0000000000000008

Changes to memory:
0x0018: 0x0000000000000000      0x0000000000000aaa
0x0020: 0x0000000000000000      0x0000000000000bbb
0x0028: 0x0000000000000000      0x0000000000000ccc
0x0030: 0x0000000000000000      0x0000000000000bbb
0x0038: 0x0000000000000000      0x0000000000000ddd
0x0040: 0x0000000000000000      0x0000000000000ddd

0x01e8: 0x0000000000000000      0x000000000000019f
0x01f0: 0x0000000000000000      0x00000000000000f6
0x01f8: 0x0000000000000000      0x0000000000000013
```
* 4.51<br>

| 阶段   | iaddq                                                                          |
| ------ | ------------------------------------------------------------------------------ |
| 取指   | icode:ifun <- M1[PC]<br>rA:rB <- M1[PC+1]<br>valC <- M8[PC+2]<br>valP <- PC+10 |
| 译码   | valB <- R[rB]                                                                  |
| 执行   | valE <- valC + valB<br>Set CC                                                  |
| 访存   |                                                                                |
| 写回   | R[rB] <- valE                                                                  |
| 更新PC | PC <- valP                                                                     |
* 4.52<br>
```bash
$ diff -u seq-full.hcl.backup seq-full.hcl
--- seq-full.hcl.backup 2022-02-20 19:39:06.776067375 +0800
+++ seq-full.hcl        2022-02-20 19:43:31.335671601 +0800
@@ -104,18 +104,18 @@
        1: imem_ifun;           # Default: get from instruction memory
 ];

-bool instr_valid = icode in
+bool instr_valid = icode in
        { INOP, IHALT, IRRMOVQ, IIRMOVQ, IRMMOVQ, IMRMOVQ,
-              IOPQ, IJXX, ICALL, IRET, IPUSHQ, IPOPQ };
+              IOPQ, IJXX, ICALL, IRET, IPUSHQ, IPOPQ, IIADDQ };

 # Does fetched instruction require a regid byte?
 bool need_regids =
-       icode in { IRRMOVQ, IOPQ, IPUSHQ, IPOPQ,
-                    IIRMOVQ, IRMMOVQ, IMRMOVQ };
+       icode in { IRRMOVQ, IOPQ, IPUSHQ, IPOPQ,
+                    IIRMOVQ, IRMMOVQ, IMRMOVQ, IIADDQ };

 # Does fetched instruction require a constant word?
 bool need_valC =
-       icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL };
+       icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL, IIADDQ };

 ################ Decode Stage    ###################################

@@ -128,7 +128,7 @@

 ## What register should be used as the B source?
 word srcB = [
-       icode in { IOPQ, IRMMOVQ, IMRMOVQ  } : rB;
+       icode in { IOPQ, IRMMOVQ, IMRMOVQ, IIADDQ  } : rB;
        icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
        1 : RNONE;  # Don't need register
 ];
@@ -136,7 +136,7 @@
 ## What register should be used as the E destination?
 word dstE = [
        icode in { IRRMOVQ } && Cnd : rB;
-       icode in { IIRMOVQ, IOPQ} : rB;
+       icode in { IIRMOVQ, IOPQ, IIADDQ } : rB;
        icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
        1 : RNONE;  # Don't write any register
 ];
@@ -152,7 +152,7 @@
 ## Select input A to ALU
 word aluA = [
        icode in { IRRMOVQ, IOPQ } : valA;
-       icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ } : valC;
+       icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IIADDQ } : valC;
        icode in { ICALL, IPUSHQ } : -8;
        icode in { IRET, IPOPQ } : 8;
        # Other instructions don't need ALU
@@ -160,8 +160,8 @@

 ## Select input B to ALU
 word aluB = [
-       icode in { IRMMOVQ, IMRMOVQ, IOPQ, ICALL,
-                     IPUSHQ, IRET, IPOPQ } : valB;
+       icode in { IRMMOVQ, IMRMOVQ, IOPQ, ICALL,
+                     IPUSHQ, IRET, IPOPQ, IIADDQ } : valB;
        icode in { IRRMOVQ, IIRMOVQ } : 0;
        # Other instructions don't need ALU
 ];
@@ -173,7 +173,7 @@
 ];

 ## Should the condition codes be updated?
-bool set_cc = icode in { IOPQ };
+bool set_cc = icode in { IOPQ, IIADDQ };

 ################ Memory Stage    ###################################
```
编译与测试
```bash
# 编译
cd seq && make clean && make ssim VERSION=full
# 测试
cd ptest && make SIM=../seq/ssim TFLAGS=-i
# 测试结果
./optest.pl -s ../seq/ssim -i
Simulating with ../seq/ssim
  All 58 ISA Checks Succeed
./jtest.pl -s ../seq/ssim -i
Simulating with ../seq/ssim
  All 96 ISA Checks Succeed
./ctest.pl -s ../seq/ssim -i
Simulating with ../seq/ssim
  All 22 ISA Checks Succeed
./htest.pl -s ../seq/ssim -i
Simulating with ../seq/ssim
  All 756 ISA Checks Succeed
```