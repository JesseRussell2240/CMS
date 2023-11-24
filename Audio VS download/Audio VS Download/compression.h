#pragma once

// Function prototypes
void encodeFile(const char* inputFileName, const char* outputFileName);
void decodeFile(const char* inputFileName, const char* outputFileName);
void encodeShorts(const short* inputArray, int inputSize, short** compressedData, long* compressedSize);
void decodeShorts(const short* compressedData, long compressedSize, short** decompressedData, long* decompressedSize);
int compressTXT(char* msgIn, char* msgOut, int msgSize);
int decompressTXT(char* msgIn, char* msgOut, int compressedSize, int decompressedSize);