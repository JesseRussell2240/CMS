/*	
Author: Jesse Hayden Zach
Date: 1 Nov 2023
Details: Tersting mainline for sub programs Transmission.cpp and AudioRecorder.cpp
*/

//dosables warning for userinput
#pragma warning	(disable:4996)

//uses provided and windows librarys sound, and stdio
#include "sound.h"
#include <stdio.h>
#include <windows.h>
#include "AudioRecorder.h"
#include "Transmission.h"
#include <time.h>
#include "message.h"
#include <stdlib.h>
#include "encrypt.h"
#include "compression.c"
#include "Queues.h"
#include <thread>
#include "VoteOn.h"
#include "main.h"
#include "RS232Comm.h"
//#include "crc.h"
		
extern HeaderForPayload;
extern short iBigBuf[];								// Declare the external variable
extern long lBigBufSize;							// Declare the external variable


// Define a structure to store COM port, bit rate, external variables, and audio settings
typedef struct {
	wchar_t comPort[20];
	int baudRate;
	short* bigBuf;
	long bigBufSize;
	int audioMessageLength;
	int audioBitRate;
	int encryption;
	int compression;
	int header;
	int priority;
	int sid;
	int rid;
	int payloadError;
	int headerError;
} ComSettings;

ComSettings settings;



/**************************************************************************************************************************

												HELPER FUNCTIONS FOR MAIN

***************************************************************************************************************************/

/**************************************************************************************************************************
-Write to settings
-Print the information inside the header
-Read last updated settings from text file
These function are used by the main to help store and retrieve user inputted data from the setting as well as print some of 
these edited setting that are in the header
**************************************************************************************************************************/


//This function is called everytime a user updates a setting and immeadiatly accesses the settings.txt file and changes it to
//To the user desired settings

void writeSettingsToFile(ComSettings* settings, const char* filename) {
	FILE* file = fopen(filename, "w");
	printf("test");
	if (file != NULL) {
		fwprintf(file, L"COMPORT=%s\n", settings->comPort);
		fwprintf(file, L"BITRATE=%d\n", settings->baudRate);
		fwprintf(file, L"AUDIOMESSAGELENGTH=%d\n", settings->audioMessageLength);
		fwprintf(file, L"AUDIOBITRATE=%d\n", settings->audioBitRate);
		fwprintf(file, L"ENCRYPTION=%d\n", settings->encryption);
		fwprintf(file, L"COMPRESSION=%d\n", settings->compression);
		fwprintf(file, L"HEADER=%d\n", settings->header);
		fwprintf(file, L"PRIORITY=%d\n", settings->priority);
		fwprintf(file, L"SID=%d\n", settings->sid);
		fwprintf(file, L"RID=%d\n", settings->rid);
		fwprintf(file, L"PAYLOADERROR=%d\n", settings->payloadError);
		fwprintf(file, L"RID=%d\n", settings->headerError);
		fclose(file);


	}
	else {
		printf("Write settings function failed");
	}
}


//This function will print the most up to date header info
//that theser would have updated in the settings

void printHeaderInfo(const header& h) {
	printf("Sender ID: %d\n", h.sid);
	printf("Receiver ID: %d\n", h.rid);
	printf("Priority: %d\n", h.priority);
	printf("Sequence Number: %d\n", h.seqNum);
	printf("Payload Size: %ld bytes\n", h.payloadSize);
	printf("Payload Type: %d\n", h.payLoadType);
	printf("Encryption: %d\n", h.encryption);
	printf("Compression: %d\n", h.compression);
}


// This function is called at the start of the main to retirve 
// the most recent setting from the settings.txt file 

