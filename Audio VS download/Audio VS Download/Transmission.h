#pragma once


// Function prototypes
void setComBits(int bits);
void setComRate(int rate);
void initializePort(const wchar_t* port);
void transmitMessage(const char* msg);
void receiveMessages(char* msgBuffer, int* msgLength);
void transmitAudio( short* audioData, int dataSize);
void receiveAudio(short* audioData, int dataSize);
