/* main.cpp - Client for the Tx/Rx Program
 * Author: Michael A. Galle
 *
 */

#include <Windows.h>    
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "RS232Comm.h"

#pragma warning(disable : 4996)


// Function prototypes
int physicalPorts();
int virtualPorts();
void initializeRxPort(const wchar_t* port);
void initializeTxPort(const wchar_t* port);
void setComBits(int bits);
void setComRate(int rate);
void transmitMessage(const char* msg);
void receiveMessages();

 // Note: Comment out the Tx or Rx sections below to operate in single sided mode

 // Declare constants, variables and communication parameters
const int BUFSIZE = 140;							// Buffer size


// Communication variables and parameters
HANDLE hComRx;										// Pointer to the selected COM port (Receiver)
HANDLE hComTx;										// Pointer to the selected COM port (Transmitter)
int nComRate = 9600;								// Baud (Bit) rate in bits/second 
int nComBits = 8;									// Number of bits per frame
COMMTIMEOUTS timeout;								// A commtimeout struct variable
wchar_t COMPORT_Rx[] = L"COM3";
wchar_t COMPORT_Tx[] = L"COM3";



const char senderID[] = "3141592"; // You can change this to your desired sender ID.







int main() {

	char userResult;
	char transmit = 'T';
	char receive = 'R';

	while (1) {
		printf("Enter an option:\n");
		printf("1. Transmit\n");
		printf("2. Receive\n");
		
		printf("Enter your choice (1, or 2): ");
		scanf_s(" %c", &userResult, 1);

		if (userResult == '1') {
			printf("Select the type of COM port:\n");
			printf("1. Virtual COM Port\n");
			printf("2. Physical COM Port\n");
			printf("Enter your choice (1 or 2): ");

			//code for selecting ComType
			int comType;
			scanf("%d", &comType);

			printf("Enter the COM port: ");


			wchar_t txPort[20];
			scanf_s("%s", txPort, 20);

			printf("Enter the message to transmit: ");
			char msgOut[BUFSIZE];
			scanf_s("%s", msgOut, BUFSIZE);

			printf("Enter the number of bits: ");
			int bits;
			scanf("%d", &bits);
			setComBits(bits);

			printf("Enter the bit rate: ");
			int rate;
			scanf("%d", &rate);
			setComRate(rate);

			initializeTxPort(txPort);
			transmitMessage(msgOut);
		}


		else if (userResult == '2') {


			printf("Select the type of COM port:\n");
			printf("1. Virtual COM Port\n");
			printf("2. Physical COM Port\n");
			printf("Enter your choice (1 or 2): ");
			int comType;
			scanf("%d", &comType);

			printf("Enter the COM port: ");

			wchar_t rxPort[20];
			scanf_s("%s", rxPort, 20);

			printf("Enter the number of bits: ");
			int bits;
			scanf("%d", &bits);
			setComBits(bits);

			printf("Enter the bit rate: ");
			int rate;
			scanf("%d", &rate);
			setComRate(rate);

			initializeRxPort(rxPort);
			receiveMessages();
		}
		else {
			printf("Invalid input. Please enter 1 or 2.\n");
		}
	}

	system("pause");
}
/*//zachs main
int main() {

	char ports;
	char vPorts = 'V';
	char pPorts = 'P';


	while (1) {

		printf("Are you using Physcial (P) com ports or virtual (V) com ports? ");
		scanf_s("%c", &ports, 1);

		if (ports == pPorts) {
			physicalPorts();
		}
		else if (ports == vPorts) {
			virtualPorts();
		}
		else {
			return(1);
		}

		system("pause");
	}
}

*/


