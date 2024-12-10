#include <signal.h>
#include <stdio.h>
#include <unistd.h>

char number = '0';

static void handler(int signum) {
  if (signum == SIGXFSZ) {
    number++;
  }

  if (number > '9') {
    number = '0';
  }
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

  printf("My PID is %d\n", getpid());

  while (1) {
    write(1, &number, 1);

    sleep(1);
  }

  return 0;
}
