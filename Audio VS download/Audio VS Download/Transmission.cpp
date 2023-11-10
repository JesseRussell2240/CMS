/*
Author: Jesse Russell, Hayden Foster, Zach McEwin
Date: 1 Nov 2023
Details: Contains various audio file functions such as , play, save, load, record,
*/

#include <Windows.h>    
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "RS232Comm.h"
#include "Transmission.h"
#include "AudioRecorder.h"

//dosables warning for userinput
#pragma warning	(disable:4996)

 // Declare constants, variables and communication parameters
const int BUFSIZE = 280;							// Buffer size
#define AUDIO_BUFFER_SIZE 280
HANDLE hComRx;										// Pointer to the selected COM port (Receiver)
HANDLE hComTx;										// Pointer to the selected COM port (Transmitter)
int nComRate = 9600;								// Baud (Bit) rate in bits/second 
int nComBits = 8;									// Number of bits per frame
COMMTIMEOUTS timeout;
extern short iBigBuf[];								// Declare the external variable
extern long lBigBufSize;							// Declare the external variable


//these need a way to update!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// A commtimeout struct variable
wchar_t COMPORT_Rx[] = L"COM8";
wchar_t COMPORT_Tx[] = L"COM9";


//this code is called from the main, and passes the audio buffer as a paramater
int transmit(short* audioData, int dataSize){


	char userResult;
	char transmit = 'T';
	char receive = 'R';
	printf("Select the type of COM port:\n");
	printf("1. Virtual COM Port\n");
	printf("2. Physical COM Port\n");
	printf("Enter your choice (1 or 2): ");
	int comType;
	scanf("%d", &comType);


	if (comType == 1) {
		
		//printf("Virtual Comports Selected this code is commented out for now.\n");
		/*
		
		Virtual comport code goes here!
		
		
		
		*/

		
		printf("Using virtual COM port for transmission: %S\n", COMPORT_Tx);
		printf("Using virtual COM port for reception: %S\n", COMPORT_Rx);

		printf("Enter the bit rate: ");
		int rate;
		scanf("%d", &rate);
		setComRate(rate);

		
		initializeRxPort(COMPORT_Rx);
		Sleep(500);
		initializeTxPort(COMPORT_Tx);

		printf("Memory address of audioData: %p\n", audioData);
		transmitAudio(audioData, dataSize);
		receiveAudio(audioData, dataSize);

	



	}


	else if (comType == 2) {

		//once user selects Physical com ports it stays in this section of code
		//while (1) {
			printf("Enter an option:\n");
			printf("1. Transmit\n");
			printf("2. Receive\n");

			printf("Enter your choice (1, or 2): ");
			scanf_s(" %c", &userResult, 1);

			if (userResult == '1') {

				/* This code does not correctly update the comm port based on user input, i commentd it out but this needs to be figure out
				printf("Enter the COM port: ");
				wchar_t txPort[20];
				scanf_s("%s", txPort, 20);
				*/
				/*
				printf("Enter the message to transmit: ");
				char msgOut[BUFSIZE];
				scanf_s("%s", msgOut, BUFSIZE);
				printf("Enter the number of bits: ");
				int bits;
				scanf("%d", &bits);
				setComBits(bits);
				*/


				printf("Enter the bit rate: ");
				int rate;
				scanf("%d", &rate);
				setComRate(rate);
				
				initializeTxPort(COMPORT_Tx);

				//const short msgConverted = msgOut[];
				transmitAudio(audioData, dataSize);

			}


			else if (userResult == '2') {

				//wchar_t rxPort[20];
				/*
				printf("Enter the COM port: ");

				
				scanf_s("%s", rxPort, 20);

				printf("Enter the number of bits: ");
				int bits;
				scanf("%d", &bits);
				setComBits(bits);
				*/

				printf("Enter the bit rate: ");
				int rate;
				scanf("%d", &rate);
				setComRate(rate);

				initializeRxPort(COMPORT_Rx);
				//short receivedAudio[AUDIO_BUFFER_SIZE]; // Define a buffer to store received audio
				receiveAudio(audioData, dataSize); // Pass the buffer to store the received audio

			}
			else {
				printf("Invalid input. Please enter 1 or 2.\n");
			}
		//}

		//system("pause");

	}
	else {
		//system("pause");

	}

	return 0;
}



void setComBits(int bits) {
	nComBits = bits;
}

void setComRate(int rate) {
	nComRate = rate;
}

void initializeRxPort(const wchar_t* port) {
	initPort(&hComRx, COMPORT_Rx, nComRate, nComBits, timeout);
	Sleep(500);
}

void initializeTxPort(const wchar_t* port) {
	initPort(&hComTx, COMPORT_Tx, nComRate, nComBits, timeout);
	Sleep(500);
}


//removed out code to keep the recive port open and appending a special ending to the message
void transmitMessage(const char* msg) {
	char msgWithSenderID[BUFSIZE];
	//snprintf(msgWithSenderID, BUFSIZE, "%s %s", msg, senderID); // Append the sender ID to the message

	//XOR encode funtion

	outputToPort(&hComTx, msg, strlen(msg) + 1);
	Sleep(500);
	purgePort(&hComTx);
	CloseHandle(hComTx);
}

void receiveMessages() {
	char msgIn[BUFSIZE];


	Sleep(1500);
	DWORD bytesRead;
	bytesRead = inputFromPort(&hComRx, msgIn, BUFSIZE);
	msgIn[bytesRead] = '\0';
	printf("Received Message: %s\n", msgIn);
//	printf("Message: %s\n", msgIn);

	purgePort(&hComRx);
	CloseHandle(hComRx);

}

void transmitAudio(short* audioData, int dataSize) {
	//const char* charData = reinterpret_cast<const char*>(audioData);
	
	outputToPort(&hComTx, (char*)audioData, dataSize * sizeof(short) * 2);//this line causes the transmission to stop. Must be what is sent or size of sent
	//outputToPort(&hComTx, (char*)audioData, 192000);//this line causes the transmission to stop. Must be what is sent or size of sent


	purgePort(&hComTx);
	CloseHandle(hComTx);
}

void receiveAudio(short* audioData, int dataSize) {
	DWORD bytesRead;
	printf("recive audio called\n");
	bytesRead = inputFromPort(&hComRx, (char*)audioData, dataSize * sizeof(short));
	printf("bytes expected %d\n", dataSize);
	
	if (bytesRead == dataSize * sizeof(short)) {
		// The received data size matches the expected size

	}
	else {
		printf("error in recive audio helper function");
		// Handle an error, as the received data size is not as expected
	}

	purgePort(&hComRx);
	CloseHandle(hComRx);
}