/*
int virtualPorts() {

	// Virtual Ports (via COM0COM)
	wchar_t COMPORT_Rx[] = L"COM6";					// COM port used for Rx (use L"COM7" for transmit program)
	wchar_t COMPORT_Tx[] = L"COM7";					// COM port used for Tx (use L"COM8" for transmit program)


	initPort(&hComTx, COMPORT_Tx, nComRate, nComBits, timeout);	// Initialize the Tx port
	Sleep(500);
	initPort(&hComRx, COMPORT_Rx, nComRate, nComBits, timeout);	// Initialize the Rx port
	Sleep(500);

	//Transmit Side
	char msgOut[] = "Hi there person";							// Sent message	

	outputToPort(&hComTx, msgOut, strlen(msgOut) + 1);			// Send string to port - include space for '\0' termination
	Sleep(500);													// Allow time for signal propagation on cable 
	purgePort(&hComTx);											// Purge the Tx port
	CloseHandle(hComTx);										// Close the handle to Tx port 

	//Recieve Side
	// Receive side  
	char msgIn[BUFSIZE];


	///////NEEDS WORK
	//WORKS but it is finding garbage in the /0
	while (1) {
		Sleep(1500);
		DWORD bytesRead;
		bytesRead = inputFromPort(&hComRx, msgIn, BUFSIZE);    // Receive string from port
		printf("Length of received msg = %d", bytesRead);
		msgIn[bytesRead] = '\0';
		printf("\nMessage Received: %s\n\n", msgIn);        // Display message from port
		purgePort(&hComRx);                                  // Purge the Rx port
		CloseHandle(hComRx);                                 // Close the handle to Rx port

		int chunkSize = 140;
		int i;
		for (i = 0; i < bytesRead; i += chunkSize) {
			char chunk[141];
			strncpy_s(chunk, msgIn + i, chunkSize);
			chunk[chunkSize] = '\0';

			// Check if the chunk contains a newline character
			if (strchr(chunk, '\n') != NULL) {
				break;  // Found a newline character, exit the loop
			}
		}

		if (i < bytesRead) {
			break;  // Found a newline character in one of the chunks, exit the loop
		}
	}
}

int physicalPorts() {

	//Physical ports
	wchar_t COMPORT_Rx[] = L"COM3";						// Check device manager after plugging device in and change this port
	wchar_t COMPORT_Tx[] = L"COM3";				// Check device manager after plugging device in and change this port
	// --> If COM# is larger than 9 then use the following syntax--> "\\\\.\\COM10"

	char userResult;
	char transmit = 'T';
	char receive = 'R';
	// Set up both sides of the comm link


	while (1) {
		printf("Would you like to transmit or receive? Enter 'T' for transmit 'R' for receive:");
		scanf_s(" %c", &userResult, 1);

		if (userResult == transmit || userResult == receive) {
			break; // Valid input, exit the loop
		}

		else {
			printf("Invalid input. Please enter 'T' for transmit or 'R' for receive.\n");
			while (getchar() != '\n'); // Clear the input buffer
		}
	}

	if (userResult == transmit) {

		initPort(&hComTx, COMPORT_Tx, nComRate, nComBits, timeout);	// Initialize the Tx port
		Sleep(500);

		//Transmit side 
		char msgOut[] = "Hi there person";							// Sent message	

		outputToPort(&hComTx, msgOut, strlen(msgOut) + 1);			// Send string to port - include space for '\0' termination
		Sleep(500);													// Allow time for signal propagation on cable 
		purgePort(&hComTx);											// Purge the Tx port
		CloseHandle(hComTx);										// Close the handle to Tx port 
	}

	else if (userResult == receive) {

		initPort(&hComRx, COMPORT_Rx, nComRate, nComBits, timeout);	// Initialize the Rx port


		// Receive side  
		char msgIn[BUFSIZE];


		///////NEEDS WORK
		//WORKS but it is finding garbage in the /0
		while (1) {
			Sleep(1500);
			DWORD bytesRead;
			bytesRead = inputFromPort(&hComRx, msgIn, BUFSIZE);    // Receive string from port
			printf("Length of received msg = %d", bytesRead);
			msgIn[bytesRead] = '\0';
			printf("\nMessage Received: %s\n\n", msgIn);        // Display message from port
			purgePort(&hComRx);                                  // Purge the Rx port
			CloseHandle(hComRx);                                 // Close the handle to Rx port

			int chunkSize = 140;
			int i;
			for (i = 0; i < bytesRead; i += chunkSize) {
				char chunk[141];
				strncpy_s(chunk, msgIn + i, chunkSize);
				chunk[chunkSize] = '\0';

				// Check if the chunk contains a newline character
				if (strchr(chunk, '\n') != NULL) {
					break;  // Found a newline character, exit the loop
				}
			}

			if (i < bytesRead) {
				break;  // Found a newline character in one of the chunks, exit the loop
			}
		}
	}
}
*/


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

void transmitMessage(const char* msg) {
	char msgWithSenderID[BUFSIZE];
	snprintf(msgWithSenderID, BUFSIZE, "%s %s", msg, senderID); // Append the sender ID to the message
	outputToPort(&hComTx, msgWithSenderID, strlen(msgWithSenderID) + 1);
	Sleep(500);
	purgePort(&hComTx);
	CloseHandle(hComTx);
}

void receiveMessages() {
	char msgIn[BUFSIZE];

	while (1) {
		Sleep(1500);
		DWORD bytesRead;
		bytesRead = inputFromPort(&hComRx, msgIn, BUFSIZE);
		msgIn[bytesRead] = '\0';
		printf("Received Message: %s\n", msgIn);

		// Find the sender ID at the end of the message
		char* senderPos = strstr(msgIn, senderID);
		if (senderPos != NULL) {
			*senderPos = '\0'; // Null-terminate the message where the sender ID starts
		}

		printf("Message: %s\n", msgIn);

		purgePort(&hComRx);
		CloseHandle(hComRx);
	}
}
