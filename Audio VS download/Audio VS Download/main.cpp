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
					
//these are buffers for storing the audio message
//extern short iBigBuf[];  // Declare the external variable// buffer
//extern long lBigBufSize;  // Declare the external variable // total number of samples



//these need a way to update!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// A commtimeout struct variable
//wchar_t COMPORT_Rx[] = L"COM8";
//wchar_t COMPORT_Tx[] = L"COM9";
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

	int randomNum;
	int numberOfQuotes;
	long int* quoteIndices;
	int* quoteLengths;
	int bytesRead = 0;

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
		printf("7. Random\n");
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

				
                printf("What COMM port would you like to use (1-9): ");
                int comPortNumber;
                scanf_s("%d", &comPortNumber);

                // Check if the COM port number is within the valid range
              if (comPortNumber >= 1 && comPortNumber <= 9) {
                wchar_t Port[20];
                swprintf(Port, L"COM%d", comPortNumber);

               
               }

				//once user selects Physical com ports it stays in this section of code
				//while (1) {
				printf("Options:\n");
				printf("1. Transmit\n");
				printf("2. Receive\n");

				printf("Enter your choice (1, or 2): ");
				scanf_s(" %c", &option, 1);

				if (option == '1') {

					// This code does not correctly update the comm port based on user input, i commentd it out but this needs to be figure out
					printf("Enter the COM port: ");
					wchar_t txPort[20];
					scanf_s("%s", txPort, 20);
					
			
	

					printf("Enter the bit rate: ");
					int rate;
					scanf("%d", &rate);
					setComRate(rate);

					initializeTxPort(COMPORT_Tx);

					//const short msgConverted = msgOut[];
					transmitAudio(iBigBuf, lBigBufSize);

				}


				else if (option == '2') {

					wchar_t rxPort[20];
					
					printf("Enter the COM port: ");


					scanf_s("%s", rxPort, 20);

					printf("Enter the number of bits: ");
					int bits;
					scanf("%d", &bits);
					setComBits(bits);
					

					printf("Enter the bit rate: ");
					int rate;
					scanf("%d", &rate);
					setComRate(rate);

					initializeRxPort(COMPORT_Rx);
					//short receivedAudio[AUDIO_BUFFER_SIZE]; // Define a buffer to store received audio
					receiveAudio(iBigBuf, lBigBufSize); // Pass the buffer to store the received audio

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

				   }	else if (userResultTwo == '2') {
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
			
			   break;
		   case 7:

			   numberOfQuotes = fnumQuotes();
			   quoteIndices = fquoteIndices(numberOfQuotes);
			   quoteLengths = fquoteLength(numberOfQuotes, quoteIndices);
			   randomNum = frandNum(0, numberOfQuotes - 1);
			   char messageBuffer[MAX_QUOTE_LENGTH];
			   bytesRead = GetMessageFromFile(messageBuffer, MAX_QUOTE_LENGTH, randomNum, numberOfQuotes, quoteIndices, quoteLengths);

			   // Print the random message
			   printf("\nRandom Message:\n%s\n", messageBuffer);
			   free(quoteIndices);
			   free(quoteLengths);

			   break;

        default:
         //   system("cls");
          //  printf("Invalid option. Please choose a valid option.\n");
        }

    } while (option != -1);

    return 0;
}
/*
  srand(time(NULL));

			   // Get the number of quotes in the file
			   int numQuotes = fnumQuotes();

			   // Generate a random number between 0 and numQuotes-1
			   int randNum = frandNum(0, numQuotes - 1);
			   // Allocate memory for quote indices and lengths
			   long int* quoteIndices = fquoteIndices(numQuotes);
			   int* quoteLengths = fquoteLength(numQuotes, quoteIndices);

			   // Allocate memory for the buffer to store the message
			   char buffer[MAX_QUOTE_LENGTH];

			   // Get the random message from the file
			   GetMessageFromFile(buffer, MAX_QUOTE_LENGTH, randNum, numQuotes, quoteIndices, quoteLengths);

			   // Transmit the random message
			   transmitMessage(buffer);

			   // Free allocated memory
			   free(quoteIndices);
			   free(quoteLengths);
			   */