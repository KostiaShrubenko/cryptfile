#include "cryptfile.h"


/* Stores data evaluated from console arguments/options. It is:             */
/* acKey            -- Key to cypher.                                       */
/* acDestination    -- path to output file.                                 */
/* acSource         -- path to input file.                                  */
/* tRoutine         -- routine to be executed.                              */

static InputEvaluatedData_t tValidInput;

/* Encryption routine. */
static void Encryption(void);
/* Decription routine. */
static void Decryption(void);

int main(int argc, char **argv)
{   
    if (Interface_ParceInput(argc, argv) == Input_Valid)
    {
        Interface_GetValidInput(&tValidInput);
        switch(tValidInput.tRoutine)
        {
            case Routine_Help:
                Interface_PrintHelp();
                break;
            case Routine_Encrypt:
                Encryption();
                break;
            case Routine_Decrypt:
                Decryption();
                break;
            case Routine_Count:
            default:
            break;   
        }
    }
    else
    {
        Interface_PrintErrorDescription();
    }
    return 0;
}


static void Encryption(void)
{
    FILE* ptFileInput = NULL;
    FILE* ptFileOutput = NULL;
    /* Buffer to store temporary data from input file and output temporary data. */
    unsigned char *pcBuffer = NULL;
    unsigned int uiTruncatedSize = 0;  /* Rounded data to be factor of <AES_BLOCK_SIZE>.    */ 
    unsigned int uiFileSize = 0;       /* Size of buffer to contain file data.              */
    unsigned int uiChecksum = 0;       /* Check sum value evaluated with CRC32 algorithm.   */  

    ptFileInput = fopen(tValidInput.acSource, "rb");
    /* Check whether file opend or created. */
    if (ptFileInput == NULL)
    { /* File can't be opend or created. Print error and stop routine execution. */
        Interface_RegisterError(Error_OpenFile);
        Interface_PrintErrorDescription();
    }
    else
    { /* File has been opend or created. */
        /* Calculate file size and set corresponding buffer size. */
        /* One must be added to buffersize for end of string.     */
        uiFileSize = Interface_FileSize(ptFileInput, 0);
        /* Round file size to factor of <AES_BLOCK_SIZE>. */
        uiTruncatedSize = uiFileSize - (uiFileSize % AES_BLOCK_SIZE);
        /* Allocate memmory with "uiBufferSize". */
        pcBuffer = malloc(uiFileSize);
        memset(pcBuffer, '\0', uiFileSize);           /* Initializate buffer to '\0'. */
        fread(pcBuffer, 1, uiFileSize, ptFileInput);  /* Read full file to buffer.    */
        fclose(ptFileInput);                          /* Close file                   */
        Cypher_Encrypt(pcBuffer, uiTruncatedSize, tValidInput.acKey);    /* Encrypt buffer's content with key.    */
        uiChecksum = Checksum(pcBuffer, uiFileSize);  /* Calculate checksum of encrypted data. */

        ptFileOutput = fopen(tValidInput.acDestination, "wb"); /* Open output file. */
        if (ptFileOutput == NULL)
        { /* File can't be opend or created. Print error and stop routine execution. */
            Interface_RegisterError(Error_OpenFile);
            Interface_PrintErrorDescription();
        }
        else
        {
            Header_Update(HEADER_NUMBER, uiFileSize, uiChecksum); /* Generate header.             */
            Header_Append(ptFileOutput);                          /* Write header to output file. */

            /* Shift position in file after header. */
            fseek(ptFileOutput, HEADER_FULL_LENGTH, SEEK_SET);
            fwrite(pcBuffer, 1, uiFileSize, ptFileOutput);
            fclose(ptFileOutput);

            /* Print header's content to stdout. */
            Header_Print(stdout);
            printf("\nData encrypted\n\n");
        }
        free(pcBuffer);
    }
}

static void Decryption(void)
{
    FILE* ptFileInput = NULL;
    FILE* ptFileOutput = NULL;
    /* Buffer to store temporary data from input file and output temporary data. */
    unsigned char *pcBuffer = NULL;
    unsigned int uiTruncatedSize = 0;  /* Rounded data to be factor of <AES_BLOCK_SIZE>.    */ 
    unsigned int uiFileSize = 0;       /* Size of buffer to contain file data.              */
    unsigned int uiChecksum = 0;       /* Check sum value evaluated with CRC32 algorithm.   */  
    HeaderContent_t tHeaderData;

    ptFileInput = fopen(tValidInput.acSource, "rb");
    /* Check whether file opend or created. */
    if (ptFileInput == NULL)
    { /* File can't be opend or created. Print error and stop routine execution. */
        Interface_RegisterError(Error_OpenFile);
        Interface_PrintErrorDescription();
    }
    else
    { /* File has been opend or created. */
        /* Check header presence. */
        if (Header_IsPresent(ptFileInput))
        { /* Header has been found. */
            Header_Retrieve(ptFileInput);   /* Read header.                     */
            Header_Get(&tHeaderData);       /* Copy header to local variable.   */
            /* Evaluate file size without size of header. */
            uiFileSize = Interface_FileSize(ptFileInput, HEADER_FULL_LENGTH);
            uiTruncatedSize = uiFileSize - (uiFileSize % AES_BLOCK_SIZE);
            Header_Print(stdout); /* Print header's content to "stdout" */
            fseek(ptFileInput, HEADER_FULL_LENGTH, SEEK_SET); /* Move position in file after header. */
        }
        else
        { /* Header has not been found. */
            uiFileSize = Interface_FileSize(ptFileInput, 0);
            uiTruncatedSize = uiFileSize - (uiFileSize % AES_BLOCK_SIZE);
            printf("No header\n");
        }
        /* Allocate memmory with "uiBufferSize". */
        pcBuffer = malloc(uiFileSize);
        memset(pcBuffer, '\0', uiFileSize);         /* Initializate buffer to '\0'. */
        fread(pcBuffer, 1, uiFileSize, ptFileInput);
        fclose(ptFileInput);
        /* Calculate checsum of encrypted data in order to compare it for one in the header. */
        uiChecksum = Checksum(pcBuffer, uiFileSize);
        printf("Newly calculated CheckSum: 0x%X\n", uiChecksum); 
        if (tHeaderData.uiCheckSum != uiChecksum)
        {
            printf("Calculated checksum does not match with one in the header.\nFile has been corupted\n");
        }
        else
        {
            printf("Calculated checksum matchs with one in the header.\nNo coruption.\n");
        }
        /* Decrypt buffer's content with given key */
        Cypher_Decrypt(pcBuffer, uiTruncatedSize, tValidInput.acKey);

        ptFileOutput = fopen(tValidInput.acDestination, "wb"); /* open output file. */
        if (ptFileOutput == NULL)
        {
            Interface_RegisterError(Error_OpenFile);
            Interface_PrintErrorDescription();
        }
        else
        {        
            fwrite(pcBuffer, 1, uiFileSize, ptFileOutput);
            fclose(ptFileOutput);
            printf("\nData decrypted\n\n");
        }
        free(pcBuffer);
    }
}
 