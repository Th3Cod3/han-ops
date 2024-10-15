/******************************************************************************
 * File:         displayFunctions.c
 * Version:      1.4
 * Date:         2018-02-20
 * Author:       M. van der Sluys, J.H.L. Onokiewicz, R.B.A. Elsinghorst, J.G. Rouland
 * Description:  OPS exercise 2:  function definitions for display.c
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "displayFunctions.h"
#include "syntaxCheck.h"

// Check the command-line parameters:
ErrCode SyntaxCheck(int argc, char **argv) {
  ErrCode errCode = NO_ERR;
  if(argc < MIN_NUM_OF_PARS) {
    errCode = ERR_PARS;
  } else {
    errCode = TestType(argv[1]);                        // Test whether argument 1 has the correct value (print type)
    if(errCode == NO_ERR) errCode = TestNr(argv[2]);    // Test whether argument 2 contains a positive integer (number of times)
    if(errCode == NO_ERR) errCode = TestNr(argv[3]);    // Test whether argument 3 contains a positive integer (nice increment)
    if(errCode == NO_ERR) errCode = TestChar(argv[4]);  // Test whether argument 4 contains only one character (print character)

    if (argc > 4) {
      for (int i = 5; i < argc; i++) {
        errCode = TestChar(argv[i]);

        if (errCode != NO_ERR) {
          break;
        }
      }
    }
  }
  return errCode;
}

// Print an error message:
void DisplayError(ErrCode errCode) {
  switch(errCode) {
  case ERR_PARS:
    printf("\nNumber of command-line parameters is less than five.\n");
    break;
  case ERR_TYPE:
    printf("\nUnknown print type.\n");
    break;
  case ERR_NR:
    printf("\nNumber of times or nice is not a positive integer.\n");
    break;
  case ERR_CHAR:
    printf("\nPrint character contains more than one character.\n");
    break;
  default:
    printf("\nUnknown error code!\n");
  }
  
  printf("\nCorrect syntax:\n");
  printf("  ./display <print type> <nice increment> <number of times> <char 1> [<char 2> ... [<char N>]]\n\n");
  printf("  first parameter: <print type>: e, p or w\n");
  printf("  second parameter: <number of times>: positive integer\n");
  printf("  third parameter: <nice increment>: positive integer\n");
  printf("  fourth parameter: <char 1>: a single character\n");
  printf("  optional N parameter: <char (2|N)>: a single character\n");
  printf("\n");  // Newline at end
}


// Print chacacter printChar numberOfTimes times using method printMethod:
void PrintCharacters(char printMethod, unsigned long numberOfTimes, char printChar) {
  unsigned long index = 0;
  char echoCommand[] = "/bin/echo -n  ";
  
  switch(printMethod) {
  case 'e':
    echoCommand[13] = printChar;  // Put character to be printed in string
    for(index = 0; index < numberOfTimes; index++) {
      system(echoCommand);
    }
    break;
  case 'p':
    for(index = 0; index < numberOfTimes; index++) {
      printf("%c", printChar);
    }
    break;
  case 'w':
    for(index = 0; index < numberOfTimes; index++) {
      write(1, &printChar, 1);
    }
    break;
  default:
    printf("INTERNAL ERROR: Unknown print type:  %s.  This should not happen!\n", &printMethod);
  }
}
