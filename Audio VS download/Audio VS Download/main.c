#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "crc.h"

#pragma warning (disable: 4996)

int main()
{
    //Declare variables

    
    unsigned char message[] = "123456789";  //Define the message as an unsigned char (0-255)
    int nBytes = strlen((char*)message);    //Calculate the number of bytes in the message
    crc compCRC;                            //Declare a varibale to hold the CRC (Variable type is crc)
    char CRCstring[8];                      //Declare an array of characters to store the CRC as a string
    char* sentMessage = (char*)malloc((nBytes + 8) * sizeof(unsigned char)); //Allocate memory for the array and the CRC is only 6 bytes
    int i;                                  //Declare a loop counter

    //compute CRCs and send message

    // Zap bits to see effect on CRC
    for(i=0; i<= nBytes; i++) {
        compCRC = crcSlow(message, nBytes); //Compute the CRC for the message using crcSlow
        sprintf(CRCstring," 0x%x", compCRC);//Format the CRC as a string. storing as a hexidecimal. adding deliminator and /0
        strcpy(sentMessage, (char*)message);//copy the message to the sentMessage buffer
        strcat(sentMessage, CRCstring);     //Add the CRC string to the message
        printf("Sent message with CRC: %s\n", sentMessage); //Print the string message with the CRC
        message[i] ='0';                    //
    }


    free(sentMessage); //Free the allocated memory for sentMessage
    return(0);
}