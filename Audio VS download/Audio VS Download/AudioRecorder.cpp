/*
Author: Jesse Russell, Hayden Foster, Zach McEwin
Date: 1 Nov 2023
Details: Contains various audio file functions such as , play, save, load, record,
*/

#pragma warning	(disable:4996)

#include "sound.h"
#include <stdio.h>
//#include <windows.h>


// Function to record audio and store it in a buffer
void RecordAudio(short* buffer, long bufferSize);
void PlayAudio(short* buffer, long bufferSize);
void SaveAudio(short* buffer, long bufferSize, const char* filePath);
void LoadAudio(short* buffer, long bufferSize, const char* filePath);
void RecordAudioTB(short* buffer, long bufferSize, int durationInSeconds, int sampleRate);
void RecordBufferTB(short* buffer, long bufferSize, int durationInSeconds, int sampleRate);





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
    // Clear the buffer
    memset(buffer, 0, sizeof(short) * bufferSize);
    printf("Recorded audio is %d\n", sizeof(short) * bufferSize);
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


