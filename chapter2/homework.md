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
* 2.63<br>
```c++
unsigned srl(unsigned x, int k)
{
    unsigned xsra = (int)x >> k;
    // ~0得到全1位模式，左移(int位数-k)位只保留左侧k位为1，再取反得到左侧k位为0的mask
    unsigned mask = ~(~0 << (sizeof(int) * 8 - k));
    // 无符号数逻辑右移左侧补0，与mask进行与操作将左侧k位置0
    return xsra & mask;
}

int sra(int x, int k)
{
    // 实质是将符号位向左填充，方法是取反加1
    // 对于00000，取反加1由于溢出得到00000
    // 对于00001，取反加1得到11111
    int xsrl = (unsigned)x >> k;
    // 通过mask将xsrl分为两部分，符号位及其左侧需要取反加1
    // 右侧保持原样，最后将二者拼接得到正确结果
    int mask = ~0 << (sizeof(int) * 8 - k - 1);
    int left = ~(xsrl & mask) + 1;
    int right = xsrl & ~mask;
    return left | right;
}
```
* 2.64<br>
```c++
/* Return 1 when any odd bit of x equals 1;
0 otherwise. Assume w=32 */
int any_odd_one(unsigned x){
    return !!(x & 0xAAAAAAAA);
}
```
* 2.65<br>
```c++
/*Return 1 when x contains an odd number of 1s;
0 otherwise. Assume w=32 */
int odd_ones(unsigned x){
    // 异或操作可以压缩奇偶性信息，如果有偶数个1则每一位进行异或操作结果为0. 折半减少操作次数
    x ^= (x >> 16);// 将32位奇偶性压缩到右侧16位，下同
    x ^= (x >> 8);
    x ^= (x >> 4);
    x ^= (x >> 2);
    x ^= (x >> 1);
    return x & 1;
}
```