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
* 2.71<br>
A.<br>
返回值是unsigned，没有正确转换为int类型<br>
B.<br>
```c++
/*Declaration of data type where 4 bytes are packed in to an unsigned*/
typedef unsigned packed_t;

/*Extract byte from word. Return as signed integer*/
int xbyte(packed_t word,int bytenums){
    // 先将需要的字节左移至最高位，转为int类型，然后右移至最低位即可
    // int类型右移执行算术右移
    return (int)(word << ((3 - bytenums) << 3)) >> 24;
}
```
* 2.72<br>
A.<br>
sizeof()返回一个unsigned类型数，计算减法后的结果也是一个unsigned类型值，始终不小于0，条件始终成立<br>
B.<br>
```c++
/*Copy integer into buffer if space is available*/
void copy_int(int val, void *buf, int maxbytes){
    if (maxbytes > 0 && maxbytes >= sizeof(val)){
        memcpy(buf, (void *)&val, sizeof(val));
    }
}
```
* 2.73<br>
```c++
/*Addition that saturates to TMin or TMax*/
int saturating_add(int x, int y)
{
    // int加法溢出可以根据加数与结果的符号关系确定
    size_t w = sizeof(int) << 3;
    int sum = x + y;
    int t = x + y;
    // 取两个加数和结果的符号位
    x >>= (w - 1);
    y >>= (w - 1);
    t >>= (w - 1);
    // 若两个加数均为正，结果为负，即发生正溢出
    int pos_ovf = ~x & ~y & t;
    // 若两个加数均为负，结果为正，即发生负溢出
    int neg_ovf = x & y & ~t;
    // 否则未发生溢出
    int novf = ~(pos_ovf | neg_ovf);
    return (pos_ovf & INT32_MAX) | (novf & sum) | (neg_ovf & INT32_MIN);
}
```
* 2.74<br>
```c++
/* Determine whether arguments can be subtracted without overflow */
int tsub_ok(int x, int y) {
  int t = x - y;
  size_t w = sizeof(int) << 3;
  // 取两个数和结果的符号位
  x >>= (w - 1);
  y >>= (w - 1);
  t >>= (w - 1);
  // 若两个数同号则不会溢出；若异号则结果与被减数同号时表示未溢出
  return (x == y) || (x == t);
}
```
* 2.75<br>
取结果的高位可以用右移w位得到，即$/2^w$. 模仿公式2.18可以得到
$$
(x'*y')/2^w=[(x+x_{w-1}2^w)*(y+y_{w-1}2^w)]/2^w=\\
(x*y)/2^w+x_{w-1}y+xy_{w-1}
$$
其中$(x'*y')/2^w$即无符号数乘积的高w位，$(x*y)/2^w$为有符号数乘积的高w位。又$x_{w-1}$和$y_{w-1}$分别表示二者的最高位即符号位，可得到如下函数
```c++
unsigned unsigned_high_prod(unsigned x, unsigned y) {
  unsigned t = (unsigned)signed_high_prod((int)x, (int)y);
  if ((int)x < 0) t += y;
  if ((int)y < 0) t += x;
  return t;
}
```
* 2.76<br>
```c++
void* calloc(size_t nmemb, size_t size) {
  size_t sz = nmemb * size;
  // sz / nmemb != size means overflow
  if (sz == 0 || sz / nmemb != size) {
    return NULL;
  }
  void* p = malloc(sz);
  if (p) {
    memset(p, 0, sz);
  }
  return p;
}
```
* 2.77<br>
A: x * 17 == (x << 4) + x<br>
B: x * -7 == x - (x << 3)<br>
C: x * 60 == (x << 6) - (x << 1)<br>
D: x * -112 == (x << 4) - (x << 7)<br>
* 2.78<br>
```c++
/*Devide by power of 2. Assume 0<=k<w-1 */
int divide_power2(int x, int k) {
  // INT_MIN: 100..00
  int is_neg = x & INT_MIN;  // get symbol bit
  // if x is negetive, add (1 << k) - 1 first
  (is_neg && (x = x + (1 << k) - 1));
  return x >> k;
}
```
* 2.79<br>
利用2.78函数处理除法计算
```c++
int mul3div4(int x) {
  x = x + (x << 1);
  return divide_power2(x, 2);
}
```