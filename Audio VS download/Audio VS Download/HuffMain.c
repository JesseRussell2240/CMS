#include <stdio.h>
#include <stdlib.h>

//Include the Huffman module
#include "huffman.h"  // Assuming you save the provided code in a file named "huffman.h"
//#include "Main.h"


#pragma warning(disable : 4996)


/*


int main() {


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
*/
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
