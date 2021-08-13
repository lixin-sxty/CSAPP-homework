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
* 2.66<br>
```c++
/*Generate mask indicating leftmost 1 in x. Assume w=32.
For example, 0xFF00->0x8000, and 0x6600->0x4000.If x=0, return 0.*/
int leftmost_one(unsigned x){
    x |= (x >> 1); //将最左的1向右扩展1位，下同
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16); // 此时x应为···000111···形式，即最左侧1之后全为1
    return x ^ (x >> 1);
}
```
* 2.67<br>
A:32位机器上不能左移32位<br>
B:令beyond_msb = 2<<31<br>
C:a = 1<<15; b = a<<15; set_msb = b<<1; beyond_msb = b<<2;
* 2.68<br>
```c++
/*Mask with least signficant n bite set to 1
Example: n=6->0x3F, n=17->0x1FFFF. Assume 1<=n<=w
Notice n=w*/
int lower_one_mask(int n){
    return (2 << (n - 1)) - 1;
}
```
* 2.68<br>
```c++
/*Mask with least signficant n bite set to 1
Example: n=6->0x3F, n=17->0x1FFFF. Assume 1<=n<=w
Notice n=w*/
int lower_one_mask(int n){
    return (2 << (n - 1)) - 1;
}
```
* 2.69<br>
```c++
/*Do rotating left shift. Assume 0<=n<w
Example when x = 0x12345678 and w=32:
   n=4->0x23456781, n=20->0x67812345*/
int rotate_left(unsigned x, int n){
    unsigned left = x << n;
    unsigned right = x >> (sizeof(int) * 8 - n - 1) >> 1;
    return left | right;
}
```
* 2.70<br>
```c++
/*Return 1 when x can be represented as an n-bit,
2's-complement number; 0 otherwise. Assume 1<=n<w*/
int fits_bits(int x, int n){
    // 2's-complement就是补码，n为补码表示范围为-2^(n-1)~2^(n-1)-1
    // 若能表示则x的右起第n-1位是符号位，符号位与其左侧的数应为全0(正数)或全1(负数)
    // 因此先右移n-1位，结果应为全0或全1
    x >>= (n - 1);
    return !x || !(~x);
}
```


