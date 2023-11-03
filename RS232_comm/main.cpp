/* main.cpp - Client for the Tx/Rx Program
 * Author: Michael A. Galle
 *
 */

#include <Windows.h>    
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "RS232Comm.h"

int physicalPorts();
int virtualPorts();

 // Note: Comment out the Tx or Rx sections below to operate in single sided mode

 // Declare constants, variables and communication parameters
const int BUFSIZE = 140;							// Buffer size


// Communication variables and parameters
HANDLE hComRx;										// Pointer to the selected COM port (Receiver)
HANDLE hComTx;										// Pointer to the selected COM port (Transmitter)
int nComRate = 9600;								// Baud (Bit) rate in bits/second 
int nComBits = 8;									// Number of bits per frame
COMMTIMEOUTS timeout;								// A commtimeout struct variable

// The client - A testing main that calls the functions below

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