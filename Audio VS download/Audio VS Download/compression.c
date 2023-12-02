#include <stdio.h>
#include <stdlib.h>

//Include the Huffman module
#include "huffman.h"  // Assuming you save the provided code in a file named "huffman.h"
#include "compression.h"


#pragma warning(disable : 4996)

int Compressionmain() {


    char inputFileName[256];
    char outputFileName[256];
    int choice;

    printf("Enter the input filename: ");
    scanf("%s", inputFileName);

    // Check if the file ends with ".huff"
    if (strstr(inputFileName, ".huff") != NULL) {                       //Check if compressed file
        printf("decoding....\n");
        printf("Enter the output filename: ");                          //Name of file to compress
        scanf("%s", outputFileName);
       // outputFileName* = strstr(inputFileName, "_compressed.huff");
        //calls helper function
        decodeFile(inputFileName, outputFileName);
        
    }
    else {
        printf("encoding...\n");
        strcpy(outputFileName, inputFileName);
        strcat(outputFileName, "_compressed.huff");                     
        encodeFile(inputFileName, outputFileName);
    }


    return 0;
}

void encodeFile(const char* inputFileName, const char* outputFileName) {

    //Declare file pointers/buffers
    FILE* inputFile;
    FILE* outputFile;
    unsigned char* inputBuffer;
    unsigned char* outputBuffer;
    int inputSize;
    int outputSize;

    // Open input file
    inputFile = fopen(inputFileName, "rb");

    // get file size
    fseek(inputFile, 0, SEEK_END);
    inputSize = ftell(inputFile);
    rewind(inputFile);



    //allocated memory
    inputBuffer = (unsigned char*)calloc(inputSize, sizeof(char)); //uses calloc to ensure no garbage is in memory location
 
    // Read input file into buffer
    fread(inputBuffer, 1, inputSize, inputFile);
    fclose(inputFile);

    // Allocate memory for output buffer //this equasion for size should be modified to use the correct size so we are more effecent
    outputBuffer = (unsigned char*)calloc(inputSize * 2, sizeof(char)); // Assume compression won't exceed 2x size this is for the tree

    // call huff compress
    outputSize = Huffman_Compress(inputBuffer, outputBuffer, inputSize);

    // Open output file
    outputFile = fopen(outputFileName, "wb");

    printf("Compression complete. Original size: %d bytes, Compressed size: %d bytes\n",inputSize, outputSize);

    fwrite(outputBuffer, 1, outputSize, outputFile);
    fclose(outputFile);
    
    free(inputBuffer);      //Free allocated memory for input buffer
    free(outputBuffer);     //Free allocated memory for output buffer
}

void decodeFile(const char* inputFileName, const char* outputFileName) {

    FILE* inputFile;
    FILE* outputFile;
    unsigned char* inputBuffer;
    unsigned char* outputBuffer;
    int inputSize;
    int outputSize;

    // Open input file
    inputFile = fopen(inputFileName, "rb");

    //file size
    fseek(inputFile, 0, SEEK_END);
    inputSize = ftell(inputFile);
    rewind(inputFile);

    // Allocate memory for buffers
    inputBuffer = (unsigned char*)calloc(inputSize, sizeof(char));
    outputBuffer = (unsigned char*)calloc(inputSize * 2, sizeof(char)); // Assume decompression won't exceed 2x size

    // Read input file into buffer
    fread(inputBuffer, 1, inputSize, inputFile);
    fclose(inputFile);
    

    // calls uncompress from huff
    Huffman_Uncompress(inputBuffer, outputBuffer, inputSize, inputSize * 2);

    // Open output file
    outputFile = fopen(outputFileName, "wb");

    // Write decompressed data to the output file
    fwrite(outputBuffer, 1, inputSize, outputFile);
    fclose(outputFile);

    //displays byte info
    printf("Decompression complete. Original size: %d bytes, Decompressed size: %d bytes\n", inputSize, inputSize);

    // free mem
    free(inputBuffer);
    free(outputBuffer);
}
void encodeShorts(const short* inputArray, int inputSize, short** compressedData, long* compressedSize) {
    unsigned char* inputBuffer;
    unsigned char* outputBuffer;
    int outputSize;

    // Allocate memory
    inputBuffer = (unsigned char*)calloc(inputSize * sizeof(short), sizeof(char));
    memcpy(inputBuffer, inputArray, inputSize * sizeof(short));

    // Allocate memory for output buffer
    outputBuffer = (unsigned char*)calloc(inputSize * 2 * sizeof(short), sizeof(char));

    // Call huff compress
    outputSize = Huffman_Compress(inputBuffer, outputBuffer, inputSize * sizeof(short));

    // Set external variables
    *compressedData = (short*)outputBuffer;
    *compressedSize = outputSize;

    printf("Compression complete. Original size: %d shorts, Compressed size: %d bytes\n", inputSize, outputSize);

    free(inputBuffer);
}

void decodeShorts(const short* compressedData, long compressedSize, short** decompressedData, long* decompressedSize) {
    unsigned char* inputBuffer;
    unsigned char* outputBuffer;
    int outputSize;

    // Allocate memory for buffers
    inputBuffer = (unsigned char*)calloc(compressedSize, sizeof(char));
    memcpy(inputBuffer, compressedData, compressedSize);

    outputBuffer = (unsigned char*)calloc(compressedSize * 2, sizeof(char));

    // Call uncompress from huff
    Huffman_Uncompress(inputBuffer, outputBuffer, compressedSize, compressedSize * 2);
    outputSize = compressedSize * 2;

    // Set external variables
    *decompressedData = (short*)outputBuffer;
    *decompressedSize = outputSize / sizeof(short);

    printf("Decompression complete. Original size: %d bytes, Decompressed size: %d shorts\n", compressedSize, *decompressedSize);

    free(inputBuffer);
}

// Function to compress a message using Huffman coding
int compressTXT(char* msgIn, char* msgOut, int msgSize) {
    if (msgSize >= 250) {
        printf("Error: Message size exceeds maximum allowed size.\n");
        return -1;
    }

    // Call Huffman compression function
    printf("compresing message :%c", &msgIn);
    int compressedSize = Huffman_Compress((unsigned char*)msgIn, (unsigned char*)msgOut, msgSize);

    if (compressedSize <= 0) {
        printf("Error: Compression failed.\n");
        return -1;
    }


    return compressedSize;

}


// Function to decompress a message using Huffman coding
int decompressTXT(char* msgIn, char* msgOut, int compressedSize, int decompressedSize) {
    if (decompressedSize >= 1000) {
        printf("Error: Decompressed message size exceeds maximum allowed size.\n");
        return -1;
    }

   // printf("compresed message :%c", &msgIn);
  
    // Call Huffman decompression function
    //int outsize;


    Huffman_Uncompress((unsigned char*)msgIn, (unsigned char*)msgOut, compressedSize, decompressedSize);

//    printf("decompressed message :%c", &msgOut);

    return decompressedSize;
}