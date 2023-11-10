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
extern short iBigBuf[];  // Declare the external variable// buffer
extern long lBigBufSize;  // Declare the external variable // total number of samples


int	main(int argc, char* argv[])
{

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
        printf("4. Save Audio in buffer\n");
        printf("5. Transmit Audio in Buffer\n");
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

            //this code needs to be validated.
            printf("Memory address of big buffaroni: %p\n", (void*)iBigBuf);

            transmit(iBigBuf, lBigBufSize);

            break;

        default:

            printf("Invalid option. Please choose a valid option.\n");
        }

    } while (option != -1);

    return 0;
}



