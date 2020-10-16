#ifndef INTERFACE_H
#define INTERFACE_H

#include "config.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "interface_datatypes.h"

/*  --- Color Codes For Setting Text's Color In Consol --- */
#define RESET       "\033[0m"           /* RESET TO DEFAULT FONT */
#define BOLDRED     "\033[1m\033[31m"   /* BOLD RED              */
#define BOLDGREEN   "\033[1m\033[32m"   /* BOLD GREEN            */


#define MIN_ARGC_VALUE 2

/*              --- Value for dealing with "getopt" function ---            */
/* All input options combined for "getopt" argument "options".              */
#define OPTIONS_FOR_GETOPT "+hs:f:e:d:" 
/* Return value of "getopt" which indicates that no valid options reamin.   */
#define NO_OPTIONS_GETOPT -1

/*   --- All valid input options ---     */
#define OPT_HELP_IDENTIFIER         'h'
#define OPT_SOURCE_FILE_IDENTIFIER  's'
#define OPT_DESTINATION_IDENTIFIER  'f'
#define OPT_ENCRYPTION_IDENTIFIER   'e'
#define OPT_DECRYPTION_IDENTIFIER   'd'



/* It takes arguments of "main" function which are console input options.   */
/* It parses it and evaluates whether they are valid or incorrect.          */
/* In first case it also evaluates "Routine_" and application arguments.    */
/* In second case it evaluates type of error.                               */
InputValidation_t Interface_ParceInput(int argc, char *argv[]);

/* Stores evaluated by previous function "Routine_" and application         */
/* arguments to structure passed by pointer as argument to this function.   */
/* NOTE: It is, only, possible to call this function if                     */
/* "Interface_ParceInput" returned "Input_Valid".                           */
void Interface_GetValidInput(InputEvaluatedData_t *ptValidInput);

/* Used to register externally detected error. By externally means otside   */
/* this module. It accepts as argument value of enumeration type, which is  */
/* error index.                                                             */
void Interface_RegisterError(ErrorType_t tError2Register);

/* Prints to "stderr" previosly detected/registered error.                  */
void Interface_PrintErrorDescription(void);

/* Prints program description from file defined in <HELP_FILE> to "stdout"  */
void Interface_PrintHelp(void);

/* Calculates file size and subtracts "iOffset" from it.                    */
unsigned int Interface_FileSize(FILE *ptFile, int iOffset);


#endif /* INTERFACE_H */