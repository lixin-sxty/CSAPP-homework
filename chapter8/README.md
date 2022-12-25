* 8.9<br>
  AB:否<br>
  AC:是<br>
  AD:是<br>
  BC:是<br>
  BD:是<br>
  CD:是<br>
* 8.10<br>
  调用一次，返回两次：`fork`<br>
  调用一次，从不返回：`execve, longjmp`<br>
  调用一次，返回一次或多次：`setjmp`<br>
* 8.11<br>
  ```
                  +-------+
                  |    "hello"
          +-------+-------+
          |     fork   "hello"
          |       +-------+
          |       |    "hello"
    +-----+-------+-------+
  main  fork    fork   "hello"
  ```
  4行<br>
* 8.12<br>
  ```
                  +-------+---------+
                  |    "hello"   "hello"
          +-------+-------+---------+
          |     fork   "hello"   "hello"
          |       +-------+---------+
          |       |    "hello"   "hello"
    +-----+-------+-------+---------+
  main  fork    fork   "hello"   "hello"
  ```
  8行<br>
* 8.13<br>
  fork()!=0即父进程打印x=++x(4), 子进程x仍为3, 之后两个进程分别打印x=--x(3和2), 顺序不确定
  ```
              x=3
           +-------------------+
           |                 "x=2"
    +------+----------+--------+
  main   fork x=3   'x=4'    "x=3"

  x=4
  x=2
  x=3
  ```
* 8.14<br>
  ```
                 +-----+----+
                 |  "hello" |
           +-----+-----+----+
           |   fork "hello" |
    +------+----------------+----+
  main   fork                 "hello"
  ```
  3个<br>
* 8.15<br>
  ```
                 +-----+-------+
                 |  "hello" "hello"
           +-----+-----+-------+
           |   fork "hello" "hello"
    +------+-------------------+
  main   fork               "hello"
  ```
  5个<br>
* 8.16<br>
  ```
              c=1 c--
           +-------+--+
           |          |
    +------+----------+----+
  main   fork c=1       ++c,c=2

  counter = 2
  ```
* 8.17<br>
  ```
                    "1" "Bye"
                +----+----+--+
                |            |
    +-----+-----+----+-------+--+----+
  main "hello" fork "0"        "2" "Bye"

  hello 0 1 Bye 2 Bye
  hello 1 0 Bye 2 Bye
  hello 1 Bye 0 2 Bye
  ```
* 8.18<br>
  ```
                 "0" "2"
             +----+---+
     atexit  |   "1" "2"
         +---+----+---+
         |  fork
         |   +----+---+
         |   |   "0"
    +----+---+----+---+
  main fork fork "1"
  ```
  根据进程图，2只可能出现在0或者1之后，因此A、C、E是可能的。注意`atexit`所添加的函数列表也是进程所有的
* 8.19<br>
  每次循环会根据已有的进程数再创建相同数量的子进程，每个进程会打印一行，因此一共打印 $2^n$ 行
* 8.20<br>
  ```c++
  #include <unistd.h>
  int main(int argc, char *const argv[], char *const envp[]) {
    execve("/bin/ls", argv, envp);
    return 0;
  }
  ```
* 8.21<br>
  ```
         +----+---+
         |   "a"  |
    +----+----+---+---+
  main fork  "b"     "c"

  abc
  bac
  ```
* 8.22<br>
  ```
  // mysystem.c
  #include <stdio.h>
  #include <stdlib.h>
  #include <sys/wait.h>
  #include <unistd.h>

  int mysystem(char *command) {
    pid_t pid = fork();
    if (pid == 0) {
      char *const argv[4] = {"/bin/sh", "-c", command, NULL};
      if (execve(argv[0], argv, NULL) < 0) {
        printf("%s : Command not found . \n", argv[0]);
        exit(0);
      }
    } else {
      printf("Pid of sub-process: %d\n", pid);
      int status;
      // exit normal
      if (wait(&status) > 0) {
        if (WIFEXITED(status))
          return WEXITSTATUS(status);
      }
      // abort by a signal
      if (WIFSIGNALED(status))
        return WTERMSIG(status);
    }
  }

  int main(int argc, char *const argv[], char *const envp[]) {
    int ret = mysystem("./exit");
    printf("Mysystem exit with %d\n", ret);

    ret = mysystem("./abort");
    printf("Mysystem abort with %d\n", ret);
    return 0;
  }

  // exit.c
  #include <stdlib.h>

  int main(int argc, char *argv[]) { exit(10); }

  // abort.c
  #include <stdio.h>

  int main(int argc, char *argv[]) {
    printf("abort is running...\n");
    while (1);
  }
  ```
  分别编译为3个执行程序mysystem, exit, abort.<br>
  执行`./mysystem`后终端输出
  ```
  Pid of sub-process: 13969
  Mysystem exit with 10
  Pid of sub-process: 13971
  abort is running...
  ```
  表明exit已经正常退出，exit code为10。abort正在循环中。<br>
  使用kill命令向子进程发送终止信号
  ```
  kill -15 13971
  ```
  终端输出
  ```
  Mysystem abort with 15
  ```
  表明abort被信号15终止

  Note：在win10 WSL2虚拟环境下执行以上程序，子进程被kill之后会马上启动一个相同命令的进程，进程号+1，尚不清楚原因
* 8.23<br>
  信号不会排队，处理第1个信号时，第2个信号会被加入pedding序列，之后收到的其他信号会被直接丢弃。与8.5.5中正确的信号处理一节示例类似
* 8.24<br>
  ```c++
  #include "csapp.h"

  int main() {
    int status, i;
    pid_t pid;
    /* Parent creates N children */
    for (i = 0; i < N; i++)
      if ((pid = fork()) == 0) /* Child */ {
        char* s = 0; /* Address 0 is protected */
        *s = 's';    /* Segmentation fault */
      }

    /* Parent reaps N children in no particular order */
    while ((pid = waitpid(-1, &status, 0)) > 0) {
      if (WIFEXITED(status))
        printf("child %d terminated normally with exit status=%d\n", pid,
              WEXITSTATUS(status));
      else {
        char* info;
        int sig = WTERMSIG(status);
        sprintf(info, "child %d terminated by signal %d", pid, sig);
        psignal(sig, info);
      }
    }

    /* The only normal termination is if there are no more children */
    if (errno != ECHILD) unix_error("waitpid error");

    exit(0);
  }
  ```
* 8.25<br>
  ```c++
  #include "csapp.h"
  jmp_buf env;

  void handle(int sig) { longjmp(env, 1); }

  char *tfgets(char *s, int size, FILE *stream) {
    signal(SIGALRM, handle);
    alarm(5);
    return setjmp(env) == 0 ? fgets(s, size, stream) : NULL;
  }

  int main() {
    char *s;
    char *input = tfgets(s, 10, stdin);
    if (input == NULL) {
      printf("Time out.\n");
    } else {
      printf("Input: %s", input);
    }
    return 0;
  }
  ```
