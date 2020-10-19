
#include "header.h"

static Header_t tHeader;

unsigned int Header_IsPresent(FILE *const ptFile)
{
    /* Value to be returned as a result of this function. Can be TRUE of FALSE. */
    unsigned int uiRetVal = 0;
    /* Value from file which will be compared with desired header magic number. */
    unsigned int uiHeaderNumber2Check = 0; 

    /* Read first bytes of quantity <HEADER_NUMBER_LENGTH>. */
    fread(&uiHeaderNumber2Check, HEADER_NUMBER_LENGTH, 1, ptFile);
    /* Check whether read bytes are the same as in magic number */
    uiRetVal = (uiHeaderNumber2Check == HEADER_NUMBER) ? TRUE : FALSE;

    /* Sinse header has two magic numbers at the begining and the end, moving */
    /* position to assuming second one.                                       */
    fseek(ptFile, HEADER_CONTENT_LENGTH, SEEK_CUR);
    fread(&uiHeaderNumber2Check, HEADER_NUMBER_LENGTH, 1, ptFile);
    /* Chech whether two magic numbers are present. */
    uiRetVal = (uiRetVal & (uiHeaderNumber2Check == HEADER_NUMBER)) ? TRUE : FALSE;
    /* Return position to the beginning of the file. */
    rewind(ptFile);

    return uiRetVal;
}

void Header_Update(unsigned int uiMagicNumber, unsigned int uiSize, unsigned int uiCheckSum)
{
    tHeader.tHeaderSeparated.uiMagicNumber1 = uiMagicNumber;
    tHeader.tHeaderSeparated.uiMagicNumber2 = uiMagicNumber;
    tHeader.tHeaderSeparated.uiSize = uiSize;
    tHeader.tHeaderSeparated.uiCheckSum = uiCheckSum;
}

void Header_Get(HeaderContent_t *ptDest)
{
    ptDest->uiMagicNumber1 = tHeader.tHeaderSeparated.uiMagicNumber1;
    ptDest->uiMagicNumber2 = tHeader.tHeaderSeparated.uiMagicNumber2;
    ptDest->uiSize = tHeader.tHeaderSeparated.uiSize;
    ptDest->uiCheckSum = tHeader.tHeaderSeparated.uiCheckSum;
}

void Header_Print(FILE *ptOutStream)
{
    fprintf(ptOutStream, "\nMagic Number: 0x%X\n", tHeader.tHeaderSeparated.uiMagicNumber1);
    fprintf(ptOutStream, "Size: %u (bytes)\n", tHeader.tHeaderSeparated.uiSize);
    fprintf(ptOutStream, "CheckSum: 0x%X\n", tHeader.tHeaderSeparated.uiCheckSum);
}

void Header_Append(FILE *const ptFile)
{
    fwrite(tHeader.acHeaderBytes, 1, HEADER_FULL_LENGTH, ptFile);
    rewind(ptFile);
}

void Header_Retrieve(FILE *const ptFile)
{
    fread(tHeader.acHeaderBytes, 1, HEADER_FULL_LENGTH, ptFile);
    rewind(ptFile);
}