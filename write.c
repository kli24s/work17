#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>

#include <sys/shm.h>

#include <unistd.h>
#include <fcntl.h>

// ============================================================================

#define SEMA_KEY 24601
#define SHARED_MEM_KEY 24602

// ============================================================================

int main() {

  int semd;
  // int r;
  // int v;

  semd = semget(SEMA_KEY, 1, 0);
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;

  semop(semd, &sb, 1);

  int create_call = open("telephone", O_RDWR | O_APPEND);

  int shmd = shmget(SHARED_MEM_KEY, 1, 0);
  int *size = shmat(shmd, 0, 0);
  lseek(create_call, (-1) * (*size), SEEK_END);

  // printf("got the semaphore!\n");

  char line[*size];
  read(create_call, line, (-1) * (*size));
  printf("Last line: %s\n", line);

  printf("Enter new line: ");
  char newline[1000];
  fgets(newline, 1000, stdin);
  *size = strlen(newline);
  write(create_call, newline, *size);
  close(create_call);

  shmdt(size);

  sb.sem_op = 1;
  semop(semd, &sb, 1);

  return 0;

}
