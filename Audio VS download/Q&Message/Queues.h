#pragma once
/*  Queues.h - Interface for the Queue functionality
 *  By: Michael Galle
 *
 */
#pragma once
const int iLength = 140;

typedef struct node Node;
typedef struct item Item;
typedef Node* link;

struct item {
	char message[iLength];
	short int sid;			// Sender ID
	short int rid;			// Receiver ID 
	char priority;			// Priority of message
	short int seqNum;		// Sequence number of the message
	char later[25];			// Character we might use later 
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

// Recursive Queue functions
link returnHead();
int count(link x);
link deleteR(link parent, link child, Item v);
void visit(link h);
void traverse(link h, void (*visit)(link));
void traverseR(link h, void (*visit)(link));