void readSettingsFromFile(ComSettings* settings, const char* filename) {

	FILE* file = fopen(filename, "r");

	if (file != NULL) {
		wchar_t line[256];

		while (fgetws(line, sizeof(line) / sizeof(line[0]), file) != NULL) {
			//use of swscanf because comport is stored as a wide char
			swscanf(line, L"COMPORT=%s", settings->comPort);
			swscanf(line, L"BITRATE=%d", &settings->baudRate);
			swscanf(line, L"AUDIOMESSAGELENGTH=%d", &settings->audioMessageLength);
			swscanf(line, L"AUDIOBITRATE=%d", &settings->audioBitRate);
			swscanf(line, L"ENCRYPTION=%d", &settings->encryption);
			swscanf(line, L"COMPRESSION=%d", &settings->compression);
			swscanf(line, L"HEADER=%d", &settings->header);
			swscanf(line, L"PRIORITY=%d", &settings->priority);
			swscanf(line, L"SID=%d", &settings->sid);
			swscanf(line, L"RID=%d", &settings->rid);
			swscanf(line, L"PAYLOADERROR=%d", &settings->payloadError);
			swscanf(line, L"HEADERERROR=%d", &settings->headerError);
		}
		fclose(file);
	}
	else {
		printf("Write settings function failed");
	}
}


/**************************************************************************************************************************

											Coded Messaging System - MAIN

***************************************************************************************************************************/

/**************************************************************************************************************************
 main is split up into 9 different cases
 1. Record Audio
 2. Play last saved recording form buffer
 3. Load audio file from and seperate file into buffer
 4. Save most recent recording into file directory
 5. Transmit or recieve audio messages
 6. Transmit or recieve text messages
 7. Update settings in settings.txt file
 8. Run dignostics check on different features

**************************************************************************************************************************/

