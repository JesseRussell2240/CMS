#pragma once


// Function prototypes
void setComBits(int bits);
void setComRate(int rate);
void initializeRxPort(const wchar_t* port);
void initializeTxPort(const wchar_t* port);
void transmitMessage(const char* msg);
void receiveMessages();
void transmitAudio(const extern short* audioData, int dataSize);
void receiveAudio(extern short* audioData, int dataSize);
int transmit(extern short audioData[], int dataSize);