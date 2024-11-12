/******************************************************************************
 * File:         display.c
 * Version:      1.4
 * Date:         2018-02-20
 * Author:       M. van der Sluys, J.H.L. Onokiewicz, R.B.A. Elsinghorst, J.G. Rouland
 * Description:  OPS exercise 2: syntax check
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include "displayFunctions.h"

int main(int argc, char *argv[]) {
  nice(0);

  unsigned long int numOfTimes;
  int8_t niceIncr;
  int8_t totalChars;
  pid_t pid;
  int currentPriority;
  char printMethod, printChar;
  ErrCode err;

  err = SyntaxCheck(argc, argv);  // Check the command-line parameters

  if(err != NO_ERR) {
    DisplayError(err);        // Print an error message
  } else {
    printMethod = argv[1][0];;
    numOfTimes = strtoul(argv[2], NULL, 10);  // String to unsigned long
    niceIncr = atoi(argv[3]);
    totalChars = argc - 4;


    for (uint8_t iChild = 0; iChild < totalChars; iChild++) {
      printChar = argv[iChild + 4][0];
      pid = fork();

      if (pid > 0) {
        continue;
      }

      nice(iChild * niceIncr);

      currentPriority = getpriority(PRIO_PROCESS, 0);
      printf("\n%d %d %c\n", iChild, currentPriority, printChar);

      PrintCharacters(printMethod, numOfTimes, printChar);  // Print character printChar numOfTimes times using method printMethod
      break;
    }

    if (pid > 0) {
      int wstatus;
      int w;

      do {
        w = wait(&wstatus);
        if (w == -1) {
          perror("waitpid");
          exit(EXIT_FAILURE);
        }

        if (WIFEXITED(wstatus)) {
          if (WEXITSTATUS(wstatus) != 0) {
            printf("exited with error, status=%d\n", WEXITSTATUS(wstatus));
          }
        } else if (WIFSIGNALED(wstatus)) {
          printf("killed by signal %d\n", WTERMSIG(wstatus));
        } else if (WIFSTOPPED(wstatus)) {
          printf("stopped by signal %d\n", WSTOPSIG(wstatus));
        } else if (WIFCONTINUED(wstatus)) {
          printf("continued\n");
        }
      } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

      sleep(1); // give the scheduler some time to print pending messages
    }
  }

  printf("\n");  // Newline at end
  return 0;
}
