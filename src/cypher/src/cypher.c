#include "cypher.h"


void Cypher_Encrypt(unsigned char *pcBuffer, unsigned char *pcInputKey)
{
    symmetric_key tSymetricKey;
    unsigned char acAesKey[KEY_SIZE];
    
    memset(acAesKey, 0, KEY_SIZE); /* Key initialization by '\0' */

    /* If pcInputKey not large enough then remainders bytes are '\0' */
    memcpy(acAesKey, pcInputKey, strlen(pcInputKey));

    aes_setup(acAesKey, KEY_SIZE, NUMBER_ROUNDS, &tSymetricKey);
    aes_ecb_encrypt(pcBuffer, pcBuffer, &tSymetricKey);
    aes_done(&tSymetricKey);
}

void Cypher_Decrypt(unsigned char *pcBuffer, unsigned char *pcInputKey)
{
    symmetric_key tSymetricKey;
    unsigned char acAesKey[KEY_SIZE];
    
    memset(acAesKey, 0, KEY_SIZE); /* Key initialization by '\0' */

    /* If pcInputKey not large enough then remainders bytes are '\0' */
    memcpy(acAesKey, pcInputKey, strlen(pcInputKey));

    aes_setup(acAesKey, KEY_SIZE, NUMBER_ROUNDS, &tSymetricKey);
    aes_ecb_decrypt(pcBuffer, pcBuffer, &tSymetricKey);
    aes_done(&tSymetricKey);
}

unsigned int Checksum(char *pcInput, unsigned int uiInputSize)
{
    crc32_state tMyCRC;
    unsigned int uiHash;

    crc32_init(&tMyCRC);
    crc32_update(&tMyCRC, pcInput, uiInputSize);
    crc32_finish(&tMyCRC, &uiHash, sizeof(unsigned int));
    return uiHash;
}

