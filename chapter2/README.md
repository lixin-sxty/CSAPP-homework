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
* 2.80<br>
参考CSAPP-3e-Solutions<br>
x先除以4后乘3，结果不会溢出。但右移2位会导致低2位数据丢失，因此低2位需要单独考虑<br>
前30位不存在溢出问题，只要将计算结果与后2位计算结果相加即可
```c++
int threefourths(int x) {
  // Get higher 30 bits as h, 2 lower bits as l
  int h = x & ~0x3;
  int l = x & 0x3;
  // Get symbol bit
  int is_neg = x & INT32_MIN;
  // l = l*3
  l = (l << 1) + l;
  // If x is negetive, l need extra operation to round to zero
  // l + (1 << 2) - 1
  (is_neg && (l += 3));
  l >>= 2;
  // h = h/4*3
  h >>= 2;
  h = (h << 1) + h;

  return l + h;
}
```
* 2.81<br>
```c++
A: -1 << k;
B: (-1 << j) ^ (-1 << k)
```
* 2.82<br>
A:x=INT_MIN,y=0<br>
B:True<br>
C:True<br>
D:True<br>
E:True<br>
* 2.83<br>
A:n=Y/(2^k-1)<br>
B:<br>
a:5/7<br>
b:2/5<br>
c:19/63<br>
* 2.84<br>
```c++
int float_le(float x, float y) {
  // f2u is a function that convert float to unsigned
  unsigned ux = f2u(x);
  unsigned uy = f2u(y);
  // Get the sign bits
  unsigned sx = ux >> 31;
  unsigned sy = uy >> 31;
  // Given an expression using only ux, uy, sx and sy
  return (ux << 1 == 0 && uy << 1 == 0) ||  // x=0 y=0 return true
         (sx && !sy) ||                     // x<0 y>=0 return true
         (sx && sy && ux >= uy) ||          // x>=0 y>=0 return ux>=uy
         (!sx && !sy && ux <= uy);          // x<0 y<0 return ux<=uy
}
```
* 2.85<br>
$$
V=(-1)^s\times M\times 2^E
$$
其中V为表示的小数，s为符号位，由浮点数第一位表示，0正1负；E为阶码，由中间的k位二进制码组成的e阶码部分表示；M为尾数，是一个二进制小数，由最后n位二进制码组成的小数f表示。它们具有以下关系：
1. 规格化数：e不全为1或0
   1. M=frac+1
   2. E=e-bias
2. 非规格化数：e全为0
   1. M=frac
   2. E=1-bias
   3. 位模式为全0的浮点数表示+0，而只有符号位为1的浮点数表示-0
3. 特殊值：e全为1<br>
   当小数域全为0时，根据符号为不同分别表示+∞和-∞；当小数域为非零时， 表示"NaN"<br>

bias = 2^(k-1)-1<br>
| 描述               | V          | M         | E      | f         | e             | bit                |
| ------------------ | ---------- | --------- | ------ | --------- | ------------- | ------------------ |
| 7.0                | 7.0        | 0b1.11    | 2      | 0b0.11    | 2^(k-1)+1     | 0 10···01 110···0  |
| 最大奇整数         | 2^(n+1)-1  | 0b1.1···1 | n      | 0b0.1···1 | n+bias        | 0 n+bias 111···111 |
| 最小规格化数       | 2^(1-bias) | 0b1.0     | 1-bias | 0b0.0     | 1             | 0 1-bias 0···0     |
| 最小规格化数的倒数 | 2^(bias-1) | 0b1.0     | bias-1 | 0b0.0     | 2bias-1=2^k-3 | 0 1···101 00···00  |
* 2.86<br>
扩展浮点精度共80位，1个符号位，15个阶码位，1个整数位，63个小数位<br>
bias = 2^14-1<br>

| 描述               | 位表示            | 十进制数值            |
| ------------------ | ----------------- | --------------------- |
| 最小的正非规格化数 | 0 0···0 0 0···01  | $2^{-16445}$          |
| 最小的正规格化数   | 0 0···01 1 0···00 | $2^{-16382}$          |
| 最大的规格化数     | 0 1···10 1 1···1  | $2^{16384}-2^{16320}$ |

* 2.87<br>
半精度浮点数：1个符号位，5个阶码位，10个小数位，bias=15

