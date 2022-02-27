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
* 4.53<br>
参考 https://dreamanddead.github.io/CSAPP-3e-Solutions/chapter4/4.53/<br>
```bash
$ diff -u pipe-nobypass.hcl.backup pipe-nobypass.hcl
--- pipe-nobypass.hcl.backup    2022-02-27 15:46:08.285395517 +0800
+++ pipe-nobypass.hcl   2022-02-27 16:28:45.022569767 +0800
@@ -304,38 +304,53 @@

 ################ Pipeline Register Control #########################

+# bool data_hazerd =
+#      (d_srcA != RNONE && d_srcA in { e_dstE, E_dstM, M_dstM, M_dstE, W_dstM, W_dstE }) ||
+#      (d_srcB != RNONE && d_srcB in { e_dstE, E_dstM, M_dstM, M_dstE, W_dstM, W_dstE });
+# bool ret =
+#      IRET in { D_icode, E_icode, M_icode };
+# bool jxx_error =
+#      E_icode == IJXX && !e_Cnd;
+
 # Should I stall or inject a bubble into Pipeline Register F?
 # At most one of these can be true.
 bool F_bubble = 0;
-bool F_stall =
+bool F_stall = # (data_hazerd || ret) && !jxx_error
        # Modify the following to stall the update of pipeline register F
-       0 ||
-       # Stalling at fetch while ret passes through pipeline
-       IRET in { D_icode, E_icode, M_icode };
+       (
+               (d_srcA != RNONE && d_srcA in { e_dstE, E_dstM, M_dstM, M_dstE, W_dstM, W_dstE }) ||
+               (d_srcB != RNONE && d_srcB in { e_dstE, E_dstM, M_dstM, M_dstE, W_dstM, W_dstE }) ||
+               IRET in { D_icode, E_icode, M_icode }
+       ) &&
+       !(E_icode == IJXX && !e_Cnd);

 # Should I stall or inject a bubble into Pipeline Register D?
 # At most one of these can be true.
-bool D_stall =
+bool D_stall = # data_hazerd && !jxx_error
        # Modify the following to stall the instruction in decode
-       0;
+       (
+               (d_srcA != RNONE && d_srcA in { e_dstE, E_dstM, M_dstM, M_dstE, W_dstM, W_dstE }) ||
+               (d_srcB != RNONE && d_srcB in { e_dstE, E_dstM, M_dstM, M_dstE, W_dstM, W_dstE })
+       ) &&
+       !(E_icode == IJXX && !e_Cnd);

-bool D_bubble =
-       # Mispredicted branch
+bool D_bubble = # jxx_error || (ret && !data_hazerd)
        (E_icode == IJXX && !e_Cnd) ||
-       # Stalling at fetch while ret passes through pipeline
-       !(E_icode in { IMRMOVQ, IPOPQ } && E_dstM in { d_srcA, d_srcB }) &&
-       # but not condition for a generate/use hazard
-       !0 &&
-         IRET in { D_icode, E_icode, M_icode };
+       (
+               IRET in { D_icode, E_icode, M_icode } &&
+               !(
+                       (d_srcA != RNONE && d_srcA in { e_dstE, E_dstM, M_dstM, M_dstE, W_dstM, W_dstE }) ||
+                       (d_srcB != RNONE && d_srcB in { e_dstE, E_dstM, M_dstM, M_dstE, W_dstM, W_dstE })
+               )
+       );

 # Should I stall or inject a bubble into Pipeline Register E?
 # At most one of these can be true.
 bool E_stall = 0;
-bool E_bubble =
-       # Mispredicted branch
+bool E_bubble = # jxx_error || data_hazerd
        (E_icode == IJXX && !e_Cnd) ||
-       # Modify the following to inject bubble into the execute stage
-       0;
+       (d_srcA != RNONE && d_srcA in { e_dstE, E_dstM, M_dstM, M_dstE, W_dstM, W_dstE }) ||
+       (d_srcB != RNONE && d_srcB in { e_dstE, E_dstM, M_dstM, M_dstE, W_dstM, W_dstE });

 # Should I stall or inject a bubble into Pipeline Register M?
 # At most one of these can be true.
```
编译与测试结果
```bash
# 编译
cd pipe && make clean && make psim VERSION=nobypass
# 测试
cd ptest && make SIM=../pipe/psim
# 测试结果
./optest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 49 ISA Checks Succeed
./jtest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 64 ISA Checks Succeed
./ctest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 22 ISA Checks Succeed
./htest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 600 ISA Checks Succeed
```
* 4.54<br>
参考4.52
```bash
$ diff -u pipe-full.hcl.backup pipe-full.hcl
--- pipe-full.hcl.backup        2022-02-27 16:52:53.375584839 +0800
+++ pipe-full.hcl       2022-02-27 16:52:33.375872084 +0800
@@ -158,7 +158,7 @@
 # Is instruction valid?
 bool instr_valid = f_icode in
        { INOP, IHALT, IRRMOVQ, IIRMOVQ, IRMMOVQ, IMRMOVQ,
-         IOPQ, IJXX, ICALL, IRET, IPUSHQ, IPOPQ };
+         IOPQ, IJXX, ICALL, IRET, IPUSHQ, IPOPQ, IIADDQ };

 # Determine status code for fetched instruction
 word f_stat = [
@@ -171,11 +171,11 @@
 # Does fetched instruction require a regid byte?
 bool need_regids =
        f_icode in { IRRMOVQ, IOPQ, IPUSHQ, IPOPQ,
-                    IIRMOVQ, IRMMOVQ, IMRMOVQ };
+                    IIRMOVQ, IRMMOVQ, IMRMOVQ, IIADDQ };

 # Does fetched instruction require a constant word?
 bool need_valC =
-       f_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL };
+       f_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL, IIADDQ };

 # Predict next value of PC
 word f_predPC = [
@@ -188,21 +188,21 @@

 ## What register should be used as the B source?
 word d_srcB = [
-       D_icode in { IOPQ, IRMMOVQ, IMRMOVQ  } : D_rB;
+       D_icode in { IOPQ, IRMMOVQ, IMRMOVQ, IIADDQ } : D_rB;
        D_icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
        1 : RNONE;  # Don't need register
 ];

 ## What register should be used as the E destination?
 word d_dstE = [
-       D_icode in { IRRMOVQ, IIRMOVQ, IOPQ} : D_rB;
+       D_icode in { IRRMOVQ, IIRMOVQ, IOPQ, IIADDQ } : D_rB;
        D_icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
        1 : RNONE;  # Don't write any register
 ];
@@ -239,7 +239,7 @@
 ## Select input A to ALU
 word aluA = [
        E_icode in { IRRMOVQ, IOPQ } : E_valA;
-       E_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ } : E_valC;
+       E_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IIADDQ } : E_valC;
        E_icode in { ICALL, IPUSHQ } : -8;
        E_icode in { IRET, IPOPQ } : 8;
        # Other instructions don't need ALU
@@ -248,7 +248,7 @@
 ## Select input B to ALU
 word aluB = [
        E_icode in { IRMMOVQ, IMRMOVQ, IOPQ, ICALL,
-                    IPUSHQ, IRET, IPOPQ } : E_valB;
+                    IPUSHQ, IRET, IPOPQ, IIADDQ } : E_valB;
        E_icode in { IRRMOVQ, IIRMOVQ } : 0;
        # Other instructions don't need ALU
 ];
@@ -260,7 +260,7 @@
 ];

 ## Should the condition codes be updated?
-bool set_cc = E_icode == IOPQ &&
+bool set_cc = E_icode in { IOPQ, IIADDQ } &&
        # State changes only during normal operation
        !m_stat in { SADR, SINS, SHLT } && !W_stat in { SADR, SINS, SHLT };
```
编译与测试结果
```bash
# 编译
cd pipe && make clean && make psim VERSION=full
# 测试
cd ptest && make SIM=../pipe/psim TFLAGS=-i
# 测试结果
./optest.pl -s ../pipe/psim -i
Simulating with ../pipe/psim
  All 58 ISA Checks Succeed
./jtest.pl -s ../pipe/psim -i
Simulating with ../pipe/psim
  All 96 ISA Checks Succeed
./ctest.pl -s ../pipe/psim -i
Simulating with ../pipe/psim
  All 22 ISA Checks Succeed
./htest.pl -s ../pipe/psim -i
Simulating with ../pipe/psim
  All 756 ISA Checks Succeed
```
* 4.55<br>
```bash
$ diff -u pipe-nt.hcl.backup pipe-nt.hcl
--- pipe-nt.hcl.backup  2022-02-27 17:01:04.592164685 +0800
+++ pipe-nt.hcl 2022-02-27 17:36:20.193727769 +0800
@@ -139,7 +139,7 @@
 ## What address should instruction be fetched at
 word f_pc = [
        # Mispredicted branch.  Fetch at incremented PC
-       M_icode == IJXX && !M_Cnd : M_valA;
+       M_icode == IJXX && M_ifun != UNCOND && M_Cnd : M_valA;
        # Completion of RET instruction
        W_icode == IRET : W_valM;
        # Default: Use predicted value of PC
@@ -183,7 +183,7 @@
 # Predict next value of PC
 word f_predPC = [
        # BNT: This is where you'll change the branch prediction rule
-       f_icode in { IJXX, ICALL } : f_valC;
+       f_icode == ICALL || (f_icode == IJXX && f_ifun == UNCOND): f_valC;
        1 : f_valP;
 ];
@@ -273,7 +273,10 @@
        !m_stat in { SADR, SINS, SHLT } && !W_stat in { SADR, SINS, SHLT };

 ## Generate valA in execute stage
-word e_valA = E_valA;    # Pass valA through stage
+word e_valA = [
+       E_icode == IJXX && E_ifun != UNCOND : E_valC;
+       1 : E_valA;
+];

 ## Set dstE to RNONE in event of not-taken conditional move
 word e_dstE = [
@@ -336,14 +339,14 @@

 bool D_bubble =
        # Mispredicted branch
-       (E_icode == IJXX && !e_Cnd) ||
+       (E_icode == IJXX && E_ifun != UNCOND && e_Cnd) ||
        # Stalling at fetch while ret passes through pipeline
        # but not condition for a load/use hazard
        !(E_icode in { IMRMOVQ, IPOPQ } && E_dstM in { d_srcA, d_srcB }) &&
@@ -354,7 +357,7 @@
 bool E_stall = 0;
 bool E_bubble =
        # Mispredicted branch
-       (E_icode == IJXX && !e_Cnd) ||
+       (E_icode == IJXX && E_ifun != UNCOND && e_Cnd) ||
        # Conditions for a load/use hazard
        E_icode in { IMRMOVQ, IPOPQ } &&
         E_dstM in { d_srcA, d_srcB};
```
编译与测试结果
```bash
# 编译
cd pipe && make clean && make psim VERSION=nt
# 测试
cd ptest && make SIM=../pipe/psim
# 测试结果
./optest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 49 ISA Checks Succeed
./jtest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 64 ISA Checks Succeed
./ctest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 22 ISA Checks Succeed
./htest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 600 ISA Checks Succeed
```
* 4.56<br>
参考https://dreamanddead.github.io/CSAPP-3e-Solutions/chapter4/4.56/
```bash
$ diff -u pipe-btfnt.hcl.backup pipe-btfnt.hcl
--- pipe-btfnt.hcl.backup       2022-02-27 17:57:54.780436720 +0800
+++ pipe-btfnt.hcl      2022-02-27 18:34:06.161856899 +0800
@@ -139,7 +139,8 @@
 ## What address should instruction be fetched at
 word f_pc = [
        # Mispredicted branch.  Fetch at incremented PC
-       M_icode == IJXX && !M_Cnd : M_valA;
+       M_icode == IJXX && M_ifun != UNCOND && M_valE < M_valA && !M_Cnd : M_valA;
+       M_icode == IJXX && M_ifun != UNCOND && M_valE >= M_valA && M_Cnd : M_valE;
        # Completion of RET instruction
        W_icode == IRET : W_valM;
        # Default: Use predicted value of PC
@@ -159,7 +160,7 @@
 ];

 # Does fetched instruction require a constant word?
@@ -183,7 +184,9 @@
 # Predict next value of PC
 word f_predPC = [
        # BBTFNT: This is where you'll change the branch prediction rule
-       f_icode in { IJXX, ICALL } : f_valC;
+       f_icode == ICALL ||
+       (f_icode == IJXX && f_ifun == UNCOND) ||
+       (f_icode == IJXX && f_ifun != UNCOND && f_valC < f_valP) : f_valC;
        1 : f_valP;
 ];

@@ -243,11 +246,11 @@
 # BBTFNT: When some branches are predicted as not-taken, you need some
 # way to get valC into pipeline register M, so that
 # you can correct for a mispredicted branch.
-
+# pass valC by M_valE, pass valP by M_valA
 ## Select input A to ALU
 word aluA = [
        E_icode in { IRRMOVQ, IOPQ } : E_valA;
-       E_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ } : E_valC;
+       E_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX } : E_valC;
        E_icode in { ICALL, IPUSHQ } : -8;
        E_icode in { IRET, IPOPQ } : 8;
        # Other instructions don't need ALU
@@ -255,9 +258,9 @@

 ## Select input B to ALU
 word aluB = [
-       E_icode in { IRMMOVQ, IMRMOVQ, IOPQ, ICALL,
+       E_icode in { IRMMOVQ, IMRMOVQ, IOPQ, ICALL,
                     IPUSHQ, IRET, IPOPQ } : E_valB;
-       E_icode in { IRRMOVQ, IIRMOVQ } : 0;
+       E_icode in { IRRMOVQ, IIRMOVQ, IJXX } : 0;
        # Other instructions don't need ALU
 ];

@@ -336,14 +339,17 @@

 bool D_bubble =
        # Mispredicted branch
-       (E_icode == IJXX && !e_Cnd) ||
+       (
+               E_icode == IJXX && E_ifun != UNCOND &&
+               ((E_valC < E_valA && !e_Cnd) ||(E_valC >= E_valA && e_Cnd))
+       ) ||
        # BBTFNT: This condition will change
        # Stalling at fetch while ret passes through pipeline
        # but not condition for a load/use hazard
@@ -355,7 +361,10 @@
 bool E_stall = 0;
 bool E_bubble =
        # Mispredicted branch
-       (E_icode == IJXX && !e_Cnd) ||
+       (
+               E_icode == IJXX && E_ifun != UNCOND &&
+               ((E_valC < E_valA && !e_Cnd) ||(E_valC >= E_valA && e_Cnd))
+       ) ||
        # BBTFNT: This condition will change
        # Conditions for a load/use hazard
        E_icode in { IMRMOVQ, IPOPQ } &&
```
编译与测试结果
```bash
# 编译
cd pipe && make clean && make psim VERSION=btfnt
# 测试
cd ptest && make SIM=../pipe/psim
# 测试结果
./optest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 49 ISA Checks Succeed
./jtest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 64 ISA Checks Succeed
./ctest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 22 ISA Checks Succeed
./htest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 600 ISA Checks Succeed
```
* 4.57<br>

