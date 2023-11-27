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
	int priority;
	int sid;
	int rid;
} ComSettings;

ComSettings settings;

// Function to write settings to a file
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
		fclose(file);
	}
	else {
		printf("Write settings function failed");
	}
}

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

void ReceiveMessagesInBackground() {
	while (true) {


		////this is eventuially where multithreading of recivie will go. I hate this idea...








		std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
			swscanf(line, L"PRIORITY=%d", &settings->priority);
			swscanf(line, L"SID=%d", &settings->sid);
			swscanf(line, L"RID=%d", &settings->rid);
		}

		fclose(file);
	}
	else {
		printf("Write settings function failed");
	}
}


int	main(int argc, char* argv[]) {

	std::thread receiveThread(ReceiveMessagesInBackground);

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
		printf("8. Exit\n");
		printf("Enter your choice: ");
		scanf_s("%d", &option);

		//used to flush the input buffer when user incorectly enters input
		while ((c = getchar()) != '\n' && c != EOF) {}


		//switch case for various user inputs
		switch (option) {

		case 1:	// Recording audio case

			system("cls");
			//calls helper function
			RecordAudioTB(iBigBuf, lBigBufSize, settings.audioMessageLength, settings.audioBitRate * 1000);
			break;

		case 2:

			system("cls");
			//calls helper to play audio
			printf("\nPlaying recording from buffer\n");
			PlayAudio(iBigBuf, lBigBufSize);
			break;

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

		case 4:

			system("cls");
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

			system("cls");
			//request transmission or reciving
			printf("Options:\n");
			printf("1. Transmit\n");
			printf("2. Receive\n");

			printf("Enter your choice (1, or 2): ");
			scanf("%d", &option, 1);

			
			setComRate(settings.baudRate);
			initializePort(settings.comPort);

			//Transmission
			if (option == 1) {

				if (settings.compression == 1) {

					short* tmpBuf[40000];
					long lengthBuf = 40000;
					encodeShorts(iBigBuf, lBigBufSize, tmpBuf, &lengthBuf);
					//*iBigBuf = *tmpBuf;
					//copy temp buf to ibigbuf
					for (int i = 0; i < lBigBufSize; ++i) {
						iBigBuf[i] = (*tmpBuf)[i];
					}
					lBigBufSize = lengthBuf / 2;
				}

				//transmitting without header
				if (settings.header == 0) {
					transmitAudio(iBigBuf, lBigBufSize);
				}

			}

			//Recieve
			else if (option == 2) {

				if (settings.compression == 1) {


					//decodeShorts(compressedData, compressedSize, &decompressedData, &decompressedSize);
					short* tmpBuf[40000];
					long lengthBuf = 40000;
					decodeShorts(iBigBuf, lBigBufSize, tmpBuf, &lengthBuf);

					for (int i = 0; i < lBigBufSize; ++i) {
						iBigBuf[i] = (*tmpBuf)[i];
					}
					lBigBufSize = lengthBuf;


				}

				if (settings.header == 0) {
				receiveAudio(iBigBuf, lBigBufSize); // Pass the buffer to store the received audio
				}

				//transmitt with header
				else if (settings.header == 1) {
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

				else if (settings.header == 1) {
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


			}

			else {
					printf("Invalid input. Please enter 1 or 2.\n");
			}

			/*
			if (settings.compression == 1 && option == '1') {

				short* tmpBuf[40000];
				long lengthBuf = 40000;
				encodeShorts(iBigBuf, lBigBufSize, tmpBuf, &lengthBuf);
				//*iBigBuf = *tmpBuf;
				//copy temp buf to ibigbuf
				for (int i = 0; i < lBigBufSize; ++i) {
					iBigBuf[i] = (*tmpBuf)[i];
				}
				lBigBufSize = lengthBuf / 2;


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
			break;
				

		case 6:

			InitQueue();
			//isQueuesEmpty();



			system("cls");
			char userResultTwo;

			printf("Options:\n");
			printf("1. Transmit\n");
			printf("2. Receive\n");
			printf("Enter your choice (1 or 2): ");
			scanf_s(" %c", &userResultTwo, 1);

			if (userResultTwo == '1') {

				//build the header for the message based on users predefined settings
				HeaderForPayload header;
				header.sid = settings.sid;
				header.rid = settings.rid;
				header.priority = settings.priority;
				//header.seqNum = 4;
				//header.payloadSize = 1; //must be determined afrer message is constructed and with be lbigbuflength in audio
				header.payLoadType = 0; //set as 0 for text
				header.encryption = settings.encryption;
				header.compression = settings.compression;
				


				// User input for text message
				char userResultThree;
				printf("Options:\n");
				printf("1. Custom Message\n");
				printf("2. Generate Message\n");
				printf("Enter your choice (1 or 2): ");
				scanf_s(" %c", &userResultThree, 1);

				char msgOut[250];
				int msgSize = 250;

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
				else {
					printf("Invalid input. Please enter 1 or 2.\n");
				}

				//encrypt transmitted message
				//XOR encode funtion
				//args in the following order to encrypt (message, messageLen, secretKey, secretKeyLen, encBuf

				if (settings.compression == 1) {

				//	printf("Huffman compressing message );

					char tmpMsg[240];
					strcpy(tmpMsg, msgOut);
					
					msgSize = compressTXT(tmpMsg, msgOut, strlen(msgOut));

					//printf("\ntest\n");
					printf("Length of input message: %d, compressed size: %d\n", strlen(msgOut), msgSize);

					printf("Original buffer Size: %d\n", strlen(msgOut));
					printf("Compressed Size: %d\n", msgSize);

				
				}

				if (settings.encryption == 1) {

					char secretKey[10] = "314159265";
					int keyLength = 10;
					char tempBuf[250];

					xorCipher(msgOut, msgSize, secretKey, keyLength, tempBuf);
				}

				//set the payload size in the header after compression/encription etc are completed.
				header.payloadSize = msgSize;

				printHeaderInfo(header);
				setComRate(settings.baudRate);
				initializePort(settings.comPort);
				
				transmitPayload(&header, (void*)msgOut);



				
				// Transmit text message
				//transmitMessage(msgOut);




				// RECIEVING TEXT MESSAGES
			}
			else if (userResultTwo == '2') {

				setComRate(settings.baudRate);
				initializePort(settings.comPort);

				int messageLength;
				char messageBuffer[250];

			//	void InitQueue(void);
				//int IsQueueEmpty(void);

			//	receiveMessages(messageBuffer, &messageLength);
				DWORD incomingBytes;

				HeaderForPayload recivedHeader;
				void* receivedPayload;


				DWORD bytesRead = receivePayload(&recivedHeader, &receivedPayload);
				
				//printf("MSG recived from %d", recivedHeader.sid);
				printHeaderInfo(recivedHeader);



				if (bytesRead == recivedHeader.payloadSize) {
					// Cast the received payload back to a character array
					char* receivedExample = (char*) (receivedPayload);
					
					
					// Free the allocated memory for the received payload
					free(receivedPayload);
				}
				

				void AddToQueue(link);

						printf("\nRecived message: %s\n", messageBuffer);
						if (settings.compression == 1) {

						int resultLength = 0;
						char tmpMsg[500];
						int decompressedSize = decompressTXT(messageBuffer, tmpMsg, strlen(messageBuffer), resultLength); //was hardcoded so it always returned 250, changed it to resultLength for now idk if that solves it tho

						printf("Decompressed Size: %d\n", decompressedSize);
						//printf("Decompressed Message: %s\n", strlen(messageBuffer));

						strcpy(messageBuffer, tmpMsg);
						printf("\nUncompressed message: %s\n", messageBuffer);
						//void decodeFile(const char* inputFileName, const char* outputFileName);
					}
					if (settings.encryption == 1) {

						char secretKey[10] = "314159265";
						int keyLength = 10;
						char tempBuf[250];

						xorCipher(messageBuffer, strlen(messageBuffer), secretKey, keyLength, tempBuf);

						printf("\nXOR Decrypted Message: %s\n", messageBuffer);
					}

				//		void AddToQueue(link);
				//	link DeQueue(void);

					//	Item receivedMsg;
					//	AddToQueue(receivedMSG);

				//}

						
					
				

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
			printf("8. Priority Level: %d\n", settings.priority);
			printf("9. Sender identification: %d\n", settings.sid);
			printf("10. Reciver identification: %d\n", settings. rid);
			printf("11. Exit");


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
	
			case 8:

				printf("What priority message are you sending (1-7)? \n");
				scanf_s("%d", &settings.priority);
			
			case 9:

				printf("What is your SID?\n");
				scanf_s("%d", &settings.sid);

			case 10:

				printf("What is your RID?\n");
				scanf_s("%d", &settings.sid);


				break;

			default:
				printf("Error changing settings\n");
				break;
			}
			system("cls");
			} while (ChangeSettings != 11);

			writeSettingsToFile(&settings, "settings.txt");

			/* Display updated settings
			printf("\nUpdated settings:\n");
			wprintf(L"COM port: %s\n", settings.comPort);
			printf("baud rate: %d\n", settings.baudRate);
			printf("Audio message length: %d seconds\n", settings.audioMessageLength);
			printf("Audio bit rate: %d kbps\n", settings.audioBitRate);
			printf("XOR encyption on (YES: 1 || NO: 0): %d\n", settings.encryption);
			printf("Huffman compression (YES: 1 || NO: 0): %d\n", settings.compression);
			printf("Header with message on (YES: 1 || NO : 0): %d\n", settings.header);
			*/
			break;

		default:

			system("cls");
			printf("Invalid option. Please choose a valid option.\n");
			break;


		//Close output window
		case 8:

			exit(0);

			break;
		}

	} while (option != -1);

		
		return 0;

}



