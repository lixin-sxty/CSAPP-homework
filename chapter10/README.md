* 10.6<br>
  fd2 = 4
* 10.7<br>
  ```c
  #include "csapp.h"
  int main(int argc, char **argv) {
    int n;
    char buf[MAXBUF];
    while ((n = Rio_readn(STDIN_FILENO, buf, MAXBUF)) != 0)
      Rio_writen(STDOUT_FILENO, buf, n);
  }
  ```
* 10.8<br>
  ```c
  #include "csapp.h"
  int main(int argc, char **argv) {
    struct stat stat;
    char *type, *readok;

    fstat(atoi(argv[1]), &stat);
    if (S_ISREG(stat.st_mode)) /* Determine file type */
      type = "regular";
    else if (S_ISDIR(stat.st_mode))
      type = "directory";
    else
      type = "other";
    if ((stat.st_mode & S_IRUSR)) /* Check read access */
      readok = "yes";
    else
      readok = "no";

    printf("type: %s, read: %s\n", type, readok);
    exit(0);
  }
  ```
* 10.9<br>
  ```c
  if (Fork() == 0) { /* child */
    dup2(0, 3); // 把3重定向到0会关闭描述符3，导致报错
    Execve("fstatcheck", argv, envp);
  }
  ```
* 10.10<br>
  ```c
  #include "csapp.h"
  int main(int argc, char **argv) {
    int n;
    rio_t rio;
    char buf[MAXLINE];
    int fd = -1;
    if (argc == 2) {
      fd = Open(argv[1], O_RDONLY, 0);
      dup2(fd, STDIN_FILENO);  // 把输入重定向到给定文件
    }
    Rio_readinitb(&rio, STDIN_FILENO);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
      Rio_writen(STDOUT_FILENO, buf, n);
  }
  ```