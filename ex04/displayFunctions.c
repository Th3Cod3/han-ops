/******************************************************************************
 * File:         displayFunctions.c
 * Version:      1.4
 * Date:         2018-02-20
 * Author:       M. van der Sluys, J.H.L. Onokiewicz, R.B.A. Elsinghorst, J.G. Rouland
 * Description:  OPS exercise 2:  function definitions for display.c
 ******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include "displayFunctions.h"
#include "syntaxCheck.h"

// Check the command-line parameters:
ErrCode SyntaxCheck(int argc, char **argv) {
  ErrCode errCode = NO_ERR;
  if(argc < MIN_NUM_OF_PARS) {
    errCode = ERR_PARS;
  } else {
    if(errCode == NO_ERR) errCode = TestNr(argv[3]);    // Test whether argument 3 contains a positive integer (nice increment)
  }
  return errCode;
}

// Print an error message:
void DisplayError(ErrCode errCode) {
  switch(errCode) {
  case ERR_PARS:
    printf("\nNumber of command-line parameters is less than five.\n");
    break;
  case ERR_NR:
    printf("\nNice is not a positive integer.\n");
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
