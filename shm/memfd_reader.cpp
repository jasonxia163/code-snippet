#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <sys/mman.h>

int recv_fd(int sock_fd) {
  struct msghdr msg;
  msg.msg_name = NULL;
  msg.msg_namelen = 0;

  // must recv at least 1Byte data
  char recvchar;
  struct iovec iov;
  iov.iov_base = &recvchar;
  iov.iov_len = sizeof(recvchar);
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;

  char cmsg_buf[CMSG_SPACE(sizeof(int))];
  msg.msg_control = cmsg_buf;
  msg.msg_controllen = sizeof(cmsg_buf);
  msg.msg_flags = 0;
  printf("msg.msg_controllen: %zu\n", msg.msg_controllen);

  if (recvmsg(sock_fd, &msg, 0) == -1) {
    perror("recvmsg");
    return -1;
  }

  struct cmsghdr *p_cmsg = CMSG_FIRSTHDR(&msg);
  if (p_cmsg == NULL) {
    printf("No control message received\n");
    return -1;
  }

  return *(int *)CMSG_DATA(p_cmsg);
}

int main() {
  // create unix socket
  int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    perror("socket");
    return 1;
  }

  // connect to unix socket
  sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, "/tmp/test_memfd");
  printf("connect to %s\n", addr.sun_path);
  if (connect(sock_fd, (sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("connect");
    return 1;
  }

  // recv fd from unix socket
  int fd = recv_fd(sock_fd);
  if (fd == -1) {
    perror("recv_fd");
    return 1;
  }

  // mmap memfd
  char *p_mmap = (char *)mmap(NULL, 1024, PROT_READ, MAP_SHARED, fd, 0);
  if (p_mmap == MAP_FAILED) {
    perror("mmap");
    return 1;
  }
  printf("p_mmap: %s\n", p_mmap);

  // unmap memfd and close memfd
  munmap(p_mmap, 1024);
  close(fd);

  // close unix socket
  close(sock_fd);
  return 0;
}