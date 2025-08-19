#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int send_fd(int sock_fd, int fd) {
  struct msghdr msg;
  struct cmsghdr *p_cmsg;
  char cmsg_buf[CMSG_SPACE(sizeof(int))];

  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = NULL;
  msg.msg_iovlen = 0;
  msg.msg_control = cmsg_buf;
  msg.msg_controllen = sizeof(cmsg_buf);
  msg.msg_flags = 0;

  p_cmsg = CMSG_FIRSTHDR(&msg);
  p_cmsg->cmsg_len = CMSG_LEN(sizeof(int));
  p_cmsg->cmsg_level = SOL_SOCKET;
  p_cmsg->cmsg_type = SCM_RIGHTS;
  memcpy(CMSG_DATA(p_cmsg), &fd, sizeof(int));

  if (sendmsg(sock_fd, &msg, 0) == -1) {
    perror("sendmsg");
    return -1;
  }

  return 0;
}

int main() {
  int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    perror("socket");
    return 1;
  }

  #ifdef __linux__
  int fd = memfd_create("test_memfd", 0);
  if (fd == -1) {
    perror("memfd_create");
    return 1;
  }
  #else
  int fd = shm_open("/test_memfd", O_CREAT | O_RDWR, 0666);
  if (fd == -1) {
    perror("shm_open");
    return 1;
  }
  #endif

  send_fd(sock_fd, fd);
  close(sock_fd);
  return 0;
}