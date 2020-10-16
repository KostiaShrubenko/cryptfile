#include "cryptfile.h"


/* Stores data evaluated from console arguments/options. It is:             */
/* acKey            -- Key to cypher.                                       */
/* acDestination    -- path to output file.                                 */
/* acSource         -- path to input file.                                  */
/* tRoutine         -- routine to be executed.                              */

InputEvaluatedData_t tValidInput;

/* Encryption routine. */
void Encryption(void);
/* Decription routine. */
void Decryption(void);

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
                printf("Data encrypted\n");
                break;
            case Routine_Decrypt:
                Decryption();
                printf("Data decrypthed\n");
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


void Encryption(void)
{
    FILE* ptFileInput = NULL;
    FILE* ptFileOutput = NULL;
    /* Buffer to store temporary data from input file and output temporary data. */
    unsigned char *pcBuffer = NULL;
    unsigned int uiBufferSize = 0;  /* Size of buffer, depends from input file size.    */ 
    unsigned int uiChecksum = 0;    /* Check sum value evaluated with CRC32 algorithm.  */  

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
        uiBufferSize = Interface_FileSize(ptFileInput, 0);
        
        /* Allocate memmory with "uiBufferSize". */
        pcBuffer = malloc(uiBufferSize);
        fread(pcBuffer, 1, uiBufferSize, ptFileInput);  /* Read full file to buffer. */
        fclose(ptFileInput);                            /* Close file */


        Cypher_Encrypt(pcBuffer, tValidInput.acKey);    /* Encrypt buffer's content with key.    */
        uiChecksum = Checksum(pcBuffer, uiBufferSize);  /* Calculate checksum of encrypted data. */

        ptFileOutput = fopen(tValidInput.acDestination, "wb"); /* Open output file. */
        if (ptFileOutput == NULL)
        { /* File can't be opend or created. Print error and stop routine execution. */
            Interface_RegisterError(Error_OpenFile);
            Interface_PrintErrorDescription();
        }
        else
        {
            Header_Update(HEADER_NUMBER, uiBufferSize, uiChecksum); /* Generate header.             */
            Header_Append(ptFileOutput);                            /* Write header to output file. */

            /* Shift position in file after header. */
            fseek(ptFileOutput, HEADER_FULL_LENGTH, SEEK_SET);
            fwrite(pcBuffer, 1, uiBufferSize, ptFileOutput);
            fclose(ptFileOutput);

            /* Print header's content to stdout. */
            Header_Print(stdout);
        }
        free(pcBuffer);
    }
}

void Decryption(void)
{
    FILE* ptFileInput = NULL;
    FILE* ptFileOutput = NULL;
    /* Buffer to store temporary data from input file and output temporary data. */
    unsigned char *pcBuffer = NULL;
    unsigned int uiBufferSize = 0;  /* Size of buffer, depends from input file size.    */ 
    unsigned int uiChecksum = 0;    /* Check sum value evaluated with CRC32 algorithm.  */  
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
            uiBufferSize = Interface_FileSize(ptFileInput, HEADER_FULL_LENGTH);
            Header_Print(stdout); /* Print header's content to "stdout" */
            fseek(ptFileInput, HEADER_FULL_LENGTH, SEEK_SET); /* Move position in file after header. */
        }
        else
        { /* Header has not been found. */
            uiBufferSize = Interface_FileSize(ptFileInput, 0);
            printf("No header\n");
        }
        /* Allocate memmory with "uiBufferSize". */
        pcBuffer = malloc(uiBufferSize);
        fread(pcBuffer, 1, uiBufferSize, ptFileInput);
        fclose(ptFileInput);

        /* Calculate checsum of encrypted data in order to compare it for one in the header. */
        uiChecksum = Checksum(pcBuffer, uiBufferSize); 
        if (tHeaderData.uiCheckSum != uiChecksum)
        {
            printf("Calculated checksum does not match with one in the header.\nFile has been corupted\n");
        }
        else
        {
            printf("Calculated checksum matchs with one in the header.\nNo coruption.\n");
        }
        /* Decrypt buffer's content with given key */
        Cypher_Decrypt(pcBuffer, tValidInput.acKey);

        ptFileOutput = fopen(tValidInput.acDestination, "wb"); /* open output file. */
        if (ptFileOutput == NULL)
        {
            Interface_RegisterError(Error_OpenFile);
            Interface_PrintErrorDescription();
        }
        else
        {        
            fwrite(pcBuffer, 1, uiBufferSize, ptFileOutput);
            fclose(ptFileOutput);
        }
        free(pcBuffer);
    }
}
 