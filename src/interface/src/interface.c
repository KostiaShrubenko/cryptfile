
#include "interface.h"

char *apcErrorMessage[Error_Count] =
{
    "Type \"-h\" to see instruction\n",
    "Option \"-h\" should be called wtihout any other options\n",
    "Option \"-e\" and \"-d\" should not be called simultaneously\n",
    "You didn't specify enough options\n",
    "Cant open or create file\n"
};

static ErrorType_t tError = Error_Count;
static InputEvaluatedData_t tEvaluatedData;
static char acRelativePath[MAX_PATH_LENGTH];

static Routine_t FindRoutine(unsigned char ucValidInputOptions);
static ErrorType_t FindError(int argc, unsigned char ucValidInputOptions);
static void ProcessInputArguments(unsigned char ucValidInputOptions, InputData_t const *ptInputArguments);
static void FindRelativePath(char const* cpExecutablePath, char *cpRelativePath);
static void TakeInputFileName(char *pcSourceFile, char *pcFileName);

InputValidation_t Interface_ParceInput(int argc, char *argv[])
{
    InputData_t InputValidArguments;
    InputValidation_t tRetValue = Input_Invalid;
    char cCurrentOption = 0;
    unsigned char ucAllOptionsRecieved = 0;

    while ((cCurrentOption = getopt(argc, argv, OPTIONS_FOR_GETOPT)) != NO_OPTIONS_GETOPT)
    {
        switch(cCurrentOption)
        {
            case OPT_HELP_IDENTIFIER:
                ucAllOptionsRecieved |= Mask_HelpOption;
                break;
            case OPT_SOURCE_FILE_IDENTIFIER:
                InputValidArguments.pcSource = optarg;
                ucAllOptionsRecieved |= Mask_SourceOption;
                break;
            case OPT_DESTINATION_IDENTIFIER:
                InputValidArguments.pcDestination = optarg;
                ucAllOptionsRecieved |= Mask_DestinationOption;
                break;
            case OPT_ENCRYPTION_IDENTIFIER:
                InputValidArguments.pcKey = optarg;
                ucAllOptionsRecieved |= Mask_EncryptionOption;
                break;
            case OPT_DECRYPTION_IDENTIFIER:
                InputValidArguments.pcKey = optarg;
                ucAllOptionsRecieved |= Mask_DecryptionOption;
                break;
            default:
                tError = Error_GetOpt;
                tRetValue = Input_Invalid;
                break;
        };
    }
    
    if (tError != Error_GetOpt)
    {
        tError = FindError(argc, ucAllOptionsRecieved);
    }

    if (tError == Error_Count)
    {
        tEvaluatedData.tRoutine = FindRoutine(ucAllOptionsRecieved);
        FindRelativePath(argv[0], acRelativePath);
        if (tEvaluatedData.tRoutine != Routine_Help)
        {
            ProcessInputArguments(ucAllOptionsRecieved, &InputValidArguments);
        }
        tRetValue = Input_Valid;
    }
    else
    {
        tRetValue = Input_Invalid;
    }

    return tRetValue;
}

void Interface_GetValidInput(InputEvaluatedData_t *ptValidInput)
{
    ptValidInput->tRoutine = tEvaluatedData.tRoutine;
    strcpy(ptValidInput->acSource, tEvaluatedData.acSource);
    strcpy(ptValidInput->acDestination, tEvaluatedData.acDestination);
    strcpy(ptValidInput->acKey, tEvaluatedData.acKey);
}

void Interface_RegisterError(ErrorType_t tError2Register)
{
    if (tError < Error_Count)
    {
        tError = tError2Register;
    }
}

void Interface_PrintErrorDescription(void)
{
    if (tError < Error_Count)
    {
        fprintf(stderr,"Error\nPromt: ");
        fprintf(stderr, apcErrorMessage[tError]);
    }
}

void Interface_PrintHelp(void)
{
    FILE *ptHelpFile = NULL;
    char acPath2Help[MAX_PATH_LENGTH];
    char *pcBuffer = NULL;
    unsigned int uiBufferSize = 0;

    memset(acPath2Help, '\0', MAX_PATH_LENGTH);

    strcpy(acPath2Help, acRelativePath);
    strcat(acPath2Help, HELP_FILE);

    ptHelpFile = fopen(acPath2Help, "r");
    if (ptHelpFile != NULL)
    {
        uiBufferSize = Interface_FileSize(ptHelpFile, 0);

        pcBuffer = malloc(uiBufferSize);

        fread(pcBuffer, 1, uiBufferSize, ptHelpFile);
        fclose(ptHelpFile);
        printf("\t\t -- Program Usage Description --\n");
        fwrite(pcBuffer, 1, uiBufferSize, stdout);
        free(pcBuffer);
    }
    else
    {
        printf("Cant open Help file.\n");
    }
}

unsigned int Interface_FileSize(FILE *ptFile, int iOffset)
{
    unsigned int uiRetVal = 0;

    fseek(ptFile, 0, SEEK_END);
    uiRetVal = ftell(ptFile);
    rewind(ptFile);

    return uiRetVal - iOffset;
}



/* ---------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------- */
/* ------- All the folowing functions are local and serve auxilary purposes --------- */

