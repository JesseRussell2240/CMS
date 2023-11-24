/*	Queues.h - Interface for the Queue functionaility


*/

#pragma once

const int ilength = 140;


typedef struct node Node;
typedef struct item Item;
typedef Node* link;

struct item {
	char message[ilength];
	short int sid;				// Sender ID
	short int rid;				// Reciever ID
	char priority;				// Priority of message
	short int seqNum;			//sequence number of the message
	char later[25];				//characters we might use later
};

struct node {
	link pNext;
	Item Data;
};

// Queue function prototypes

void InitQueue(void);
int IsQueueEmpty(void);
void AddToQueue(link);
link DeQueue(void);
