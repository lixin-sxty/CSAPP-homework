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
* 9.17<br>
  ```
  // Global pointer record last end position
  static void *last_start = heap_listp;

  static void *coalesce(void *bp)
  {
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));
    if (prev_alloc && next_alloc) {
      return bp;
    }
    else if (prev_alloc && !next_alloc) {
      size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
      PUT(HDRP(bp), PACK(size, 0));
      PUT(FTRP(bp), PACK(size, 0));
    }
    else if (!prev_alloc && next_alloc) {
      size += GET_SIZE(HDRP(PREV_BLKP(bp)));
      PUT(FTRP(bp), PACK(size, 0));
      PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
      bp = PREV_BLKP(bp);
    }
    else {
      size += GET_SIZE(HDRP(PREV_BLKP(bp))) +
      GET_SIZE(FTRP(NEXT_BLKP(bp)));
      PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
      PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
      bp = PREV_BLKP(bp);
    }
    // If rover point to mid of a coalesced block
    // move it to block head
    if ((last_start > (char *)bp) && (last_start < NEXT_BLKP(bp)))
      last_start = bp;
    return bp;
  }

  void* find_fit(size_t asize) {
    // Record position before search
    void *current = last_start;
    // Search from last_start to the end
    for (; GET_SIZE(HDRP(last_start)) > 0; last_start = NEXT_BLKP(last_start)){
      if (!GET_ALLOC(HDRP(last_start)) && (asize <= GET_SIZE(HDRP(last_start))))
        return last_start;
    }
    // Search from start to last_start
    for (last_start = heap_listp; last_start < current; last_start = NEXT_BLKP(last_start)){
      if (!GET_ALLOC(HDRP(last_start)) && (asize <= GET_SIZE(HDRP(last_start))))
        return last_start;
    }
    // Not fit
    return NULL;
  }
  ```
