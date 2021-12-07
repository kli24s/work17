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

// union semun {
//   int              val;
//   struct semid_ds *buf;
//   unsigned short  *array;
//   struct seminfo  *__buf;
// };

// ============================================================================

int main(int argc, char ** argv) {

  if (argc >= 2) {

    int semd;
    int shmd;

    // ================================================================= CREATE
    if (strcasecmp(argv[1], "-create") == 0) {

      int r;
      int semd;
      int shmd;

      semd = semget(SEMA_KEY, 1, IPC_CREAT | 0640);
      shmd = shmget(SHARED_MEM_KEY, 4, IPC_CREAT | 0777);

      union semun us;
      us.val = 1;

      r = semctl(semd, 0, SETVAL, us);

      char * telephone = argv[2];
      int create_call = open("telephone", O_CREAT, 0644);
      close(create_call);
    }

    // ================================================================= REMOVE
    else if (strcasecmp(argv[1], "-remove") == 0) {

      semd = shmget(SEMA_KEY, 1, 0);
      shmd = shmget(SHARED_MEM_KEY, 0, 0);

      semctl(semd, IPC_RMID, 0);
      shmctl(shmd, IPC_RMID, 0);

      char telephone[1000];
      int open_call = open("telephone", O_RDONLY);
      read(open_call, telephone, 1000);
      printf("Message Received: %s\n", telephone);

      close(open_call);

    }

    // ================================================================== ERROR
    else {
      printf("error: incorrect command\n");
    }
  }

  // ==================================================================== ERROR
  else {
    printf("error: no command found\n");
  }

  return 0;

}
