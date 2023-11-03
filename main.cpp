/*	Filename: main.cpp
Author: Michael Galle
Date: Updated 2022
Details: Testing mainline for Windows sound API
*/

#pragma warning	(disable:4996)

#include "sound.h"
#include <stdio.h>
#include <windows.h>
//#include "AudioRecorder.h"


//protoypes
// Function to record audio and store it in a buffer
void RecordAudio(short* buffer, long bufferSize);
void PlayAudio(short* buffer, long bufferSize);
void SaveAudio(short* buffer, long bufferSize, const char* filePath);
void LoadAudio(short* buffer, long bufferSize, const char* filePath);
void RecordAudioTB(short* buffer, long bufferSize, int durationInSeconds, int sampleRate);
void RecordBufferTB(short* buffer, long bufferSize, int durationInSeconds, int sampleRate);

int	main(int argc, char* argv[])
{
    extern short iBigBuf[];     // buffer
    extern long lBigBufSize;    // total number of samples

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
        printf("\nOptions:\n");
        printf("1. Record audio\n");
        printf("2. Play audio\n");
        printf("3. Load audio from file\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf_s("%d", &option);


        while ((c = getchar()) != '\n' && c != EOF) {}

        switch (option) {
        case 1:
        {
            //time duration userinput code
            printf("Enter the  duration \n");
            int recordingDuration;
            scanf_s("%d", &recordingDuration);

            //code to get sample rate from user
            printf("Enter the sample rate in kHz (1-20): ");
            int sampleRate;
            scanf_s("%d", &sampleRate);


            //calls helper function
            RecordAudioTB(iBigBuf, lBigBufSize, recordingDuration, sampleRate * 1000);
            break;


        }
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
            printf("\nExiting...\n");
            printf("\n");
            system("pause");
            break;


        default:
            printf("Invalid option. Please choose a valid option.\n");
        }


    } while (option != 4);


    return 0;
}


// Function to record audio and store it in a buffer with a specified duration (in seconds) and sample rate
void RecordAudioTB(short* buffer, long bufferSize, int durationInSeconds, int sampleRate) {
    InitializeRecording();
    RecordBufferTB(buffer, bufferSize, durationInSeconds, sampleRate);
    CloseRecording();
}


//Do we need to ensure this code is not recording really long audio?
// Function to record audio with a specified duration (in seconds) and sample rate
void RecordBufferTB(short* buffer, long bufferSize, int durationInSeconds, int sampleRate) {
    // Calculate the number of samples needed for the specified duration
    long requiredSamples = durationInSeconds * sampleRate;
    RecordBuffer(buffer, requiredSamples);
}




// Function to record audio and store it in a buffer
void RecordAudio(short* buffer, long bufferSize) {
    InitializeRecording();
    RecordBuffer(buffer, bufferSize);
    CloseRecording();
}

// Function to play audio from a buffer
void PlayAudio(short* buffer, long bufferSize) {
    InitializePlayback();
    PlayBuffer(buffer, bufferSize);
    ClosePlayback();
}

// Function to save audio data to a file
void SaveAudio(short* buffer, long bufferSize, const char* filePath) {
    FILE* f = fopen(filePath, "wb");
    if (!f) {
        printf("Unable to open %s\n", filePath);
        return;
    }

    fwrite(buffer, sizeof(short), bufferSize, f);
    fclose(f);
}

// Function to load audio data from a file into a buffer
void LoadAudio(short* buffer, long bufferSize, const char* filePath) {
    FILE* f = fopen(filePath, "rb");
    if (!f) {
        printf("Unable to open %s\n", filePath);
        return;
    }

    fread(buffer, sizeof(short), bufferSize, f);
    fclose(f);
}


