* 2.55-2.57 略
* 2.58<br>
```c++
int is_little_endian(){
    int a=1;
    //On little endian machine, a is 0x01000000,
    //char* will get first byte, so return 1.
    //While on little endian machine, a is 0x00000001,
    //function return 0.
    return *(static_cast<char*>(&a));
}
```
* 2.59<br>
```math
C = (x & 0xFF) | (y & ~0xFF)
```
* 2.60<br>
```c++
unsigned replace_byte (unsigned x, int i, unsigned char b){
    return (x & ~(0xFF<<(i<<3))) | (b<<(i<<3));
}
```
* 2.61<br>
```math
A: !~x
B: !x
C: !((i & 0xFF) ^ 0xFF)  &取出最后字节，^判断与FF相同
D: !(x>>(sizeof(int-1)<<3)) x右移n-1字节只保留最高字节
```
* 2.62<br>
```c++
int int_shifts_are_arithmetic()
{
    int a = -1;
    //负数符号位为1，右移n-1位若全为1则为算术右移
    return !~(a >> (sizeof(int) << 3 - 1));
}
```