int	main(int argc, char* argv[]) {

	char filePath[150];
	char save;
	char replay;
	char c;						 // used to flush extra input
	int n;                       // Declare 'n' here
	int randomNum;
	int numberOfQuotes;
	long int* quoteIndices;
	int* quoteLengths;
	int bytesRead = 0;
	int option;
	int ChangeSettings = -1;
	int choice2;

	//seed random number so fortion cookies qoute can be genorated
	srand((unsigned int)time(NULL));

	// Read settings from file on start up
	readSettingsFromFile(&settings, "settings.txt");


	//menu loop
	do {
		//system("cls");
		//prints main menu options and request input
		printf("\nOptions:\n");
		printf("1. Record audio into buffer\n");
		printf("2. Play audio in buffer\n");
		printf("3. Load audio from file into buffer\n");
		printf("4. Save audio in buffer\n");
		printf("5. Transmit audio in buffer\n");
		printf("6. Transmit text message in buffer\n");
		printf("7. Settings\n");
		printf("8. Diagnostic Testing\n");
		printf("9. Exit\n");
		printf("Enter your choice: ");
		scanf_s("%d", &option);

		//used to flush the input buffer when user incorectly enters input
		while ((c = getchar()) != '\n' && c != EOF) {}


		//switch case for various user inputs
		switch (option) {





/**************************************************************************************************************************

											CASE 1: RECODRING AUDIO

**************************************************************************************************************************/

/**************************************************************************************************************************
	
	Recording audio and storing in local buffer.
	Case 1 is where the user will record messages, before choosing what to do with them
	from here the user can either record, rerecord their message or leave the case
	Recording audio will overwrite other audio files in buffer unless saves otherwise. Buffer is just meant to hold temporarly

**************************************************************************************************************************/



		case 1:	// Recording audio case
		{
			int playback;
			int rerecord = 1;

			while (rerecord == 1) {

				system("cls");
				//calls helper function
				RecordAudioTB(iBigBuf, lBigBufSize, settings.audioMessageLength, settings.audioBitRate * 1000);
				printf("\nDo you want to hear the recording (1)?\n");
				printf("Do you want to rerecord the message (2)?\n");
				printf("Exit (3)\n");
				scanf_s("%d", &playback);

				if (playback == 1) {
					printf("playing audio recording:\n");
					PlayAudio(iBigBuf, lBigBufSize);
					rerecord = 0;
				}
				else if (playback == 2) {
					rerecord = 1;
				}
				else if (playback == 3) {
					rerecord = 0;
					system("cls");
				}
				else {
					printf("Invalid option. Please choose 1 or 2");
				}
			}
		}
			break;


/**************************************************************************************************************************

											CASE 2: PLAY RECODRING FROM BUFFER 

**************************************************************************************************************************/

/*************************************************************************************************************************

case 2 will call the function Play audio and and play the most recent audio recording fomr the buffer.

**************************************************************************************************************************/

		case 2:
		{
			system("cls");
			//calls helper to play audio
			printf("\nPlaying recording from buffer\n");
			PlayAudio(iBigBuf, lBigBufSize);
			break;
		}


/**************************************************************************************************************************

											CASE 3: LOAD SAVED AUDIO RECORDING FILE INTO BUFFER

***************************************************************************************************************************/

/**************************************************************************************************************************
* 
* Case 3 loads a file from a user specidfied file directory into the audio buffer
* Loading a file will overwrite any other audio recodings currently in the buffer 
* 
***************************************************************************************************************************/
		case 3:
			system("cls");
			//user option for loading into buff from audio file

			//file loading code
			printf("Enter the path to the audio file to load: ");
			fgets(filePath, sizeof(filePath), stdin);
			//remove line end char
			n = strlen(filePath);  // Assign to 'n' (declared before the switch)
			filePath[n - 1] = '\0';
			//move audio to buffer
			LoadAudio(iBigBuf, lBigBufSize, filePath);

			//calls helper function to play audio recording
			printf("\nPlaying recording from loaded file ...\n");
			PlayAudio(iBigBuf, lBigBufSize);
			break;


/**************************************************************************************************************************
* 
*										CASE 4: SAVE AUDIO INTO USER DESIRED FILE DIRECTORY
* 
**************************************************************************************************************************/

/*************************************************************************************************************************
* 
* case 4 allows user to take the audio recoding that is stored in the buffer, and save it to a desier 
* file directory
* 
* ************************************************************************************************************************/


		case 4:

			system("cls");
			//file loading code
			printf("Enter the path to save the audio file: ");
			fgets(filePath, sizeof(filePath), stdin);

			//remove line end char
			n = strlen(filePath);  // Assign to 'n' (declared before the switch)
			filePath[n - 1] = '\0';

			//move audio to buffer
			SaveAudio(iBigBuf, lBigBufSize, filePath);
			printf("\Saved audio to file ...\n");
			break;


/**************************************************************************************************************************
* 
*									CASE 5: TRANSMITTING AND RECIEVING AUDIO MESSAGES
* 
**************************************************************************************************************************/

/*************************************************************************************************************************
* 
* In the case the user will select what statio they are operating in. Transmit or recieve station.
* 
***************************************************************************************************************************/ 

		case 5:

			system("cls");
			char userResultthree;

			printf("Options:\n");
			printf("1. Transmit\n");
			printf("2. Receive\n");
			printf("Enter your choice (1 or 2): ");
			scanf_s(" %c", &userResultthree, 1);



			//
			//***********************************************************************************************************************
			//	
			//									transmit audio message


			//logic for audio message transmission
			if (userResultthree == '1') {

				//build the header for the message based on users predefined settings and for audio transmission
				HeaderForPayload header;
				header.sid = settings.sid;
				header.rid = settings.rid;
				header.priority = settings.priority;
				header.payLoadType = 1; 
				header.encryption = settings.encryption;
				header.compression = settings.compression;
				header.payloadSize = lBigBufSize + 1;
				
				
				
				

				//logic for compression of text message transmission
				if (settings.compression != 0) {
					header.compression = lBigBufSize;

					
				//	encodeShorts(iBigBuf, sizeof(lBigBufSize) / sizeof(short), tempBuf, &tempSize);

				//	memcpy(iBigBuf, tempBuf, tempSize);

					//printf("\ntest\n");
				//	printf("Length of input message: %d\n", header.compression);
				//	printf("Compressed message: %d\n", tempSize);

				}

				//logic for encryption of text transmission
				if (settings.encryption == 1) {

					char secretKey[10] = "314159265";
					int keyLength = 10;
				//	memcpy(iBigBuf, tempBuf, tempSize);


				//	xorCipher(msgOut, msgSize, secretKey, keyLength, tmpMsg);
				//	printf("Encrypted message: %d\n", msgOut);

				}


				//set the payload size in the header after compression/encription etc are completed.
				
			


				printHeaderInfo(header);
				setComRate(settings.baudRate);
				initializePort(settings.comPort);
				transmitPayload(&header, (void*)iBigBuf, settings.headerError);



			}


			//
			//***********************************************************************************************************************
			//	
			//									logic for reciving text message

			else if (userResultthree == '2') {


				HeaderForPayload recivedHeader;
				void* receivedPayload;
				// Receive incoming header and payload
				setComRate(settings.baudRate);
				initializePort(settings.comPort);
				DWORD bytesRead;
				link newNode;

				bytesRead = receivePayload(&recivedHeader, &receivedPayload, settings.headerError);

				printHeaderInfo(recivedHeader);

				if (bytesRead == recivedHeader.payloadSize) {
					// Cast the received payload back to a character array
					memcpy(iBigBuf, receivedPayload, recivedHeader.payloadSize);
					free(receivedPayload);
			//		strcpy(messageBuffer, receivedExample);
				//	printf("\nThe message buffer is: %s\n", messageBuffer);
				}


				/*
				
				
				if (recivedHeader.payLoadType == 0) {
					printf("recived data was text not audio\n");
					break;
				}


				printf("compression status :%d", recivedHeader.compression);
				if (recivedHeader.compression != 0) {

					int resultLength = 250;
				//	int decompressedSize = decompressTXT(messageBuffer, tmpMsg, recivedHeader.payloadSize, resultLength); //was hardcoded so it always returned 250, changed it to resultLength for now idk if that solves it tho

				//	tmpMsg[recivedHeader.compression] = '\0';
				//	printf("\nUncompressed message: %s\n", tmpMsg);

			//		strcpy(messageBuffer, tmpMsg);
				}

				printf("encryption status :%d", recivedHeader.encryption);
				//logic to decrypt recived text message
				if (recivedHeader.encryption == 1) {
					//strcpy(messageBuffer, tmpMsg);
					//printf("\nEncryption is ON!!!!!\n");
					char secretKey[10] = "314159265";
					int keyLength = 10;


			//		xorCipher(messageBuffer, strlen(messageBuffer), secretKey, keyLength, tmpMsg);

					//printf("\nXOR Decrypted Message: %s\n", messageBuffer);
				}
				*/
			}
			break;

/***************************************************************************************************************************
* 
*									CASE 6: TRANSMITTING AND RECIEVING TEXT MESSAGES
* 
***************************************************************************************************************************/

/**************************************************************************************************************************
* 
* Case 6 will allow the user to choose their home station. Transmission or recieving. 
* Transmitting the user will have the option:
* - Send Custom message
* - Generate a random quote from a text file and send it
* The reciever will have the option:
* 
* 
***************************************************************************************************************************/

		case 6:
			
			system("cls");
			char userResultTwo;

			printf("Options:\n");
			printf("1. Transmit\n");
			printf("2. Receive\n");
			printf("Enter your choice (1 or 2): ");
			scanf_s(" %c", &userResultTwo, 1);

			//Transmit Text
			if (userResultTwo == '1') {

				//build the header for the message based on users predefined settings and for text message transmission
				HeaderForPayload header;
				header.sid = settings.sid;
				header.rid = settings.rid;
				header.priority = settings.priority;
				header.payLoadType = 0; //set as 0 for text
				header.encryption = settings.encryption;
				header.compression = settings.compression;
				header.crcOnPayload = settings.payloadError;

			//	header.voteOn = settings.headerError;
				

				// User input for text message
				char userResultThree;
				printf("Options:\n");
				printf("1. Custom Message\n");
				printf("2. Generate Message\n");
				printf("Enter your choice (1 or 2): ");
				scanf_s(" %c", &userResultThree, 1);

				char msgOut[250];					//this should not be hardcoded to 250
				int msgSize = 250;			//this should not be hardcoded to 250

				//logic for cutom text message transmission CUSTOM
				if (userResultThree == '1') {
					printf("Enter the text message to transmit: ");
					scanf(" %[^\n]s", msgOut);
				}

				//logic for auto genorating text message from random qoute in fourtion cookies file
				else if (userResultThree == '2') {
					InitQueue();

					//loadPhoneBook("PhoneBook.txt");
					//code calls queues to q fourtion cookies and then randomly get a qoute
					numberOfQuotes = fnumQuotes();
					quoteIndices = fquoteIndices(numberOfQuotes);
					quoteLengths = fquoteLength(numberOfQuotes, quoteIndices);
					randomNum = frandNum(0, numberOfQuotes - 1);
					char messageBuffer[MAX_QUOTE_LENGTH];
					bytesRead = GetMessageFromFile(messageBuffer, MAX_QUOTE_LENGTH, randomNum, numberOfQuotes, quoteIndices, quoteLengths);

					// Print the random message
					printf("\nRandom Message:\n%s\n", messageBuffer);
					strcpy(msgOut, messageBuffer);
					free(quoteIndices);
					free(quoteLengths);
				}

				else {
					printf("Invalid input. Please enter 1 or 2.\n");
				}

				//set the msgSize depending on number of chars user entered.
				msgSize = strlen(msgOut);
				char tmpMsg[250];

				if (settings.payloadError == 1) {

					//zach CRC code for transmission goes here

					//use msgOut as input and when you are done set msgOut to the new CRC message use tmpMsg
					strcpy(tmpMsg, msgOut);
					//CRC(msgOut);

					printf("The computed CRC message is: %s\n", msgOut);

				}

				//logic for encryption of text transmission
				if (settings.encryption == 1) {

					char secretKey[10] = "314159265";
					int keyLength = 10;
					strcpy(tmpMsg, msgOut);

					xorCipher(tmpMsg, msgSize, secretKey, keyLength, msgOut);

					printf("Encrypted message: %d\n", msgOut);

				}

				//logic for compression of text message transmission
				if (settings.compression != 0) {
					header.compression = msgSize;
					
					strcpy(tmpMsg, msgOut);
					msgSize = compressTXT(tmpMsg, msgOut, msgSize);

					//printf("\ntest\n");
					printf("Length of input message: %d\n", msgSize);
					printf("Compressed message: %d\n", tmpMsg);

				}

				

				
				//set the payload size in the header after compression/encription etc are completed.
				header.payloadSize = msgSize + 1;


				//printHeaderInfo(header);
				setComRate(settings.baudRate);
				initializePort(settings.comPort);
				transmitPayload(&header, (void*)msgOut, settings.headerError);


				
			}

			//Recieve Text
			else if (userResultTwo == '2') {

				InitQueue(); //Initialize the queue
				loadPhoneBook("PhoneBook.txt"); //Access phonebook.txt and load data
				int userMessageCount = numOfMessages();

				int choice;
				do {
					printf("\nYou have %d messages in the queue", userMessageCount);
					printf("\n1. Receive Message\n");
					printf("2. Display Received Messages\n");
					printf("3. Exit\n");
					printf("Enter your choice: ");
					scanf("%d", &choice);

					HeaderForPayload recivedHeader;

					void* receivedPayload;
					// Receive incoming header and payload
					setComRate(settings.baudRate);
					initializePort(settings.comPort);
					DWORD bytesRead;
					link newNode;

					int messageLength;
					char messageBuffer[260];
					int senderIDSearch;

					//switch case for queuing results of reciving
					switch (choice) {
						//case 1 for reciving text and adding to queue
					case 1:
						bytesRead = receivePayload(&recivedHeader, &receivedPayload, settings.headerError);

						printHeaderInfo(recivedHeader);

						if (bytesRead == recivedHeader.payloadSize) {
							// Cast the received payload back to a character array
							char* receivedExample = (char*)(receivedPayload);
							strcpy(messageBuffer, receivedExample);
							printf("\nThe message buffer is: %s\n", messageBuffer);
						}
						if (recivedHeader.payLoadType == 1) {
							printf("recived data was audio not text\n");
							break;
						}

						char tmpMsg[250];
						printf("compression status :%d", recivedHeader.compression);
						if (recivedHeader.compression != 0) {

							int resultLength = 250;
							int decompressedSize = decompressTXT(messageBuffer, tmpMsg, recivedHeader.payloadSize, resultLength); //was hardcoded so it always returned 250, changed it to resultLength for now idk if that solves it tho

							tmpMsg[recivedHeader.compression] = '\0';
							printf("\nUncompressed message: %s\n", tmpMsg);

							strcpy(messageBuffer, tmpMsg);
						}

						printf("encryption status :%d", recivedHeader.encryption);
						//logic to decrypt recived text message
						if (recivedHeader.encryption == 1) {


							char secretKey[10] = "314159265";
							int keyLength = 10;


							xorCipher(messageBuffer, strlen(messageBuffer), secretKey, keyLength, tmpMsg);
							strcpy(messageBuffer, tmpMsg);

							printf("\nDecrypted message: %s\n", messageBuffer);

						}

						if (recivedHeader.crcOnPayload == 1) {
						
								//zach crc check of recived message goes here
								//input should be messageBuffer and when you are done you need to set the messageBuffer back to the actual message
							
								//add a print statment saying what the orgional message is
						}

						//	printf("adding to queue");
							// Create a new node for the received message
						newNode = (link)malloc(sizeof(Node));
						newNode->Data.sid = recivedHeader.sid;
						newNode->Data.rid = recivedHeader.rid;
						newNode->Data.priority = recivedHeader.priority;
						strcpy(newNode->Data.message, messageBuffer);  // Copy the received message

						// Add the new node to the queue
						AddToQueue(newNode);
						savePhoneBook("PhoneBook.txt");

						break;

						//case 2 options for dequeuing or displaying queue.
					case 2:
						if (IsQueueEmpty()) {
							printf("Queue is empty. Nothing to dequeue.\n");
							break;
						}

						system("cls");
						do {
							printf("\n1. Search Queue by Sender ID\n");
							printf("2. Display Entire Queue\n");
							printf("3. Display Queue by Priority\n");
							printf("4. Dequeue in FIFO\n");
							printf("5. Dequeue in LIFO\n");
							printf("6. Dequeue by Priority\n");
							printf("7. Exit\n");
							printf("Enter your choice: ");
							scanf("%d", &choice2);

							link dequeuedNode;

							//queue display option control
							switch (choice2) {
							case 1:
								printf("\nEnter a sender ID to view their message count:");
								scanf("%d", &senderIDSearch);
								printf("\nUser %d has sent you %d messages", senderIDSearch, numOfUserMessages(senderIDSearch));
								break;
							case 2:
								printf("\nDisplaying Entire Queue:\n");
								PrintQueueContents();
								break;
							case 3:
								printf("\nDisplaying Queue by Priority:\n");
								PrintQueueContentsByPriority();
								break;
							case 4:

								dequeuedNode = DeQueue();
								printf("Dequeued in FIFO:\n\n Message: %s\n, Sender ID: %d\n, Receiver ID: %d\n, Priority: %c\n\n\n",
									dequeuedNode->Data.message, dequeuedNode->Data.sid, dequeuedNode->Data.rid,
									dequeuedNode->Data.priority);
								//free(dequeuedNode);
								savePhoneBook("PhoneBook.txt");

								break;
							case 5:
								DequeueLIFO();
								savePhoneBook("PhoneBook.txt");
								break;
							case 6:
								DequeueByPriority();
								savePhoneBook("PhoneBook.txt");
								break;
							case 7:
								printf("Exiting the program.\n");
								break;
							default:
								system("cls");
								printf("Please enter a valid option\n");
							}

						} while (choice2 != 7);
						break;
						//system("cls");
					case 3:
						printf("Exiting\n");
						break;
					default:
						system("cls");
						printf("Please enter a valid option\n");
					}

				} while (choice != 3);
				system("cls");

			}

			break;
			/**************************************************************************************************************************

			case 7 for updating settings


			**************************************************************************************************************************/


		case 7:



			system("cls");
			do{
				
			//printf("Option 7 coming soon.\n");
			printf("\n\n(ON: 1 || OFF: 0)\n\n");
			wprintf(L"1. Current COM port: %s\n", settings.comPort);
			printf("2. Current audio message length: %d seconds\n", settings.audioMessageLength);
			printf("3. Current bit rate: %d\n", settings.audioBitRate);
			printf("4. Current baud rate: %d\n", settings.baudRate);
			printf("5. XOR encryption on: %d\n", settings.encryption);
			printf("6. Huffman compression on: %d\n", settings.compression);
			printf("7. Send header with message on: %d\n", settings.header);
			printf("8. Priority Level (1-7): %d\n", settings.priority);
			printf("9. Sender identification: %d\n", settings.sid);
			printf("10. Reciver identification: %d\n", settings.rid);
			printf("11. Header error detection and correction:%d\n", settings.headerError);
			printf("12. Payload error detction:%d\n", settings.payloadError);
			printf("13. Exit");


			// Prompt the user to change settings
			printf("\nWhat settings would you like to change: ");

			scanf_s("%d", &ChangeSettings);
			//int 
			switch (ChangeSettings) {
				
			case 1:
				printf("What COMM port would you like to use (1-9): ");
				int comPortNumber;

				//comport update code
				if (wscanf_s(L"%d", &comPortNumber) != 1 || comPortNumber < 1 || comPortNumber > 9) {
					wprintf(L"Invalid input. Please enter a number between 1 and 9.\n");

				}
				else {
					swprintf_s(settings.comPort, sizeof(settings.comPort) / sizeof(settings.comPort[0]), L"COM%d", comPortNumber);
					// swprintf_s(settings.comPort, sizeof(settings.comPort) / sizeof(settings.comPort[0]), L"COM%d", comPortNumber);
				}
				break;
				//strcpy(settings.comPort, COMPORT);
			case 2:
				//time duration userinput code
				printf("Enter the audio duration in seconds: ");
				scanf_s("%d", &settings.audioMessageLength);
				break;
				//time duration userinput code

			case 3:
				//code to get sample rate from user
				//this can cause issues if user enters too low or two high
				printf("Enter the sample rate in kHz (1-20): ");
				scanf_s("%d", &settings.audioBitRate);

				settings.audioBitRate *= 1000;
				break;

			case 4:
				printf("Enter the transmission baud rate: ");
				scanf_s("%d", &settings.baudRate);
				break;


			case 5:
				printf("Do you want to use XOR encyption: ");
				scanf_s("%d", &settings.encryption);
				break;
			case 6:
				printf("Do you want to use Huffman compression: ");
				scanf_s("%d", &settings.compression);
				break;

			case 7:
				printf("Do you want to send your message with a header: ");
				scanf_s("%d", &settings.header);
				break;
	
			case 8:

				printf("What priority message are you sending (1-7)? \n");
				scanf_s("%d", &settings.priority);
				break;
			
			case 9:

				printf("What is your SID?\n");
				scanf_s("%d", &settings.sid);

				break;

			case 10:

				printf("What is your RID?\n");
				scanf_s("%d", &settings.rid);


				break;

			case 11:

				printf("Do you want error detection and correction on for the header?\n ");
				scanf_s("%d", &settings.headerError);

				break;

			case 12:

				printf("Do you want error detection for the message?\n");
				scanf_s("%d", &settings.payloadError);
				break;

			case 13:
				break;

			default:
				system("cls");
				printf("Please enter a valid option\n");
			}

			} while (ChangeSettings != 13);
			system("cls");

			writeSettingsToFile(&settings, "settings.txt");
			

			break;



			/**************************************************************************************************************************

			case 8 for diagnostic functionallity


			**************************************************************************************************************************/



		case 8:
			system("cls");
			do {
				//printf("Option 7 coming soon.\n");
				printf("\n\nDiagnostic Testing\n\n");
				printf("1. Compression/Decompression\n");
				printf("2. Encryption/Dencryption\n");
				printf("3. Generate a custom message\n");
				printf("4. Queues\n");
				printf("5. Test Baudrates\n");
				printf("6. Exit\n");

				char testSecretKey[10] = "314159265";
				char* message = new char[40000]; //used in finding and optimizing baudrate
				

				scanf_s("%d", &ChangeSettings);
				//int 
				switch (ChangeSettings) {
					


				case 1: //compression Testing

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

					break;




				case 2:

					char encryptionTesting[25];	
					char encryptionMsgOut[25];
					char dencryptionMsgOut[25];
					char ecryptionMessageBuffer[25];

					printf("Enter the text message to decrypt: ");
					scanf("%s", encryptionTesting);
					encryptionTesting[sizeof(encryptionTesting) - 1] = '\0';


				

					xorCipher(encryptionTesting, strlen(encryptionTesting), testSecretKey, 10, encryptionMsgOut);
					printf("Encrypted message in hex:");                               // Will not print as a string so print in HEX, one byte at a time
					for (int i = 0; i < strlen(encryptionTesting); i++) {
						printf("%02x", encryptionMsgOut[i]);
					}
		
					

					xorCipher(encryptionMsgOut, strlen(encryptionMsgOut), testSecretKey, 10, dencryptionMsgOut);
					printf("\nDecrypted message: %s\n", dencryptionMsgOut);

					break;

				case 3:

					numberOfQuotes = fnumQuotes();
					quoteIndices = fquoteIndices(numberOfQuotes);
					quoteLengths = fquoteLength(numberOfQuotes, quoteIndices);
					randomNum = frandNum(0, numberOfQuotes - 1);
					char messageBuffer[MAX_QUOTE_LENGTH];
					bytesRead = GetMessageFromFile(messageBuffer, MAX_QUOTE_LENGTH, randomNum, numberOfQuotes, quoteIndices, quoteLengths);

					// Print the random message
					printf("\nRandom Message:\n%s\n", messageBuffer);

					break;

				case 4:

					InitQueue();
				
					int numMessages;
					printf("How many structures would you like to queue? ");
					scanf("%d", &numMessages);
					getchar();

					for (int i = 0; i < numMessages; ++i) {
						link newNode = (link)malloc(sizeof(Node)); // Allocate memory for a new node

						if (newNode == NULL) {
							printf("Memory allocation failed. Exiting...\n");
						
						}

						Item newItem;

						printf("Enter message: ");
						fgets(newItem.message, ilength, stdin);
						newItem.message[strcspn(newItem.message, "\n")] = '\0'; 

						printf("Enter Sender ID: ");
						scanf("%hd", &newItem.sid);
						getchar(); // Clears the newline character from the input buffer

						printf("Enter Receiver ID: ");
						scanf("%hd", &newItem.rid);
						getchar(); // Clears the newline character from the input buffer

						printf("Enter Priority: ");
						scanf(" %c", &newItem.priority);
						getchar(); // Clears the newline character from the input buffer


						newItem.seqNum = i + 1; // Setting sequence number

						// Add the item to the queue
						newNode->Data = newItem;
						newNode->pNext = NULL;
						AddToQueue(newNode);
					}
						PrintQueueContents();

					break;
				case 5:
					//adds alphabet to 40000 bits
					for (int i = 0; i < 40000; ++i) {
						message[i] = 'A' + (i % 26);
					}

					int operation;

					// Prompt user for operation
					printf("1. Transmit \n2. Receive\nEnter Choice: ");
					scanf("%d", &operation);

					for (int baudRate = 9600; baudRate <= 115200; baudRate *= 2) {
						// Move handle creation and initialization inside the loop
						HANDLE hCom;
						initializePort(settings.comPort);
						setComRate(baudRate);

						if (operation == 1) {
							//build the header for the message based on users predefined settings and for text message transmission
							HeaderForPayload header;
							header.sid = settings.sid;
							header.rid = settings.rid;
							header.priority = settings.priority;
							header.payLoadType = 0; //set as 0 for text
							header.encryption = settings.encryption;
							header.compression = settings.compression;

							//set the msgSize depending on number of chars user entered.
							int msgSize = strlen(message);

							header.payloadSize = 40000;
							transmitPayload(&header, (void*)message, 0);
							printf("Transmit | Baud Rate: %d bps\n", baudRate);
						

						}
						else if (operation == 2) {

							HeaderForPayload recivedHeader;
							void* receivedPayload;
							DWORD bytesRead;
							int messageLength;
							char messageBuffer[40000];

					
								bytesRead = receivePayload(&recivedHeader, &receivedPayload, settings.headerError);

								if (bytesRead == recivedHeader.payloadSize) {
									// Cast the received payload back to a character array
									char* receivedExample = (char*)(receivedPayload);
									receivedExample[recivedHeader.payloadSize - 1] = '\0';
									strcpy(messageBuffer, receivedExample);

								}
								int correctBits = 0;
								for (int i = 0; i < 40000; ++i) {
									if (message[i] == messageBuffer[i]) {
										correctBits++;
									}
								}

								double accuracy = (double)(correctBits) / 40000 * 100.0;
								printf("Receive  | Baud Rate: %6d bps | Accuracy: %f%\n", baudRate, accuracy);


						}
						else {
							printf("Invalid operation. Please enter 1 for Transmission or 2 for Reception.\n");
							break;
						}

					}


					break;
				case 6:
					break;

				default:
					system("cls");
					printf("Please enter a valid option\n");
			}

			} while (ChangeSettings != 6);
			system("cls");

			break;


		//Close output window
		case 9:

			exit(0);

			break;
		}


	} while (option != -1);

		
		return 0;

}

