/*	Filename: main.cpp
Author: Michael Galle
Date: Updated 2022
Details: Testing mainline for Windows sound API
*/

#pragma warning	(disable:4996)

#include "sound.h"
#include <stdio.h>
#include <windows.h>

// Function to record audio and store it in a buffer
void RecordAudio(short* buffer, long bufferSize);

// Function to play audio from a buffer
void PlayAudio(short* buffer, long bufferSize);

// Function to save audio data to a file
void SaveAudio(short* buffer, long bufferSize, const char* filePath);

// Function to load audio data from a file into a buffer
void LoadAudio(short* buffer, long bufferSize, const char* filePath);

int	main(int argc, char* argv[])
{
<<<<<<< HEAD
    extern short iBigBuf[];     // buffer
    extern long lBigBufSize;    // total number of samples

    char filePath[150];
    char save;
    char replay;
    char c;                     // used to flush extra input
=======
	extern short iBigBuf[];													// buffer
	extern long  lBigBufSize;												// total number of samples
	short* iBigBufNew = (short*)malloc(lBigBufSize * sizeof(short));		// buffer used for reading recorded sound from file

	char filePath[150];
	char save;
	char replay;
	char c;																	// used to flush extra input
	FILE* f;
>>>>>>> 030d41e833e20406aa51615fed677528656581e5

    // initialize playback and recording
    InitializePlayback();
    InitializeRecording();

    // Record audio
    RecordAudio(iBigBuf, lBigBufSize);

    // playback recording
    printf("\nPlaying recording from buffer\n");
    PlayAudio(iBigBuf, lBigBufSize);

    // Save audio recording
    printf("Would you like to save your audio recording? (y/n): ");
    scanf_s("%c", &save);
    while ((c = getchar()) != '\n' && c != EOF) {}

    if (save == 'y' || save == 'Y') {
        printf("Where would you like to save your audio recording?: ");
        fgets(filePath, sizeof(filePath), stdin);

        int n = strlen(filePath);
        filePath[n - 1] = '\0';
        strcat_s(filePath, ".dat");

        SaveAudio(iBigBuf, lBigBufSize, filePath);
    }

    // Replay audio recording from file
    printf("Would you like to replay the saved audio recording from the file? (y/n): ");
    scanf_s("%c", &replay, 1);
    while ((c = getchar()) != '\n' && c != EOF) {}

    if (replay == 'y' || replay == 'Y') {
        printf("Enter the path to the saved audio file: ");
        fgets(filePath, sizeof(filePath), stdin);

        int n = strlen(filePath);
        filePath[n - 1] = '\0';

        LoadAudio(iBigBuf, lBigBufSize, filePath);

        printf("\nPlaying recording from saved file ...\n");
        PlayAudio(iBigBuf, lBigBufSize);
    }

    printf("\n");
    system("pause");
    return 0;
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


