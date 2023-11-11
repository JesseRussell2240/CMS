// main.cpp : Client for the XOR / Vigenere encryption/decryption functions
//

#include <stdio.h>
#include <string.h>
#include "encrypt.h"

//Function slighly changed for easier integration into project
//Varibles need to be changed

void encryption();
void decryption();


char message[140], encBuf[140], decBuf[140], secretKey[140];
int messageLen, secretKeyLen;
int encrypt;
int i;

//helper function

void encrpytiondep() {

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

