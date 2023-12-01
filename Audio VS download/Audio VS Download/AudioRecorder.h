#pragma once





// Function to record audio and store it in a buffer
void RecordAudio(short* buffer, long bufferSize, int sampleRate);
//void PlayAudio(short* buffer, long bufferSize);
void SaveAudio(short* buffer, long bufferSize, const char* filePath);
void LoadAudio(short* buffer, long bufferSize, const char* filePath);
void RecordAudioTB(short* buffer, long bufferSize, int durationInSeconds, int sampleRate);
void RecordBufferTB(short* buffer, long bufferSize, int durationInSeconds, int sampleRate);
void PlayAudio(short* buffer, long bufferSize, int sampleRate);


