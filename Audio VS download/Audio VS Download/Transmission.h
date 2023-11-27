#pragma once


typedef struct header HeaderForPayload;

struct header {
	short int sid;				//sender ID
	short int rid;				//reciver ID 
	char priority;				//0-7 prioaty of importance in 
	short int seqNum;				
	long int payloadSize;		// Number of bytes in payload after this header
	char payLoadType;			// 0: Text, 1: Audio, 2: Image etc.
	char encryption;			// 0: None, 1: XOR,	  2: Vigenere	3: Both
	char compression;			// 0: None, 1: RLE,	  2: Huffman,	3: Both
	
};

// Function prototypes
void setComBits(int bits);
void setComRate(int rate);
void initializePort( wchar_t* port);
void transmitMessage(const char* msg);
void receiveMessages(char* msgBuffer, int* msgLength);
void transmitAudio( short* audioData, int dataSize);
void receiveAudio(short* audioData, int dataSize);
//DWORD receivePayload(Header* Header, void** Payload, HANDLE* hCom, wchar_t* COMPORT, int nComRate, int nComBits, COMMTIMEOUTS timeout);
//void transmitPayload(Header* Header, void* Payload, HANDLE* hCom, wchar_t* COMPORT, int nComRate, int nComBits, COMMTIMEOUTS timeout);
DWORD receivePayload(HeaderForPayload* Header, void** Payload, wchar_t* port, int nComRate);
void transmitPayload(HeaderForPayload* Header, void* Payload, wchar_t* port, int nComRate);