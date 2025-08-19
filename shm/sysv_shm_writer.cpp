#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdio>
#include <cstring>

int main() {
  key_t key = ftok(".", 'a');
  if (key == -1) {
    perror("ftok");
    return 1;
  }

  int shm_id = shmget(key, 1024, 0666 | IPC_CREAT);
  if (shm_id == -1) {
    perror("shmget");
    return 1;
  }

  char *shm_addr = (char *)shmat(shm_id, NULL, 0);
  if (shm_addr == (char *)-1) {
    perror("shmat");
    return 1;
  }

  strcpy(shm_addr, "Hello, World!");
  printf("shm_id: %d, shm_addr: %p, %s\n", shm_id, shm_addr, shm_addr);
  shmdt(shm_addr);
  
  return 0;
}