A:
```bash
bool load_use = [
  E_icode in { IMRMOVQ, IPOPQ } && (E_dstM == d_srcB || (E_dstM == d_srcA &&  !(D_icode in { IPUSHQ, IRMMOVQ })))
]
```
B:
```bash
$ diff -u pipe-lf.hcl.backup pipe-lf.hcl
--- pipe-lf.hcl.backup  2014-12-29 23:08:40.000000000 +0800
+++ pipe-lf.hcl 2022-02-27 19:13:55.096629583 +0800
@@ -267,10 +267,11 @@
        !m_stat in { SADR, SINS, SHLT } && !W_stat in { SADR, SINS, SHLT };

 ## Generate valA in execute stage
 ## LB: With load forwarding, want to insert valM
 ##   from memory stage when appropriate
 ## Here it is set to the default used in the normal pipeline
 word e_valA = [
+       E_srcA == M_dstM && E_icode in { IPUSHQ, IRMMOVQ }: m_valM;
        1 : E_valA;  # Use valA from stage pipe register
 ];

@@ -329,23 +330,30 @@
 bool F_stall =
        # Conditions for a load/use hazard
        ## Set this to the new load/use condition
-       0 ||
+       (
+               E_icode in { IMRMOVQ, IPOPQ } &&
+               (E_dstM == d_srcB || (E_dstM == d_srcA &&  !(D_icode in { IPUSHQ, IRMMOVQ })))
+       ) ||
        # Stalling at fetch while ret passes through pipeline
        IRET in { D_icode, E_icode, M_icode };

 # Should I stall or inject a bubble into Pipeline Register D?
 # At most one of these can be true.
-bool D_stall =
+bool D_stall =
        # Conditions for a load/use hazard
        ## Set this to the new load/use condition
-       0;
+       E_icode in { IMRMOVQ, IPOPQ } &&
+       (E_dstM == d_srcB || (E_dstM == d_srcA &&  !(D_icode in { IPUSHQ, IRMMOVQ })));

 bool D_bubble =
        # Mispredicted branch
        (E_icode == IJXX && !e_Cnd) ||
        # Stalling at fetch while ret passes through pipeline
        # but not condition for a load/use hazard
-       !(E_icode in { IMRMOVQ, IPOPQ } && E_dstM in { d_srcA, d_srcB }) &&
+       !(
+               E_icode in { IMRMOVQ, IPOPQ } &&
+               (E_dstM == d_srcB || (E_dstM == d_srcA &&  !(D_icode in { IPUSHQ, IRMMOVQ })))
+       ) &&
          IRET in { D_icode, E_icode, M_icode };

 # Should I stall or inject a bubble into Pipeline Register E?
@@ -356,7 +364,10 @@
        (E_icode == IJXX && !e_Cnd) ||
        # Conditions for a load/use hazard
        ## Set this to the new load/use condition
-       0;
+       (
+               E_icode in { IMRMOVQ, IPOPQ } &&
+               (E_dstM == d_srcB || (E_dstM == d_srcA &&  !(D_icode in { IPUSHQ, IRMMOVQ })))
+       );

 # Should I stall or inject a bubble into Pipeline Register M?
 # At most one of these can be true.
```
编译与测试结果
```bash
# 编译
cd pipe && make clean && make psim VERSION=lf
# 测试
cd ptest && make SIM=../pipe/psim
# 测试结果
./optest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 49 ISA Checks Succeed
./jtest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 64 ISA Checks Succeed
./ctest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 22 ISA Checks Succeed
./htest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 600 ISA Checks Succeed
```
* 4.58<br>
```bash
$ diff -u pipe-1w.hcl.backup pipe-1w.hcl
--- pipe-1w.hcl.backup  2022-02-27 19:26:20.128154559 +0800
+++ pipe-1w.hcl 2022-02-27 19:51:07.126357445 +0800
@@ -152,11 +152,12 @@
 ];

 ## Determine icode of fetched instruction
 ## 1W: To split ipopq into two cycles, need to be able to
 ## modify value of icode,
 ## so that it will be IPOP2 when fetched for second time.
 word f_icode = [
        imem_error : INOP;
+       D_icode == IPOPQ: IPOP2;
        1: imem_icode;
 ];

@@ -167,9 +168,9 @@
 ];

 # Is instruction valid?
 bool instr_valid = f_icode in
        { INOP, IHALT, IRRMOVQ, IIRMOVQ, IRMMOVQ, IMRMOVQ,
-         IOPQ, IJXX, ICALL, IRET, IPUSHQ, IPOPQ };
+         IOPQ, IJXX, ICALL, IRET, IPUSHQ, IPOPQ, IPOP2 };

 # Determine status code for fetched instruction
 word f_stat = [
@@ -181,17 +182,18 @@

 # Does fetched instruction require a regid byte?
 bool need_regids =
-       f_icode in { IRRMOVQ, IOPQ, IPUSHQ, IPOPQ,
-                    IIRMOVQ, IRMMOVQ, IMRMOVQ };
+       f_icode in { IRRMOVQ, IOPQ, IPUSHQ, IPOPQ,
+                    IIRMOVQ, IRMMOVQ, IMRMOVQ, IPOP2 };

 # Predict next value of PC
 word f_predPC = [
        f_icode in { IJXX, ICALL } : f_valC;
        ## 1W: Want to refetch popq one time
+       f_icode == IPOPQ: f_pc;
        1 : f_valP;
 ];

@@ -204,14 +206,14 @@
 ## What register should be used as the A source?
 word d_srcA = [
        D_icode in { IRRMOVQ, IRMMOVQ, IOPQ, IPUSHQ  } : D_rA;
-       D_icode in { IPOPQ, IRET } : RRSP;
+       D_icode in { IRET } : RRSP;
        1 : RNONE; # Don't need register
 ];

 ## What register should be used as the B source?
 word d_srcB = [
        D_icode in { IOPQ, IRMMOVQ, IMRMOVQ  } : D_rB;
-       D_icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
+       D_icode in { IPUSHQ, IPOPQ, ICALL, IRET, IPOP2 } : RRSP;
        1 : RNONE;  # Don't need register
 ];

@@ -224,7 +226,7 @@

 ## What register should be used as the M destination?
 word d_dstM = [
-       D_icode in { IMRMOVQ, IPOPQ } : D_rA;
+       D_icode in { IMRMOVQ, IPOP2 } : D_rA;
        1 : RNONE;  # Don't write any register
 ];

@@ -255,15 +257,15 @@
 word aluA = [
        E_icode in { IRRMOVQ, IOPQ } : E_valA;
        E_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ } : E_valC;
-       E_icode in { ICALL, IPUSHQ } : -8;
+       E_icode in { ICALL, IPUSHQ, IPOP2 } : -8;
        E_icode in { IRET, IPOPQ } : 8;
        # Other instructions don't need ALU
 ];

 ## Select input B to ALU
 word aluB = [
-       E_icode in { IRMMOVQ, IMRMOVQ, IOPQ, ICALL,
-                    IPUSHQ, IRET, IPOPQ } : E_valB;
+       E_icode in { IRMMOVQ, IMRMOVQ, IOPQ, ICALL,
+                    IPUSHQ, IRET, IPOPQ, IPOP2 } : E_valB;
        E_icode in { IRRMOVQ, IIRMOVQ } : 0;
        # Other instructions don't need ALU
 ];
@@ -292,13 +294,13 @@

 ## Select memory address
 word mem_addr = [
-       M_icode in { IRMMOVQ, IPUSHQ, ICALL, IMRMOVQ } : M_valE;
-       M_icode in { IPOPQ, IRET } : M_valA;
+       M_icode in { IRMMOVQ, IPUSHQ, ICALL, IMRMOVQ, IPOP2 } : M_valE;
+       M_icode in { IRET } : M_valA;
        # Other instructions don't need address
 ];

 ## Set read control signal
-bool mem_read = M_icode in { IMRMOVQ, IPOPQ, IRET };
+bool mem_read = M_icode in { IMRMOVQ, IPOP2, IRET };

 ## Set write control signal
 bool mem_write = M_icode in { IRMMOVQ, IPUSHQ, ICALL };
@@ -350,16 +352,16 @@
 bool F_bubble = 0;
 bool F_stall =
        # Conditions for a load/use hazard
-       E_icode in { IMRMOVQ, IPOPQ } &&
+       E_icode in { IMRMOVQ, IPOP2 } &&
         E_dstM in { d_srcA, d_srcB } ||
        # Stalling at fetch while ret passes through pipeline
        IRET in { D_icode, E_icode, M_icode };

 # Should I stall or inject a bubble into Pipeline Register D?
 # At most one of these can be true.
-bool D_stall =
+bool D_stall =
        # Conditions for a load/use hazard
-       E_icode in { IMRMOVQ, IPOPQ } &&
+       E_icode in { IMRMOVQ, IPOP2 } &&
         E_dstM in { d_srcA, d_srcB };

 bool D_bubble =
@@ -367,7 +369,7 @@
        (E_icode == IJXX && !e_Cnd) ||
        # Stalling at fetch while ret passes through pipeline
        # but not condition for a load/use hazard
-       !(E_icode in { IMRMOVQ, IPOPQ } && E_dstM in { d_srcA, d_srcB }) &&
+       !(E_icode in { IMRMOVQ, IPOP2 } && E_dstM in { d_srcA, d_srcB }) &&
        # 1W: This condition will change
          IRET in { D_icode, E_icode, M_icode };

@@ -378,7 +380,7 @@
        # Mispredicted branch
        (E_icode == IJXX && !e_Cnd) ||
        # Conditions for a load/use hazard
-       E_icode in { IMRMOVQ, IPOPQ } &&
+       E_icode in { IMRMOVQ, IPOP2 } &&
         E_dstM in { d_srcA, d_srcB};

 # Should I stall or inject a bubble into Pipeline Register M?
```
编译与测试结果
```bash
# 编译
cd pipe && make clean && make psim VERSION=1w
# 测试
cd ptest && make SIM=../pipe/psim
# 测试结果
./optest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 49 ISA Checks Succeed
./jtest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 64 ISA Checks Succeed
./ctest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 22 ISA Checks Succeed
./htest.pl -s ../pipe/psim
Simulating with ../pipe/psim
  All 600 ISA Checks Succeed
```
* 4.59<br>
```bash
# 4.47
L4:
  mrmovq	8(%rax), %rdx   # Get *(i+1)
	mrmovq	(%rax), %rcx    # Get *i
  rrmovq  %rdx, %r11
	subq	%rcx, %r11
	jge	L3
	rmmovq	%rcx, 8(%rax)   # swap *(i+1) and *i
	rmmovq	%rdx, (%rax)
# 4.48
L4:
  mrmovq	8(%rax), %rdx   # Get *(i+1)
	mrmovq	(%rax), %rcx    # Get *i
  rrmovq  %rdx, %r11
	subq	%rcx, %r11
	cmovge  %rcx, %r12
	cmovge  %rdx, %rcx
	cmovge  %r12, %rdx
	rmmovq	%rcx, 8(%rax)
	rmmovq	%rdx, (%rax)
# 4.49
L4:
  mrmovq	8(%rax), %rdx   # Get *(i+1)
	mrmovq	(%rax), %rcx    # Get *i
  rrmovq  %rdx, %r11
	subq	%rcx, %r11
	rrmovq  %rcx, %r12		# origin rcx in r12
	xorq    %rdx, %rcx
	cmovge  %r12, %rdx		# change rdx to origin rcx
	xorq    %rcx, %rdx
	xorq    %rdx, %rcx
	rmmovq	%rcx, 8(%rax)
	rmmovq	%rdx, (%rax)
```
4.47中存在条件跳转，其中预测正确的概率为50%。正确时执行指令数为5，错误时执行指令数为7+2=9（添加两条nop指令）。因此每次循环平均执行指令数为5×0.5+9×0.5=7<br>
4.48中每个循环执行指令数相同，为9<br>
4.48中每个循环执行指令数相同，为11<br>
因此4.47的方法效率最高。这里使用条件传送指令效率更低是因为条件跳转预测错误的代价只有两条指令。当错误代价更大时使用条件传送指令效率更高