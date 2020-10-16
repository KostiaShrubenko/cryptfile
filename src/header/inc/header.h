#ifndef HEADER_H
#define HEADER_H

/* This module is intended to give simple way to append header at   */
/* the beginning of the file.                                       */
/* NOTE: content of a header are stored correspondingly "endians"   */
/* usually it is "little endian", so if you oppen output file with  */
/* binary viewer you will see most-significant bytes be to the left */
/* side and less-significant bytes to the right.                    */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"

/* Header structure representation. */
/* NOTE: order is essential.        */
typedef struct {
    unsigned int uiMagicNumber1;
    unsigned int uiSize;
    unsigned int uiCheckSum;
    unsigned int uiMagicNumber2;
} HeaderContent_t;

/* Such a data structure allow us easily without any conversion form */
/* header from its structure representation.                         */
typedef union {
    HeaderContent_t tHeaderSeparated;
    char acHeaderBytes[HEADER_FULL_LENGTH];
} Header_t;

/* Check whether file contains header at the beginning. */
unsigned int Header_IsPresent(FILE *const ptFile);

/* Update header structure representation with corresponding arguments. */
void Header_Update(unsigned int uiMagicNumber, unsigned int uiSize, unsigned int uiChecksum);

/* Copy header structure to one pointed by argument. */
void Header_Get(HeaderContent_t *ptDest);

/* Print header values to "stdout". */
void Header_Print(FILE *ptOutStream);

/* Write header to file pointed by argument starting from current set position  */
/* in it. In the end it return position to the beginning of the file.           */
void Header_Append(FILE *const ptFile);

/* Read header from file given by argument.                     */
/* NOTE: this function does not check presence of the header.   */
void Header_Retrieve(FILE *const ptFile);


#endif /* HEADER_H */