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
	int priority;				// Priority of message
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
void PrintQueueContents(void);
void PrintQueueContentsByPriority(void);

void DequeueLIFO(void);
void DequeueByPriority(void);
void savePhoneBook(const char* filename);
void loadPhoneBook(const char* filename);
int numOfMessages(void);
int numOfUserMessages(int userID);