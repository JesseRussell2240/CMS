#include "sound.h"
#include <stdio.h>
#include "AudioHandling.h"
#include <QFileDialog>
#include <QString>

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
    // Open a file dialog for the user to select a file
    QString initialfilePath = QFileDialog::getOpenFileName(nullptr, "Open Audio File", "", "Audio Files (*.wav *.pcm);;All Files (*)");

    // Check if the user canceled the file dialog
    if (initialfilePath.isEmpty()) {
        printf("File selection canceled\n");
        return;
    }

    //  Convert QString to const char* for compatibility with your existing code
    const char* filePathChar = initialfilePath.toUtf8().constData();

    // Now you can use the file path to load the audio data
    FILE* f = fopen(filePathChar, "rb");
    if (!f) {
        printf("Unable to open %s\n", filePathChar);
        return;
    }

    fread(buffer, sizeof(short), bufferSize, f);
    fclose(f);

    printf("Audio loaded from %s\n", filePathChar);

    fclose(f);
}

// Function to load audio data from a file into a buffer
void LoadAudio(short* buffer, long bufferSize, const char* filePath) {

    // Open a file dialog for the user to select a file
    QString initialfilePath = QFileDialog::getOpenFileName(nullptr, "Open Audio File", "", "Audio Files (*.wav *.pcm);;All Files (*)");

    // Check if the user canceled the file dialog
    if (initialfilePath.isEmpty()) {
        printf("File selection canceled\n");
        return;
    }

   //  Convert QString to const char* for compatibility with your existing code
    const char* filePathChar = initialfilePath.toUtf8().constData();

    // Now you can use the file path to load the audio data
    FILE* f = fopen(filePathChar, "rb");
    if (!f) {
        printf("Unable to open %s\n", filePathChar);
        return;
    }

    fread(buffer, sizeof(short), bufferSize, f);
    fclose(f);

    printf("Audio loaded from %s\n", filePathChar);

   fclose(f);
}

