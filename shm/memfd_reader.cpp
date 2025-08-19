#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

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