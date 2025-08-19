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

  msg.msg_name = NULL;
  msg.msg_namelen = 0;

  // must send at least 1Byte data
  struct iovec iov;
  char sendchar = 0;
  iov.iov_base = &sendchar;
  iov.iov_len = sizeof(sendchar);
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;

  char cmsg_buf[CMSG_SPACE(sizeof(fd))];
  msg.msg_control = cmsg_buf;
  msg.msg_controllen = sizeof(cmsg_buf);
  msg.msg_flags = 0;
  printf("msg.msg_controllen: %zu\n", msg.msg_controllen);

  struct cmsghdr *p_cmsg = CMSG_FIRSTHDR(&msg);
  p_cmsg->cmsg_level = SOL_SOCKET;
  p_cmsg->cmsg_type = SCM_RIGHTS;
  p_cmsg->cmsg_len = CMSG_LEN(sizeof(fd));
  printf("p_cmsg->cmsg_len: %zu\n", p_cmsg->cmsg_len);

  int *p_fd = (int *)CMSG_DATA(p_cmsg);
  *p_fd = fd;

  if (sendmsg(sock_fd, &msg, 0) == -1) {
    perror("sendmsg");
    return -1;
  }

  return 0;
}

int main() {
  // create memfd
  int fd = memfd_create("test_memfd", 0);
  if (fd == -1) {
    perror("memfd_create");
    return 1;
  }
  ftruncate(fd, 1024);

  // mmap memfd
  char *p_mmap = (char *)mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (p_mmap == MAP_FAILED) {
    perror("mmap");
    return 1;
  }
  // write to memfd
  strcpy(p_mmap, "Hello, World!");
  printf("p_mmap: %s\n", p_mmap);

  // create unix socket
  int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    perror("socket");
    return 1;
  }

  // bind unix socket
  sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, "/tmp/test_memfd");
  unlink(addr.sun_path);
  printf("bind to %s\n", addr.sun_path);
  if (bind(sock_fd, (sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("bind");
    return 1;
  }

  // listen
  printf("listen\n");
  if (listen(sock_fd, 1) == -1) {
    perror("listen");
    return 1;
  }

  // accept client and send fd
  int user_id = 0;
  while (true) {
    int client_fd = accept(sock_fd, NULL, NULL);
    printf("accept client fd: %d\n", client_fd);
    if (client_fd == -1) {
      perror("accept");
      continue;
    }
    sprintf(p_mmap, "hello, you are the %dth user", user_id);
    user_id++;
    send_fd(client_fd, fd);
    printf("send fd: %d\n", fd);
    close(client_fd);
  }

  // unmap memfd and close memfd
  munmap(p_mmap, 1024);
  close(fd);

  // close unix socket
  close(sock_fd);
  return 0;
}