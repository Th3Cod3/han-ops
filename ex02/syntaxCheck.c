/******************************************************************************
 * File:         syntaxCheck.c
 * Version:      1.4
 * Datum:        2018-02-20
 * Author:       M. van der Sluys, J.H.L. Onokiewicz, R.B.A. Elsinghorst, J.G. Rouland
 * Description:  OPS exercise 2:  definitions of test functions for display.c
 ******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "syntaxCheck.h"


// Test whether an argument is one character long and has the correct value (e,p,w):
ErrCode TestType(char *printMethod) {
  ErrCode fout = NO_ERR;
  
  // Add the missing code
  
  return fout;
}


// Test whether an argument contains a non-negative number:
ErrCode TestNr(char *numberOfTimes)
{
  if (atoi(numberOfTimes) < 0) {
    return ERR_NR;
  }
  
  return NO_ERR;
}

// Test whether an argument contains only one character:
ErrCode TestChar(char *printChar)
{
  if (strlen(printChar) > 1) {
    return ERR_CHAR;
  }
  
  return NO_ERR;
}
