/*	Queues.cpp - Implementation for the Queue functionaility


*/

#include "Queues.h"
#include <stdlib.h>
#include <stdio.h>

static link phead, ptail;


void InitQueue(void) {
	phead = ptail = NULL;			//Initializes the head and teail pointers to null making the queue empty
}

int IsQueueEmpty(void) {
	return(phead == NULL);			//checks if phead is empty and returns if it is or not
}

void AddToQueue(link pn) {
	if (IsQueueEmpty()) {			//If queue is empty
		phead = ptail = pn;
									//Set head and tail to the new pointer to new node called pn
	}
	else {
		ptail->pNext = pn;			//old tail points to the node pn
		ptail = pn;					//ptail points to pn

	}
	ptail->pNext = NULL;			//Set the end of the queue to NULL
}

link DeQueue(void) {
	link pTemp;						//Hold the current head
	if (IsQueueEmpty()) {			//Checks if queue is empty in functions
			return(NULL);			//returns null if queue is empty
	} else {
			pTemp = phead;			// pHead is returned and then incremented to next node in list
			phead = phead->pNext;	// pHead points to next node in list
			return(pTemp);			// pTemp is original head

	}
}

void PrintQueueContents(void) {
	link currentNode = phead;

	if (IsQueueEmpty()) {
		printf("Queue is empty.\n");
		return;
	}

	printf("Queue contents:\n");

	while (currentNode != NULL) {
		Item currentItem = currentNode->Data;

		printf("Message: %s, Sender ID: %d, Receiver ID: %d, Priority: %c\n",
			currentItem.message, currentItem.sid, currentItem.rid,
			currentItem.priority);

		currentNode = currentNode->pNext;
	}
}

