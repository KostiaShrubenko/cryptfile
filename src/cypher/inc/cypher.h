#ifndef CYPHER_H
#define CYPHER_H

#include "config.h"
#include <tomcrypt.h>
#include <string.h>


void Cypher_Encrypt(unsigned char *pcBuffer, unsigned int uiBufferSize, unsigned char *pcInputKey);

void Cypher_Decrypt(unsigned char *pcBuffer, unsigned int uiBufferSize, unsigned char *pcInputKey);

unsigned int Checksum(char *pcInput, unsigned int uiInputSize);

#endif /* CYPHER_H */