/* encrypt.cpp - Implementation of the encryption functions (XOR, Vigenere)
 * By: Michael 
 *
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "encrypt.h"


void encryption();
void decryption();


char message[140], encBuf[140], decBuf[140], secretKey[140];
int messageLen, secretKeyLen;
int encrypt;
int i;


void encryption() {

    printf("Now encrypting ...\n");

    // Encrypt the message (xor)
    xorCipher(message, messageLen, secretKey, secretKeyLen, encBuf);
    printf("XOR Encrypted message in hex:");                               // Will not print as a string so print in HEX, one byte at a time
    for (i = 0; i < messageLen; i++) {
        printf("%02x", encBuf[i]);
    }
}


void decryption() {

    printf("Now decrypting ...");

    // Decrypt the message (XOR)
    xorCipher(encBuf, messageLen, secretKey, secretKeyLen, decBuf);
    printf("\nXOR Decrypted Message: %s\n\n\n\n", decBuf);                          // Can print as a string
}


void xorCipher(void* message, int messageLength, void* secretKey, int secretKeyLength, void* encBuf) {
    int i;
    char* msg, * key, * enc;                          // Cast buffers to single byte (char) arrays 
    msg = (char*)message;
    key = (char*)secretKey;
    enc = (char*)encBuf;
    for (i = 0; i < messageLength; i++) {
        enc[i] = msg[i] ^ key[i % secretKeyLength];  // XOR encrypt bytewise (loop the key as many times as required
    }
    enc[messageLength] = '\0';                       // Null terminate the ecrypted message
    encBuf = (void*)enc;                             // Encrypted/decrypted buffer   
}

