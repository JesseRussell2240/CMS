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

//extern wchar_t COMPORT_Rx[];
//extern wchar_t COMPORT[];

 // Declare constants, variables and communication parameters
const int BUFSIZE = 280;							// Buffer size
#define AUDIO_BUFFER_SIZE 280
HANDLE hCom;										// Pointer to the selected COM port (Receiver)
int nComRate = 9600;								// Baud (Bit) rate in bits/second 
int nComBits = 8;									// Number of bits per frame
COMMTIMEOUTS timeout;
extern short iBigBuf[];								// Declare the external variable
extern long lBigBufSize;							// Declare the external variable


//setter function for comBits
void setComBits(int bits) {
	nComBits = bits;
}

//setter function for comRate
void setComRate(int rate) {
	nComRate = rate;
}

//port initilization helper
void initializePort( wchar_t* port) {
	initPort(&hCom, port, nComRate, nComBits, timeout);
	Sleep(50);
}

//removed out code to keep the recive port open and appending a special ending to the message
void transmitMessage(const char* msg) {
	//char msgWithSenderID[BUFSIZE];
	//snprintf(msgWithSenderID, BUFSIZE, "%s %s", msg, senderID); // Append the sender ID to the message

	outputToPort(&hCom, msg, strlen(msg) + 1);
	Sleep(500);
	purgePort(&hCom);
	CloseHandle(hCom);
}

void receiveMessages(char* msgBuffer, int* msgLength) {
	Sleep(1500);
	DWORD bytesRead;
	bytesRead = inputFromPort(&hCom, msgBuffer, BUFSIZE);
	msgBuffer[bytesRead] = '\0';
	*msgLength = bytesRead;
	//printf("Received Message: %s\n", msgBuffer);

	purgePort(&hCom);
	CloseHandle(hCom);
}

void transmitAudio(short* audioData, int dataSize) {
	outputToPort(&hCom, (char*) audioData, dataSize * sizeof(short));
	purgePort(&hCom);
	CloseHandle(hCom);
}

void receiveAudio(short* audioData, int dataSize) {

	DWORD bytesRead;
	printf("recive audio called\n");
	bytesRead = inputFromPort(&hCom, (char*)audioData, dataSize * sizeof(short));
	printf("bytes expected %d\n", dataSize);
	
	if (bytesRead == dataSize * sizeof(short)) {
		// The received data size matches the expected size
		PlayAudio(audioData, dataSize);
		
	}
	else {
		printf("error in recive audio. Expected size =! recived size...");
		// Handle an error, as the received data size is not as expected
	}

	

	purgePort(&hCom);
	CloseHandle(hCom);
}