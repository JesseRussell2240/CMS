/*	Filename: main.cpp
Author: Michael Galle
Date: Updated 2022
Details: Testing mainline for Windows sound API
*/

#pragma warning	(disable:4996)

#include "sound.h"
#include <stdio.h>
#include <windows.h>

int	main(int argc, char* argv[])
{
	extern short iBigBuf[];												// buffer
	extern long  lBigBufSize;											// total number of samples
	short* iBigBufNew = (short*)malloc(lBigBufSize * sizeof(short));		// buffer used for reading recorded sound from file

	char filePath[150];
	char save;
	char replay;
	char c;																// used to flush extra input
	FILE* f;

	// initialize playback and recording
	InitializePlayback();
	InitializeRecording();

	// start recording
	RecordBuffer(iBigBuf, lBigBufSize);
	CloseRecording();

	// playback recording 
	printf("\nPlaying recording from buffer\n");
	PlayBuffer(iBigBuf, lBigBufSize);
	ClosePlayback();

	// Save audio recording
	printf("Would you like to save your audio recording? (y/n): ");
	scanf_s("%c", &save);
	while ((c = getchar()) != '\n' && c != EOF) {} //waits till user entered enter	

	if (save == 'y' || save == 'Y') {
		printf("Where would you like to save your audio recording?: ");
		fgets(filePath, sizeof(filePath), stdin);

		//sets an int = to the length of the string 
		int n = strlen(filePath);
		//delete the terminator \n and replace it with \0
		filePath[n - 1] = '\0';
		strcat_s(filePath, ".dat");

		/* Open input file in "ab" mode (append or create if it doesn't exist) */
		f = fopen(filePath, "ab");
		if (!f) {
			printf("Unable to open %s\n", filePath);
			return 1; // Return an error code to indicate failure
		}

		printf("Writing to sound file ...\n");
		// You can write your audio recording to the 'f' file pointer here.
		fwrite(iBigBuf, sizeof(short), lBigBufSize, f);
		fclose(f);
	}

	// replay audio recording from file -- read and store in buffer, then use playback() to play it
	printf("Would you like to replay the saved audio recording from the file? (y/n): ");
	scanf_s("%c", &replay, 1);
	while ((c = getchar()) != '\n' && c != EOF) { //waits till user entered enter							// Flush other input
		if ((replay == 'y') || (replay == 'Y')) {
			/* Open input file */
			fopen_s(&f, filePath, "rb");
			if (!f) {
				printf("Unable to open %s\n", filePath);
				return 0;
			}
			printf("Reading from sound file ...\n");

			fread(iBigBufNew, sizeof(short), lBigBufSize, f); // Read audio data from the file into the buffer iBigBufNew			// Record to new buffer iBigBufNew
			fclose(f); // Close the file
			InitializePlayback(); // Initialize the audio playback

			printf("\nPlaying recording from saved file ...\n");

			PlayBuffer(iBigBufNew, lBigBufSize); // Play the audio from the buffer
			ClosePlayback(); // Close the audio playback
		}

		printf("\n");
		system("pause");
		return(0);
	}
}