static Routine_t FindRoutine(unsigned char ucValidInputOptions)
{
    Routine_t tRetValue = Routine_Count;
    /* Erase flags which indicates options such as "-f", "-s". It is possible  */
    /* cause this options accepted by two Routines simultaneously. So in order */
    /* to simplify [switch] statement where following variable will be passed. */
    ucValidInputOptions &= ~(Mask_DestinationOption | Mask_SourceOption);

    switch (ucValidInputOptions)
    {
        case Mask_HelpOption:
            tRetValue = Routine_Help;
            break;
        case Mask_EncryptionOption:
            tRetValue = Routine_Encrypt;
            break;
        case Mask_DecryptionOption:
            tRetValue = Routine_Decrypt;
            break;
        default:
            break;
    };
    return tRetValue;
}

/* #########################    WARNING    ########################## */
/*          The following function using "goto" statement.            */
/* Each goto in that function send program flow to the end of that    */
/* function before "return" statement. It doesn't jump out of scope   */
/* and befor and after variable defenition.                           */
static ErrorType_t FindError(int argc, unsigned char ucValidInputOptions)
{
    ErrorType_t tRetValue = Error_Count;
    if (argc < MIN_ARGC_VALUE)
    {
        tRetValue = Error_NoArguments;
        goto ret;
    }

    if (((ucValidInputOptions & Mask_HelpOption) != 0) && 
        (ucValidInputOptions & (~Mask_HelpOption)) != 0)
    {
        tRetValue = Error_NotUniqueOption;
        goto ret; 
    }
    else if ((ucValidInputOptions & Mask_HelpOption) != 0)
    {
        goto ret;
    }

    if ((ucValidInputOptions & (Mask_EncryptionOption | Mask_DecryptionOption)) == 
        (Mask_EncryptionOption | Mask_DecryptionOption))
    {
        tRetValue = Error_NotMatchingOptions;
        goto ret;
    }

    if (((ucValidInputOptions & Mask_SourceOption) == 0) || 
        ((ucValidInputOptions & (Mask_EncryptionOption | Mask_DecryptionOption)) == 0))
    {
        tRetValue = Error_NotEnoughOptions;
        goto ret;
    }

ret:
    return tRetValue;
}

static void ProcessInputArguments(unsigned char ucValidInputOptions, InputData_t const *ptInputArguments)
{
    char acOutFileName[MAX_FILE_NAME_LENTGTH];
    memset(acOutFileName, '\0', MAX_FILE_NAME_LENTGTH);
    
    if ((ucValidInputOptions & Mask_DestinationOption) == 0)
    {
        TakeInputFileName(ptInputArguments->pcSource, acOutFileName);
        strcpy(tEvaluatedData.acDestination, acRelativePath);
        strcat(tEvaluatedData.acDestination, DEFAULT_DESTINATION);
        if (tEvaluatedData.tRoutine == Routine_Encrypt)
        {
            strcat(tEvaluatedData.acDestination, FILE_PREFIX_ENCRYPTED);
        }
        else
        {
            strcat(tEvaluatedData.acDestination, FILE_PREFIX_DECRYPTED);
        }
        strcat(acOutFileName, FILE_EXTENSION);
        strcat(tEvaluatedData.acDestination, acOutFileName);
        printf("dest: %s\n", tEvaluatedData.acDestination);
    }
    else
    {
        strcpy(tEvaluatedData.acDestination, ptInputArguments->pcDestination);
    }

    strcpy(tEvaluatedData.acSource, ptInputArguments->pcSource);
    memset(tEvaluatedData.acKey, 0, KEY_SIZE);
    if (strlen(ptInputArguments->pcKey) <= KEY_SIZE)
    {
        strcpy(tEvaluatedData.acKey, ptInputArguments->pcKey);  
    }
    else
    {
        strncpy(tEvaluatedData.acKey, ptInputArguments->pcKey, KEY_SIZE);
    }
}


static void FindRelativePath(char const* cpExecutablePath, char *cpRelativePath)
{
    unsigned int uiStringLength2Exe = 0;
    char *pcLastFolderSeparator = NULL;
    
    pcLastFolderSeparator = strrchr(cpExecutablePath, FOLDER_SEPARATOR);
    uiStringLength2Exe = (unsigned int)(pcLastFolderSeparator - cpExecutablePath) + 1;
    strncpy(cpRelativePath, cpExecutablePath, uiStringLength2Exe);
}

static void TakeInputFileName(char *pcSourceFile, char *pcFileName)
{
    char *pcFileNameBeginning = NULL;
    char *pcFileNameEnd = NULL;

    if ((pcFileNameBeginning = strrchr(pcSourceFile, FOLDER_SEPARATOR)) == NULL)
    {
        pcFileNameBeginning = pcSourceFile;
    }
    else
    {
        pcFileNameBeginning++;
    }
    if ((pcFileNameEnd = strrchr(pcSourceFile, EXTENSION_SEPARATOR)) == NULL)
    {
        strcpy(pcFileName, pcFileNameBeginning);
    }
    else
    {
        strncpy(pcFileName, pcFileNameBeginning, pcFileNameEnd - pcFileNameBeginning);
    }
}