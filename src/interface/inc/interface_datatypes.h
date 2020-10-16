#ifndef INTERFACE_DATATYPES_H
#define INTERFACE_DATATYPES_H

#include "config.h"

/* Indicates whether input data are valid or not. */
typedef enum {
    Input_Invalid,
    Input_Valid
} InputValidation_t;

/* Masks which indicates unique input option. */
typedef enum {
    Mask_HelpOption         = 0x01,
    Mask_SourceOption       = 0x02,
    Mask_DestinationOption  = 0x04,
    Mask_EncryptionOption   = 0x08,
    Mask_DecryptionOption   = 0x10
} MaskOption_t;

/* Data retrieved from command line as arguments to options*/
typedef struct {
    char *pcKey;
    char *pcDestination;
    char *pcSource;
} InputData_t;

/* Type of routine to be performed by program. */
typedef enum {
    Routine_Help = 0,
    Routine_Encrypt,
    Routine_Decrypt,
    Routine_Count
} Routine_t;

/* Evaluated data from inputed one. It is transfered to external */
/* modules by function "Interface_GetValidInput".                */
typedef struct {
    char acKey[KEY_SIZE];
    char acDestination[MAX_PATH_LENGTH];
    char acSource[MAX_PATH_LENGTH];
    Routine_t tRoutine;
} InputEvaluatedData_t;

/* Beware this enumerators used as array indexes. Also they     */
/* indicate current error which happpend.                       */
typedef enum {
    Error_NoArguments = 0,
    Error_NotUniqueOption,
    Error_NotMatchingOptions,
    Error_NotEnoughOptions,
    Error_OpenFile,
    Error_Count,                /* Indicates that no error was found. */
    Error_GetOpt                /* Since this error handled by GetOpt it is not any array index. */
} ErrorType_t;


#endif /* INTERFACE_DATATYPES_H */