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
//include "Huffmain.h"
		
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
} ComSettings;

ComSettings settings;

// Function to write settings to a file
void writeSettingsToFile(const ComSettings* settings, const char* filename) {
	FILE* file = fopen(filename, "w");

	if (file != NULL) {
		fwprintf(file, L"COMPORT=%s\n", settings->comPort);
		fwprintf(file, L"BITRATE=%d\n", settings->baudRate);
		fwprintf(file, L"AUDIOMESSAGELENGTH=%d\n", settings->audioMessageLength);
		fwprintf(file, L"AUDIOBITRATE=%d\n", settings->audioBitRate);
		fwprintf(file, L"ENCRYPTION=%d\n", settings->encryption);
		fwprintf(file, L"COMPRESSION=%d\n", settings->compression);
		fwprintf(file, L"HEADER=%d\n", settings->header);
		fclose(file);
	}
	else {
		printf("Write settings function failed");
	}
}

// Function to read settings from a file
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
		}

		fclose(file);
	}
	else {
		printf("Write settings function failed");
	}
}


int	main(int argc, char* argv[]) {

	char transmit = 'T';
	char receive = 'R';
	char filePath[150];
	char save;
	char replay;
	char c;                     // used to flush extra input
	int n;                       // Declare 'n' here
	int randomNum;
	int numberOfQuotes;
	long int* quoteIndices;
	int* quoteLengths;
	int bytesRead = 0;
	int option;
	int ChangeSettings = -1;

	srand((unsigned int)time(NULL));


	// Read settings from file
	readSettingsFromFile(&settings, "settings.txt");

	//menu loop
	do {

		//prints main menu options and request input
		printf("\nOptions:\n");
		printf("1. Record audio into buffer\n");
		printf("2. Play audio in buffer\n");
		printf("3. Load audio from file into buffer\n");
		printf("4. Save audio in buffer\n");
		printf("5. Transmit audio in buffer\n");
		printf("6. Transmit text message in buffer\n");
		printf("7. Settings\n");
		printf("Enter your choice: ");
		scanf_s("%d", &option);

		//used to flush the input buffer when user incorectly enters input
		while ((c = getchar()) != '\n' && c != EOF) {}


		//switch case for various user inputs
		switch (option) {

		case 1:	// Recording audio case

			//calls helper function
			RecordAudioTB(iBigBuf, lBigBufSize, settings.audioMessageLength, settings.audioBitRate * 1000);
			break;

		case 2:

			//calls helper to play audio
			printf("\nPlaying recording from buffer\n");
			PlayAudio(iBigBuf, lBigBufSize);
			break;

		case 3:
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

		case 4:

			//file loading code
			printf("Enter the path to save the audio file to load: ");
			fgets(filePath, sizeof(filePath), stdin);

			//remove line end char
			n = strlen(filePath);  // Assign to 'n' (declared before the switch)
			filePath[n - 1] = '\0';

			//move audio to buffer
			SaveAudio(iBigBuf, lBigBufSize, filePath);
			printf("\Saved audio to file ...\n");
			break;

		case 5:

			//request transmission or reciving
			printf("Options:\n");
			printf("1. Transmit\n");
			printf("2. Receive\n");

			printf("Enter your choice (1, or 2): ");
			scanf_s(" %c", &option, 1);


			setComRate(settings.baudRate);
			initializePort(settings.comPort);
			
			if (settings.compression == 1 && option == '1') {

				short* tmpBuf[40000];
				long lengthBuf = 40000;
				encodeShorts(iBigBuf, lBigBufSize, tmpBuf, &lengthBuf);
				//*iBigBuf = *tmpBuf;
				//copy temp buf to ibigbuf
				for (int i = 0; i < lBigBufSize; ++i) {
					iBigBuf[i] = (*tmpBuf)[i];
				}
				lBigBufSize = lengthBuf;


			}


			//transmitting without header
			if (option == '1' && settings.header == 0) {
				transmitAudio(iBigBuf, lBigBufSize);
			}

			//reciving without a header
			else if (option == '2' && settings.header == 0) {
				receiveAudio(iBigBuf, lBigBufSize); // Pass the buffer to store the received audio
			}

			//transmitt with header
			else if (option == '2' && settings.header == 1) {
				//char Payload[] = "\nHi there this is a great message for you\n"; 	// Payload is a text message in this example but could be any data	

				// Header (sample data type is text but this should work with audio and images as well)
				//HeaderForPayload.sid = 1;
				//HeaderForPayload.rid = 2;
				//HeaderForPayload.payloadSize = strlen(txPayload) + 1;				// Flexible payload size - Send size of payload inside header (payload can be anything) and enough memory will be malloc'd in the receive function
				//HeaderForPayload.compression = 0;									// None
				//HeaderForPayload.encryption = 0;									// None
				//HeaderForPayload.payLoadType = 0;									// Text

			//	transmitPayload(&HeaderForPayload, iBigBuf, &hComTx, settings.comPort, settings.baudRate, nComBits, timeout);  // Transmit header and payload


			}

			//recive with header
			else if (option == '2' && settings.header == 1) {
				/*****************************************************************************************************************************

					bytesRead = receive(&rxHeader, &HeaderForPayload, &hComRx, COMPORT_Rx, nComRate, nComBits, timeout);		// Pass pointer to rxPayload so can access malloc'd memory inside the receive function from main()

					// Use header info to determine if payload needs to be decrypted or decompressed
					if (HeaderForPayload.encryption != 0) {
						printf("\nMessage is encrypted so need to decrypt!\n");
						// rxPayload = decrypt(rxPayload)
					}
					else {
						printf("\nMessage is not encrypted\n");
					}
					if (HeaderForPayload.compression != 0) {
						printf("\nMessage is compressed so need to decompress!\n");
						// rxPayload = decompress(rxPayload)
					}
					else {
						printf("\nMessage is not compressed\n");
					}

					// Determine type of payload from header data and corresponding action to complete (e.g. display text, play audio, etc)
					if (HeaderForPayload.payLoadType == 0) {
						printf("\nPayload is text\n");
						printf("\nMessage recieved: %s\n", (char*)rxPayload);		// May need to set rxPayload[bytesRead - 1] = '\0'
						// Enqueue()
						free(HeaderForPayload);											// malloc'd in the receive function
					}
					else if (HeaderForPayload.payLoadType == 1) {
						printf("\nPayload is audio\n");
						// Playbuffer();
						// Enqueue
						free(rxPayload);											// malloc'd in the receive function
					}
					else {
						printf("\Payload is an image or something else ...\n");
						// DisplayImage();
						free(rxPayload);											// malloc'd in the receive function
					}
*/
				}

				else {
					printf("Invalid input. Please enter 1 or 2.\n");
				}

				if (settings.compression == 1 && option == '2') {


					//decodeShorts(compressedData, compressedSize, &decompressedData, &decompressedSize);
					short* tmpBuf[40000];
					long lengthBuf = 40000;
					decodeShorts(iBigBuf, lBigBufSize, tmpBuf, &lengthBuf);
					
					for (int i = 0; i < lBigBufSize; ++i) {
						iBigBuf[i] = (*tmpBuf)[i];
					}
					lBigBufSize = lengthBuf;


				}

				break;
				

		case 6:

			char userResultTwo;

			printf("Options:\n");
			printf("1. Transmit\n");
			printf("2. Receive\n");
			printf("Enter your choice (1 or 2): ");
			scanf_s(" %c", &userResultTwo, 1);

			if (userResultTwo == '1') {

				// User input for text message
				char userResultThree;
				printf("Options:\n");
				printf("1. Custom Message\n");
				printf("2. Generate Message\n");
				printf("Enter your choice (1 or 2): ");
				scanf_s(" %c", &userResultThree, 1);

				char msgOut[250];

				if (userResultThree == '1') {

					printf("Enter the text message to transmit: ");
					scanf(" %[^\n]s", msgOut);
				}
				else if (userResultThree == '2') {

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

				//encrypt transmitted message
				//XOR encode funtion
				//args in the following order to encrypt (message, messageLen, secretKey, secretKeyLen, encBuf

				if (settings.compression == 1) {

					void encodeFile(const char* inputFileName, const char* outputFileName);
				}

				if (settings.encryption == 1) {

					char secretKey[10] = "314159265";
					int keyLength = 10;
					char tempBuf[250];

					xorCipher(msgOut, strlen(msgOut), secretKey, keyLength, tempBuf);
				}

				initializePort(settings.comPort);
				// Transmit text message
				transmitMessage(msgOut);




				// RECIEVING TEXT MESSAGES
			}
			else if (userResultTwo == '2') {


				initializePort(settings.comPort);
				int messageLength;
				char messageBuffer[250];
				receiveMessages(messageBuffer, &messageLength);


				printf("\nRecived message: %s\n", messageBuffer);

				if (settings.compression == 1) {

					//void decodeFile(const char* inputFileName, const char* outputFileName);
				}

				if (settings.encryption == 1) {

					char secretKey[10] = "314159265";
					int keyLength = 10;
					char tempBuf[250];

					xorCipher(messageBuffer, strlen(messageBuffer), secretKey, keyLength, tempBuf);

					printf("\nXOR Decrypted Message: %s\n", messageBuffer);
				}



				// Receive text message

			}
			else {
				printf("Invalid input. Please enter 1 or 2.\n");
			}

			break;

		case 7:
			system("cls");
			do{
			//printf("Option 7 coming soon.\n");
			wprintf(L"1. Current COM port: %s\n", settings.comPort);
			printf("2. Current audio message length: %d seconds\n", settings.audioMessageLength);
			printf("3. Current bit rate: %d\n", settings.audioBitRate);
			printf("4. Current baud rate: %d\n", settings.baudRate);
			printf("5. XOR encryption on (YES: 1 || NO: 0): %d\n", settings.encryption);
			printf("6. Huffman compression on (YES: 1 || NO: 0): %d\n", settings.compression);
			printf("7. Send header with message on (YES: 1 || NO: 0): %d\n", settings.header);
			printf("8. Exit");


			// Prompt the user to change settings
			printf("\nWhat settings would you like to change: ");

			scanf("%d", &ChangeSettings);
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
				break;

			case 4:
				printf("Enter the transmission baud rate: ");
				scanf_s("%d", &settings.baudRate);
				break;


			case 5:
				printf("Do you want to use XOR encyption (YES: 1 || NO: 0): ");
				scanf_s("%d", &settings.encryption);
				break;
			case 6:
				printf("Do you want to use Huffman compression (YES: 1 || NO: 0): ");
				scanf_s("%d", &settings.compression);
				break;


			case 7:
				printf("Do you want to send your message with a header (YES: 1 || NO: 0): ");
				scanf_s("%d", &settings.header);
				break;

			default:
				printf("Error changing settings\n");
				break;
			}
			system("cls");
			} while (ChangeSettings != 8);

			writeSettingsToFile(&settings, "settings.txt");

			// Display updated settings
			printf("\nUpdated settings:\n");
			wprintf(L"COM port: %s\n", settings.comPort);
			printf("baud rate: %d\n", settings.baudRate);
			printf("Audio message length: %d seconds\n", settings.audioMessageLength);
			printf("Audio bit rate: %d kbps\n", settings.audioBitRate);
			printf("XOR encyption on (YES: 1 || NO: 0): %d\n", settings.encryption);
			printf("Huffman compression (YES: 1 || NO: 0): %d\n", settings.compression);
			printf("Header with message on (YES: 1 || NO : 0): %d\n", settings.header);

			break;


		default:

			//system("cls");
			printf("Invalid option. Please choose a valid option.\n");
			break;
			}
		} while (option != -1);

	
		return 0;

}



