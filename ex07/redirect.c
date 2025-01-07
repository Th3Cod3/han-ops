#include <fcntl.h>  /* Obtain O_* constant definitions */
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  // parent reads
  int pipePR[2];
  // parent writes
  int pipePW[2];

  if(pipe(pipePR) == -1 || pipe(pipePW) == -1) {
    perror("pipe");

    return 1;
  }

  switch (fork()) {
    case -1:
      perror("fork");

      return 1;
    case 0:
      close(pipePR[0]);
      close(pipePW[1]);
      dup2(pipePR[1], 1);
      dup2(pipePW[0], 0);
      close(pipePR[1]);
      close(pipePW[0]);
      execlp("./filter", "filter", NULL);
      perror("execlp");

      return 1;
    default:
      close(pipePR[1]);
      close(pipePW[0]);

      while(1) {
        char c;
        if(read(0, &c, 1) == 0) {
          // normally, this process is killed by the child 
          // with a signal SIGPIPE
          printf("child has closed the pipe\n");

          break;
        }

        write(pipePW[1], &c, 1);
        read(pipePR[0], &c, 1);
        write(1, &c, 1);
      }
  }

  return 0;
}
