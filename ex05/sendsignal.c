#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <pid>\n", argv[0]);

    return 1;
  }

  int pid = atoi(argv[1]);

  while (1) {
    if (kill(pid, SIGXFSZ) == -1) {
      perror("kill");

      return 1;
    }

    write(STDOUT_FILENO, ".", 1);
    sleep(3);
  }

  return 0;

}
