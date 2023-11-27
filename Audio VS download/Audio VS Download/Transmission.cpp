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
#include "Queues.h"


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

void transmitPayload(HeaderForPayload* Header, void* Payload) {
	//initPort(&hCom, port, nComRate, nComBits, timeout);				// Initialize the Tx port
	outputToPort(&hCom, Header, sizeof(Header));						// Send Header
	outputToPort(&hCom, Payload, (*Header).payloadSize);				// Send payload
	Sleep(500);															// Allow time for signal propagation on cable 
	purgePort(&hCom);													// Purge the Tx port
	CloseHandle(hCom);													// Close the handle to Tx port 
}

DWORD receivePayload(HeaderForPayload* Header, void** Payload) {
	// Note: Pointer to rxPayload buffer (pointer to a pointer) is passed to this function since this function malloc's the amount of memory required - need to free it in main()
	DWORD bytesRead;
	//initPort(&hCom, port, nComRate, nComBits, timeout);				// Initialize the Rx port


	inputFromPort(&hCom, Header, sizeof(Header));						// Read in Header first (which is a standard number of bytes) to get size of payload 
	*Payload = (void*)malloc((*Header).payloadSize);				// Allocate buffer memory to receive payload. Will have to recast these bytess later to a specific data type / struct / etc - rembmer top free it in main()
	bytesRead = inputFromPort(&hCom, *Payload, (*Header).payloadSize);// Receive payload 

	//link newPayloadNode = payloadQueue(Payload);
	//AddToQueue(newPayloadNode);

	purgePort(&hCom);													// Purge the Rx port
	CloseHandle(hCom);													// Close the handle to Rx port 
	return bytesRead;													// Number of bytes read
}

/*
link payloadQueue(void* Payload) {

	link NewNode = (link)malloc(sizeof(Node));

	if (!NewNode) {
		printf("Memory allocation error: Cannot add message to queue");
	}

	NewNode->Data.sid = Payload;
	NewNode->Data.rid = Payload;

	return NewNode;
}
*/