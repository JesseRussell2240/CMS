/* encrypt.h - Interface for the encryption functions (XOR, Vigenere)
 * By: Michael
 *
 */

#pragma once

// Function prototypes

void xorCipher(void* message, int messageLength, void* secretKey, int secretKeyLength, void* encBuf); 
void encryption();
void decryption();
