#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

char number = '0';

static void handler(int signum) {
  if (signum == SIGXFSZ) {
    number++;
  }

  if (number > '9') {
    number = '0';
  }
}

static inline void sendPID() {
  printf("Opening PIDpipe\n");
  int fd = open("PIDpipe", O_WRONLY);
  if (fd == -1) {
    perror("Error opening PIDpipe");

    exit(1);
  }

  pid_t pid = getpid();
  printf("Sending PID to PIDpipe\n");
  if (write(fd, &pid, sizeof(pid_t)) == -1) {
    perror("Error writing to PIDpipe");

    exit(1);
  }

  printf("PID sent: %d\n", pid);

  close(fd);
}

int main(void) {
  struct sigaction sa;

  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGXFSZ, &sa, NULL) == -1) {
    printf("Error setting signal handler\n");

    return 1;
  }

  sendPID();

  while (1) {
    write(1, &number, 1);

    sleep(1);
  }

  return 0;
}
