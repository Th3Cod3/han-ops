/******************************************************************************
 * File:         syntaxCheck.h
 * Version:      1.4
 * Datum:        2018-02-20
 * Auhtor:       M. van der Sluys, J.H.L. Onokiewicz, R.B.A. Elsinghorst, J.G. Rouland
 * Description:  Declaration of the test functies defined in testfuncties.c
 *               for display.c
 ******************************************************************************/

#ifndef syntaxCheckH
#define syntaxCheckH

#include "errorCodes.h"

ErrCode TestNr(char *numberOfTimes);
/*
  Input:          numberOfTimes = starting address of number-of-times parameter
  Precondition:	  At least one character is present from address numberOfTimes
  Function:       Test whether:
                  - string contains a number without + or - sign
                  - string does not contain additional characters
  Postcondition:  None
  Output:	  Error code:
            0 = NO_ERR:	 no error
            3 = ERR_NR:	 incorrect number
*/
#endif
