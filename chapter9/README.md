* 9.11<br>
  A:00 0010 0111 1100<br>
  B:
  | 参数    | 值   |
  | ------- | ---- |
  | VPN     | 0x09 |
  | TLB索引 | 0x01 |
  | TLB标记 | 0x02 |
  | TLB命中 | 否   |
  | 缺页    | 否   |
  | PPN     | 17   |

  C:0101 1111 1100<br>
  D:
  | 参数           | 值   |
  | -------------- | ---- |
  | 字节偏移       | 0x0  |
  | 缓存索引       | 0x0f |
  | 缓存标记       | 0x17 |
  | 缓存命中       | 否   |
  | 返回的缓存字节 | -    |
* 9.12<br>
  A:00 0011 1010 1001<br>
  B:
  | 参数    | 值   |
  | ------- | ---- |
  | VPN     | 0x0e |
  | TLB索引 | 0x02 |
  | TLB标记 | 0x03 |
  | TLB命中 | 否   |
  | 缺页    | 否   |
  | PPN     | 0x11 |

  C: 0100 0110 1001<br>
  D:
  | 参数           | 值   |
  | -------------- | ---- |
  | 字节偏移       | 0x1  |
  | 缓存索引       | 0x0a |
  | 缓存标记       | 0x11 |
  | 缓存命中       | 否   |
  | 返回的缓存字节 | -    |
* 9.13<br>
  A:00 0000 0100 0000<br>
  B:
  | 参数    | 值   |
  | ------- | ---- |
  | VPN     | 0x01 |
  | TLB索引 | 0x01 |
  | TLB标记 | 0x00 |
  | TLB命中 | 否   |
  | 缺页    | 是   |
  | PPN     | -    |
* 9.14<br>
  ```c
    #include <unistd.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <fcntl.h>

    int main(int argc, char* const argv[]) {
      // Open file
      int fd = open("hello.txt", O_RDWR, 0);
      // Get file size
      struct stat st;
      fstat(fd, &st);
      size_t size = st.st_size;
      // Map fd to new v-memory
      void* bufp = mmap(NULL, size, PROT_WRITE, MAP_SHARED, fd, 0);
      // Change first char to J
      *(char*)bufp = 'J';
      // Unmap fd
      munmap(bufp, size);
      // Close file
      close(fd);

      return 0;
    }
  ```
* 9.15<br>
  | 请求       | 块大小(十进制) | 块头部(十六进制) |
  | ---------- | -------------- | ---------------- |
  | malloc(3)  | 8              | 0x9              |
  | malloc(11) | 16             | 0x11             |
  | malloc(20) | 24             | 0x19             |
  | malloc(21) | 32             | 0x33             |
* 9.16<br>
  需要考虑pred和succ指针占用的空间，最小块大小=头部4+(脚部4)+pred4+succ4+1按要求取整
  | 对齐要求 | 已分配块 | 空闲块   | 最小块大小 |
  | -------- | -------- | -------- | ---------- |
  | 单字     | 头、脚部 | 头、脚部 | 20         |
  | 单字     | 头部     | 头、脚部 | 16         |
  | 双字     | 头、脚部 | 头、脚部 | 24         |
  | 双字     | 头部     | 头、脚部 | 16         |