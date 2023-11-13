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
					
//these are buffers for storing the audio message
//extern short iBigBuf[];  // Declare the external variable// buffer
//extern long lBigBufSize;  // Declare the external variable // total number of samples



//these need a way to update!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// A commtimeout struct variable
wchar_t COMPORT_Rx[] = L"COM6";
wchar_t COMPORT_Tx[] = L"COM7";


extern short iBigBuf[];								// Declare the external variable
extern long lBigBufSize;							// Declare the external variable


int	main(int argc, char* argv[])
{
    char transmit = 'T';
    char receive = 'R';
    char filePath[150];
    char save;
    char replay;
    char c;                     // used to flush extra input
    int n;                       // Declare 'n' here

    // initialize playback and recording
    InitializePlayback();
    InitializeRecording();

    int option;


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
        printf("Enter your choice: ");
        scanf_s("%d", &option);

        //used to flush the input buffer when user incorectly enters input
        while ((c = getchar()) != '\n' && c != EOF) {}

        //switch case for various user inputs
        switch (option) { // Recording audio case
        case 1:
      
            //time duration userinput code
            printf("Enter the  duration \n");
            int recordingDuration;
            scanf_s("%d", &recordingDuration);

            //code to get sample rate from user
            //this can cause issues if user enters too low or two high
            printf("Enter the sample rate in kHz (1-20): ");
            int sampleRate;
            scanf_s("%d", &sampleRate);

            //calls helper function
            RecordAudioTB(iBigBuf, lBigBufSize, recordingDuration, sampleRate * 1000);
            break;

        case 2:

            //calls helper to play audio
            printf("\nPlaying recording from buffer\n");
            PlayAudio(iBigBuf, lBigBufSize);
            break;

        case 3:

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

            //calls helper function to play audio recording
            printf("\Saved audio to file ...\n");
           
            break;

        case 5:



			/*
						//gets user input for comport setup	
            printf("What COMM port would you like to use (1-9): ");
			int comPortNumber;
            scanf_s("%d", &comPortNumber);
			wchar_t TempPort[20];
			swprintf(TempPort, L"COM%d", comPortNumber);
			wcscpy(COMPORT, TempPort); 
			
			*/
            


				*/



				//once user selects Physical com ports it stays in this section of code
				//while (1) {
				printf("Options:\n");
				printf("1. Transmit\n");
				printf("2. Receive\n");

				printf("Enter your choice (1, or 2): ");
				scanf_s(" %c", &option, 1);

				if (option == '1') {

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
					transmitAudio(iBigBuf, lBigBufSize);

				}


			   /*
			   	   //updated comm port number based on user provided input
			   //var initilization is in case 5 - thiscould be a sub function to reduce redunduncy, but so could the transmit/recive request
			   printf("What COMM port would you like to use (1-9): ");
			   scanf_s("%d", &comPortNumber);
			   swprintf(TempPort, L"COM%d", comPortNumber);
			   wcscpy(COMPORT, TempPort);
			   */
		


				else if (option == '2') {

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
					receiveAudio(iBigBuf, lBigBufSize); // Pass the buffer to store the received audio

				printf("Options:\n");
				printf("1. Transmit\n");
				printf("2. Receive\n");
				printf("Enter your choice (1 or 2): ");
				scanf_s(" %c", &userResultTwo, 1);

				//setBitrate for communication
				printf("Enter the bit rate: ");
				int rate;
				scanf("%d", &rate);
				setComRate(rate);

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
					char secretKey[10] = "314159265";
					int keyLength = 10;
					char tempBuf[250];
					xorCipher(msgOut, strlen(msgOut), secretKey, keyLength, tempBuf);


					initializePort(COMPORT);
					// Transmit text message
					transmitMessage(msgOut);

				}	else if (userResultTwo == '2') {
					// Receive text message

					initializePort(COMPORT);
					int messageLength;
					char messageBuffer[250];
					receiveMessages(messageBuffer, &messageLength);
					char secretKey[10] = "314159265";
					int keyLength = 10;
					char tempBuf[250];

					xorCipher(messageBuffer, strlen(messageBuffer), secretKey, keyLength, tempBuf);

					printf("\nXOR Decrypted Message: %s\n\n\n\n", tempBuf);

					// Receive text message
					

				}
				else {
					printf("Invalid input. Please enter 1 or 2.\n");
				}
				//}

				//system("pause");

			

			//return 0;

            break;

           case 6:
			   char userResultTwo;
			   printf("Select the type of COM port:\n");
			   printf("1. Virtual COM Port\n");
			   printf("2. Physical COM Port\n");
			   printf("Enter your choice (1 or 2): ");
			   int comTypeTwo;
			   scanf("%d", &comTypeTwo);

			   if (comTypeTwo == 1) {
				   // Virtual COM Port selected
				   printf("Using virtual COM port for transmission: %S\n", COMPORT_Tx);
				   printf("Enter the bit rate: ");
				   int rate;
				   scanf("%d", &rate);
				   setComRate(rate);

				   initializeRxPort(COMPORT_Rx);
				   initializeTxPort(COMPORT_Tx);

				   // User input for text message
				   printf("Enter the text message to transmit: ");
				   char msgOut[250];
				   scanf(" %[^\n]s", msgOut);

				   // Transmit text message
				   transmitMessage(msgOut);
				   receiveMessages();
			   }
			   else if (comTypeTwo == 2) {
				   // Physical COM Port selected
				   printf("Options:\n");
				   printf("1. Transmit\n");
				   printf("2. Receive\n");
				   printf("Enter your choice (1 or 2): ");
				   scanf_s(" %c", &userResultTwo, 1);

				   if (userResultTwo == '1') {
					   // Transmit text message
					   printf("Enter the bit rate: ");
					   int rate;
					   scanf("%d", &rate);
					   setComRate(rate);

					   initializeTxPort(COMPORT_Tx);

					   // User input for text message
					   printf("Enter the text message to transmit: ");
					   char msgOut[250];
					   scanf(" %[^\n]s", msgOut);

					   // Transmit text message
					   transmitMessage(msgOut);
				   }
				   else if (userResultTwo == '2') {
					   // Receive text message
					   printf("Enter the bit rate: ");
					   int rate;
					   scanf("%d", &rate);
					   setComRate(rate);

					   initializeRxPort(COMPORT_Rx);

					   // Receive text message
					   receiveMessages();
				   }
				   else {
					   printf("Invalid input. Please enter 1 or 2.\n");
				   }
			   }
			   else {
				   printf("Invalid input. Please enter 1 or 2.\n");
			   }
			   break;

        default:
            system("cls");
            printf("Invalid option. Please choose a valid option.\n");
        }

    } while (option != -1);

    return 0;
}