* 9.18<br>
第一次思考比较简单，只是把已分配结点的footer移除，相应调整合并函数。这样导致无法在常数时间内完成合并。参考https://dreamanddead.github.io/CSAPP-3e-Solutions/chapter9/9.18/中思路，把header中的空余位利用起来，可以在不影响性能的基础上去掉footer。注意2点：1、由于已分配块的footer移除，已分配块的最小大小变为8 2、修改一个块的状态之后(alloc，free)需要相应改变下一个block中的PREV_ALLOC位
  ```
  // 利用header末尾3位中的第2位记录上一个块是否已经分配
  #define PACK(size, alloc, prev_alloc)  ((size) | (alloc) | (prev_alloc << 1))
  // 新增辅助宏获取上一个block是否已分配
  #define GET_PREV_ALLOC(p) ((GET(p) >> 1) & 0x1)

  int mm_init(void){
    /* 创建初始空堆 */
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1)
      return -1;
    // 创建一个空的空闲链表
    PUT(heap_listp, 0); /* Alignment padding */
    PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1, 1)); /* Prologue header */
    PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1, 1)); /* Prologue footer */
    PUT(heap_listp + (3*WSIZE), PACK(0, 1, 1)); /* Epilogue header */
    heap_listp += (2*WSIZE);
    /* 用CHUNKSIZE字节组成的空闲块扩展空堆 */
    if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
      return -1;
    return 0;
  }

  static void *extend_heap(size_t words){
    char *bp;
    size_t size;

    /* 分配偶数WORD数以保持对齐 */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
    if ((long)(bp = mem_sbrk(size)) == -1)
      return NULL;

    /* 初始化自由块头部/脚部和结尾块头部 */
    PUT(HDRP(bp), PACK(size, 0, GET_PREV_ALLOC(HDRP(bp)))); /* Free block header */
    PUT(FTRP(bp), PACK(size, 0, GET_PREV_ALLOC(HDRP(bp)))); /* Free block footer */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1, 0)); /* New epilogue header */

    /* 如果前一个块是空闲的则合并 */
    return coalesce(bp);
  }

  void *mm_malloc(size_t size){
    size_t asize; /* Adjusted block size */
    size_t extendsize; /* Amount to extend heap if no fit */
    char *bp;

    /* Ignore spurious requests */
    if (size == 0)
      return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    // 现在最小块大小为4，加上不为0的负载，只需要对齐到8即可
    if (size <= WSIZE)
      asize = DSIZE;
    else
      asize = DSIZE * ((size + (WSIZE) + (DSIZE-1)) / DSIZE); // 添加4字节并向上舍入到8的整数倍

    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL) {
      place(bp, asize);
      return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize,CHUNKSIZE);
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
      return NULL;
    place(bp, asize);
    return bp;
  }

  void mm_free(void *bp){
    size_t size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size, 0, GET_PREV_ALLOC(HDRP(bp))));
    PUT(FTRP(bp), PACK(size, 0, GET_PREV_ALLOC(HDRP(bp))));
    // free块后还需要把下一个块中的PREV_ALLOC位修改为0
    if (GET_ALLOC(HDRP(NEXT_BLKP(bp))))
      PUT(HDRP(NEXT_BLKP(bp)), PACK(GET_SIZE(HDRP(NEXT_BLKP(bp))), 1, 0));
    else {
      PUT(HDRP(NEXT_BLKP(bp)), PACK(GET_SIZE(HDRP(NEXT_BLKP(bp))), 0, 0));
      PUT(FTRP(NEXT_BLKP(bp)), PACK(GET_SIZE(HDRP(NEXT_BLKP(bp))), 0, 0));
    }
    coalesce(bp);
  }

  static void *coalesce(void *bp){
    size_t prev_alloc = GET_PREV_ALLOC(HDRP(bp));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));
    if (prev_alloc && next_alloc) { /* 前后都不为空 */
      return bp;
    }
    else if (prev_alloc && !next_alloc) { /* 与后一个合并 */
      size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
      PUT(HDRP(bp), PACK(size, 0, 1));
      PUT(FTRP(bp), PACK(size, 0, 1));
    }
    else if (!prev_alloc && next_alloc) { /* 与前一个合并 */
      size += GET_SIZE(HDRP(PREV_BLKP(bp)));
      // 空闲块之前一定是已分配块，这里PREV_ALLOC=1
      PUT(FTRP(bp), PACK(size, 0, 1));
      PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0, 1));
      bp = PREV_BLKP(bp);
    }
    else { /* 与前后合并 */
      size += GET_SIZE(HDRP(PREV_BLKP(bp))) +
      GET_SIZE(FTRP(NEXT_BLKP(bp)));
      // 合并两个空闲块不需要改变后续块的状态
      PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0, 1));
      PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0, 1));
      bp = PREV_BLKP(bp);
    }
    return bp;
  }

  void place(void* bp, size_t asize) {
    size_t block_size = GET_SIZE(HDRP(bp));
    size_t rest_size = block_size-asize;
    if(rest_size >= 2*DSIZE){
      PUT(HDRP(bp), PACK(asize, 1, 1));
      bp = NEXT_BLKP(bp);
      PUT(HDRP(bp), PACK(rest_size, 0, 1));
      PUT(FTRP(bp), PACK(rest_size, 0, 1));
    }
    else{
      PUT(HDRP(bp), PACK(block_size, 1, 1));
      PUT(HDRP(NEXT_BLKP(bp)), PACK(csize, 1, 1));
    }
  }
  ```
* 9.19<br>
  1<br>
    &ensp;&ensp;a：正确<br>
    &ensp;&ensp;b：首次适配内存分配算法通常比最佳适配算法快<br>
    &ensp;&ensp;c：空闲链表按照LIFO排序时，使用边界标记来回收也快速<br>
    &ensp;&ensp;d：伙伴系统也会有外部碎片<br>
  2<br>
    &ensp;&ensp;a：在这种情况下首次适配算法应该很快，但无法避免外部碎片<br>
    &ensp;&ensp;b：对于最佳适配方法，空闲块链表应该按照块大小排序。<br>
    &ensp;&ensp;c：最佳适配方法选择与请求段匹配的最小的空闲块<br>
    &ensp;&ensp;d：正确<br>
  3<br>
    &ensp;&ensp;b：正确，具体见9.10.3节内容
