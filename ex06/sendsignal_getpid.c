#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

static inline pid_t readPID() {
  printf("Opening PIDpipe\n");
  int fd = open("PIDpipe", O_RDONLY);
  if (fd == -1) {
    perror("Error opening PIDpipe");

    exit(1);
  }

  pid_t pid;
  printf("Reading PID from PIDpipe\n");
  if (read(fd, &pid, sizeof(pid_t)) == -1) {
    perror("Error reading from PIDpipe");

    exit(1);
  }

  printf("PID read: %d\n", pid);

  close(fd);

  return pid;
}

int main(int argc, char *argv[]) {
  if (argc != 1) {
    printf("Usage: %s\n", argv[0]);

    return 1;
  }

  pid_t pid = readPID();

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