| 描述                   | Hex  | M                   | E   | V                     | D               |
| ---------------------- | ---- | ------------------- | --- | --------------------- | --------------- |
| -0                     | 8000 | 0                   | -14 | -0                    | -0.0            |
| 最小的大于2的值        | 4001 | $1\frac{1}{1024}$   | 1   | $2\frac{1}{512}$      | 2.001953125     |
| 512                    | 6000 | 1                   | 9   | 512                   | 512.0           |
| 最大的非规格化数       | 03FF | $\frac{1023}{1024}$ | -14 | $\frac{1023}{2^{24}}$ | 0.0000609755516 |
| -∞                     | FC00 | ——                  | ——  | -∞                    | -∞              |
| 十六进制表示为3BB0的数 | 3BB0 | $1\frac{59}{64}$    | -1  | $\frac{123}{128}$     | 0.9609375       |
* 2.88<br>
左侧为格式A：1+5（阶码）+3（小数）,bias=15<br>
右侧为格式B：1+4（阶码）+4（小数）,bias=7<br>

| 位          | 值                  | 位          | 值                  |
| ----------- | ------------------- | ----------- | ------------------- |
| 1 01110 001 | $\frac{-9}{16}$     | 1 0110 0010 | $\frac{-9}{16}$     |
| 0 10110 101 | 208                 | 0 1110 1010 | 208                 |
| 1 00111 110 | $\frac{-7}{2^{10}}$ | 1 0000 0111 | $\frac{-7}{2^{10}}$ |
| 0 00000 101 | $\frac{5}{2^{17}}$  | 0 0000 0001 | $\frac{1}{2^{10}}$  |
| 1 11011 000 | -4096               | 1 1110 1111 | -248                |
| 0 11000 100 | 768                 | 0 1111 0000 | ∞                   |
* 2.89<br>
A:true.32位整型可以被double精确表示<br>
B:x=INT_MIN,y=INT_MAX,x-y溢出，dx-dy不溢出，两边不同<br>
C:true，int类型加法在double表示下不会溢出，满足交换律<br>
D:x=INT_MAX,y=INT_MAX-101,z=INT_MAX-101.double类型无法精确表示较大的整数，不同的计算顺序可能导致舍入结果不一致<br>
E:x=0,y=1.dx/dx=NaN,dy/dy=1<br>
* 2.90<br>
```c++
float fpwr2(int x) {
  /*Result exponent and fraction */
  unsigned exp, frac;
  unsigned u;
  if (x < -149) {
    /*Too small, return 0.0*/
    exp = 0;
    frac = 0;
  } else if (x < -126) {
    /*Denormalized result*/
    exp = 0;
    frac = 1 << (x + 149);
  } else if (x < 128) {
    /*Normalized result*/
    exp = x + 127;
    frac = 0;
  } else {
    /*Too big, return +∞*/
    exp = 255;
    frac = 0;
  }

  /*Pack exp and frac into 32 bits*/
  u = exp << 23 | frac;
  return u2f(u);
}
```
* 2.91<br>
π的单精度浮点近似表示为：0x40490FDB<br>
A:上述表示的位模式为0 100000000 10010010000111111011011，E=128-127=1,M=1.10010010000111111011011,二进制小数为M*2=11.00100100001111110110110<br>
B:$\frac{22}{7}=3\frac{1}{7}$,根据2.83，Y=1,k=3,无穷串为001。22/7的二进制小数为11.001 001 001 ···<br>
C：二者从第9位小数开始不同<br>
* 2.92<br>
```c++
/* compute -f. If f is NaN, then return f. */
float_bits float_negate(float_bits f) {
  unsigned sign = f >> 31;
  unsigned exp = f >> 23 & 0xFF;
  unsigned frac = f & 0x7FFFFF;
  if (exp == 0xFF && frac != 0) {
    return f;
  }
  return ((-sign) << 31) | (exp << 23) | frac;
}
```
* 2.93<br>
```c++
/* compute |f|. If f is NaN, then return f. */
float_bits float_absval(float_bits f) {
  unsigned sign = f >> 31;
  unsigned exp = f >> 23 & 0xFF;
  unsigned frac = f & 0x7FFFFF;
  if (exp == 0xFF && frac != 0) {
    return f;
  }
  return (0 << 31) | (exp << 23) | frac;
}
```
* 2.94<br>
```c++
/* compute 2*f. If f is NaN, then return f. */
float_bits float_twice(float_bits f) {
  unsigned sign = f >> 31;
  unsigned exp = f >> 23 & 0xFF;
  unsigned frac = f & 0x7FFFFF;
  // f is NaN or Inf
  if (exp == 0xFF) {
    return f;
  }
  if (exp == 0x00){
    frac <<= 1;
  }else if(exp == 0xFF - 1){
    exp = 0xFF;
    frac = 0;
  }else{
    exp += 1;
  }
  return (sign << 31) | (exp << 23) | frac;
}
```
* 2.95<br>
参考[CSAPP-3e-Solutions](https://dreamanddead.github.io/CSAPP-3e-Solutions/chapter2/2.95/)
```c++
/* compute 0.5*f. If f is NaN, then return f. */
float_bits float_half(float_bits f) {
  unsigned sign = f >> 31;
  unsigned exp = f >> 23 & 0xFF;
  unsigned frac = f & 0x7FFFFF;
  unsigned rest = f & 0x7FFFFFFF;
  if (exp == 0xFF) {
    return f;
  }
  /* Round to even, think about last 2 bits of frac
    00 => 0
    01 => 0
    10 => 1
    11 => 10 (round to even) need >>1 and plus 1*/
  int addition = (frac & 0x11) == 0x11;
  if (exp != 0x00) {
    exp -= 1;
  } else {
    frac >>= 1;
  }

  if (exp == 0) {
    frac >>= 1;
    frac += addition;
  } else if (exp == 1) {
    /* Normalized to denormalized */
    rest >>= 1;
    rest += addition;
    exp = rest >> 23 & 0xFF;
    frac = rest & 0x7FFFFF;
  } else {
    exp -= 1;
  }
  return (0 << 31) | (exp << 23) | frac;
}
```
* 2.96<br>
* 参考[CSAPP-3e-Solutions](https://dreamanddead.github.io/CSAPP-3e-Solutions/chapter2/2.96/)
当0<=f<1, 返回0<br>
当f>=2^31, 返回无穷大<br>
其他情况返回(int)f
```c++
/* compute (int)f. If converion causes overflow or f is NaN, return 0x80000000. */
int float_f2i(float_bits f) {
  unsigned sig = f >> 31;
  unsigned exp = f >> 23 & 0xFF;
  unsigned frac = f & 0x7FFFFF;
  unsigned bias = 0x7F;

  int num;
  unsigned E;
  unsigned M;

  // 0 <= |f| < 1
  if (exp >= 0 && exp - bias < 0) {
    /* number less than 1 */
    num = 0;
  // |f| >= 2^31 or f is ∞
  } else if (exp - bias >= 31) {
    num = 0x80000000;
  // other. V = M * 2^E
  } else {
    E = exp - bias;
    M = frac | 0x800000;
    if (E > 23) {
      num = M << (E - 23);
    } else {
      num = M >> (23 - E);
    }
  }
  return sig ? -num : num;
}
```
* 2.97<br>
* 参考[CSAPP-3e-Solutions](https://dreamanddead.github.io/CSAPP-3e-Solutions/chapter2/2.96/)
```c++
size_t bits_length(unsigned i) {
  size_t n = 0;
  while (i) {
    i >>= 1;
    ++n;
  }
  return n;
}

/* compute (float)i */
float_bits i2f(int i) {
  unsigned sig = 0, exp, frac, bias = 0x7F;
  // special value 0 and INT_MIN
  if (i == 0) {
    sig = 0;
    exp = 0;
    frac = 0;
    return sig << 31 | exp << 23 | frac;
  }
  if (i == INT32_MIN) {
    sig = 1;
    exp = bias + 31;
    frac = 0;
    return sig << 31 | exp << 23 | frac;
  }
  // if i is negative, convert it to positive
  if (i < 0) {
    sig = 1;
    i = -i;
  }
  // 0111->3; 010101->5
  unsigned bits = bits_length(i);
  // in float, 11111->1.1111*2^4, fbits = bits - 1
  unsigned fbits = bits - 1;
  // exp in float need add bias
  exp = bias + fbits;
  // get decimal part of float
  unsigned rest = i & ((1 << fbits) - 1);
  if (fbits <= 23) {
    frac = rest << (23 - fbits);
    exp = exp << 23 | frac;
  } else {
    unsigned offset = fbits - 23;
    unsigned round_mid = 1 << (offset - 1);

    unsigned round_part = rest & ((1 << offset) - 1);
    frac = rest >> offset;
    exp = exp << 23 | frac;

    if (round_part > round_mid) {
      exp += 1;
    } else if (round_part == round_mid) {
      // round to even
      if ((frac & 0x1) == 1) {
        exp += 1;
      }
    }
  }

  return sig << 31 | exp;
}

int main(int argc, char const *argv[]) {
  for (int32_t i = -INT32_MIN; i < INT32_MAX; ++i) {
    float f = i;
    unsigned fu = reinterpret_cast<unsigned &>(f);
    auto a = i2f(i);
    assert(fu == a);
  }
}
```