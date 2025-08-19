#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

int main() {
  int fd = shm_open("/test_shm", O_RDONLY, 0666);
  if (fd == -1) {
    perror("shm_open");
    return 1;
  }

  char *shm_addr = (char *)mmap(NULL, 1024, PROT_READ, MAP_SHARED, fd, 0);
  if (shm_addr == (char *)-1) {
    perror("mmap");
    return 1;
  }

  printf("shm_addr: %p, %s\n", shm_addr, shm_addr);
  munmap(shm_addr, 1024);

  shm_unlink("/test_shm");
}