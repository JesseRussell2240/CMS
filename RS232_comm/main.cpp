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
	printf("Select the type of COM port:\n");
	printf("1. Virtual COM Port\n");
	printf("2. Physical COM Port\n");
	printf("Enter your choice (1 or 2): ");
	int comType;
	scanf("%d", &comType);


	if (comType == 1) {
		printf("Virtual Comports Selected\n");

		//Select com for tx
		printf("Enter the TxCOM port: ");
		wchar_t txPort[20];
		scanf_s("%s", txPort, 20);

		//select com for rx
		printf("Enter the RxCOM port: ");
		wchar_t rxPort[20];
		scanf_s("%s", rxPort, 20);

		//request message from user
		printf("Enter the message to transmit: ");
		char msgOut[BUFSIZE];
		scanf_s("%s", msgOut, BUFSIZE);


		//request bit input
		printf("Enter the number of bits: ");
		int bits;
		scanf("%d", &bits);
		setComBits(bits);

		printf("Enter the bit rate: ");
		int rate;
		scanf("%d", &rate);
		setComRate(rate);

		//initilize ports
		initializeRxPort(rxPort);
		initializeTxPort(txPort);

		//transmit then recive
		transmitMessage(msgOut);
		receiveMessages();


	}


	else if (comType == 2) {

		//once user selects Physical com ports it stays in this section of code
		while (1) {
			printf("Enter an option:\n");
			printf("1. Transmit\n");
			printf("2. Receive\n");

			printf("Enter your choice (1, or 2): ");
			scanf_s(" %c", &userResult, 1);

			if (userResult == '1') {

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
	else {
		system("pause");

	}






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

	//while (1) {
		Sleep(1500);
		DWORD bytesRead;
		bytesRead = inputFromPort(&hComRx, msgIn, BUFSIZE);
		msgIn[bytesRead] = '\0';
		printf("Received Message: %s\n", msgIn);

		// Find the sender ID at the end of the message
	//	char* senderPos = strstr(msgIn, senderID);
	//	if (senderPos != NULL) {
	//		*senderPos = '\0'; // Null-terminate the message where the sender ID starts
	//	}

		printf("Message: %s\n", msgIn);

		purgePort(&hComRx);
		CloseHandle(hComRx);
	//}
}
