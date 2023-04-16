* 11.6<br>
  A:
  ```shell
  diff --git a/code/netp/tiny/tiny.c b/code/netp/tiny/tiny.c
  index d1a7302..741be33 100644
  --- a/code/netp/tiny/tiny.c
  +++ b/code/netp/tiny/tiny.c
  @@ -9,6 +9,7 @@
  #include "csapp.h"

  void doit(int fd);
  +void echo(int fd);
  void read_requesthdrs(rio_t *rp);
  int parse_uri(char *uri, char *filename, char *cgiargs);
  void serve_static(int fd, char *filename, int filesize);
  @@ -37,7 +38,9 @@ int main(int argc, char **argv) {
      Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE,
                  0);
      printf("Accepted connection from (%s, %s)\n", hostname, port);
  -    doit(connfd);  // line:netp:tiny:doit
  +    // doit(connfd);  // line:netp:tiny:doit
  +    // Homework 11.6 A
  +    echo(connfd);
      Close(connfd); // line:netp:tiny:close
    }
  }
  @@ -95,6 +98,17 @@ void doit(int fd) {
  }
  /* $end doit */

  +void echo(int fd) {
  +  size_t n;
  +  char buf[MAXLINE];
  +  rio_t rio;
  +
  +  Rio_readinitb(&rio, fd);
  +  while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0 && strcmp(buf, "\r\n")) {
  +    Rio_writen(fd, buf, n);
  +  }
  +}
  +
  /*
    * read_requesthdrs - read HTTP request headers
    */
  ```
  B:
  ```
  GET / HTTP/1.1
  Host: 192.168.94.214:1234
  Connection: keep-alive
  Upgrade-Insecure-Requests: 1
  User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36
  Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
  Accept-Encoding: gzip, deflate
  Accept-Language: zh-CN,zh;q=0.9

  GET /godzilla.gif HTTP/1.1
  Host: 192.168.94.214:1234
  Connection: keep-alive
  User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36
  Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8
  Referer: http://192.168.94.214:1234/
  Accept-Encoding: gzip, deflate
  Accept-Language: zh-CN,zh;q=0.9
  ```
  C:HTTP 1.1<br>
  D:<br>
  HTTP1.1 rfc文件网址：https://www.rfc-editor.org/rfc/rfc2616.pdf<br>
  | 请求字段                  | rfc中含义                                                                        |
  | ------------------------- | -------------------------------------------------------------------------------- |
  | Host                      | 指定了被请求资源的互联网主机和端口号，从用户或参考资源提供的原始URI中获得的。    |
  | Connection                | 允许发送方指定该特定连接所需的选项，并且不得由代理机构通过进一步的连接进行通信。 |
  | Upgrade-Insecure-Requests | rfc中未找到                                                                      |
  | User-Agent                | 包含关于发起请求的用户代理的信息                                                 |
  | Accept                    | 指定响应中可接受的某些媒体类型                                                   |
  | Referer                   | 允许客户端指定获取Request-URI的资源的地址(URI)                                   |
  | Accept-Encoding           | 与Accept类似，但限制了响应中可接受的内容编码                                     |
  | Accept-Language           | 与Accept类似，但限制了作为请求响应的首选自然语言的集合                           |