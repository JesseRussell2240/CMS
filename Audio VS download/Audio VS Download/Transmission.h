#pragma once


// Function prototypes
void setComBits(int bits);
void setComRate(int rate);
void initializeRxPort(const wchar_t* port);
void initializeTxPort(const wchar_t* port);
void transmitMessage(const char* msg);
void receiveMessages();
void transmitAudio( short* audioData, int dataSize);
void receiveAudio(short* audioData, int dataSize);
int transmit(short* audioData, int dataSize);