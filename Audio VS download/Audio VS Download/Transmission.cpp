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

extern wchar_t COMPORT_Rx[];
extern wchar_t COMPORT_Tx[];

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
	
	//printf("size of transmitted audio is %d\n", dataSize * sizeof(short) * 2);


	//char* pntr = (char*)audioData;
	//PlayAudio(audioData, dataSize);
	
	//outputToPort(&hComTx, (char*)&audioData, dataSize * sizeof(short) * 2);
	outputToPort(&hComTx, (char*) audioData, (128000) * 2);

	purgePort(&hComTx);
	CloseHandle(hComTx);
}

void receiveAudio(short* audioData, int dataSize) {


	DWORD bytesRead;
	printf("recive audio called\n");
	//bytesRead = inputFromPort(&hComRx, (char*)audioData, dataSize * sizeof(short));
	bytesRead = inputFromPort(&hComRx, (char*)audioData, dataSize * 2);
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