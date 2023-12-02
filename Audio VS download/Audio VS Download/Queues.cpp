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

void PrintQueueContentsByPriority(void) {

	if (IsQueueEmpty()) {
		printf("Queue is empty.\n");
		return;
	}

	// Create an array to store pointers to nodes for sorting
	int queueSize = 0;
	link* queueArray = (link*)malloc(sizeof(link) * queueSize);


	if (queueArray == NULL) {
		perror("Memory allocation error");
		exit(EXIT_FAILURE);
	}

	link currentNode = phead;
	while (currentNode != NULL) {
		queueArray = (link*) realloc(queueArray, sizeof(link) * (queueSize + 1));

		if (queueArray == NULL) {
			perror("Memory reallocation error");
			exit(EXIT_FAILURE);
		}

		queueArray[queueSize] = currentNode;
		queueSize++;
		currentNode = currentNode->pNext;
	}


	//dumb dumb simple sort
	for (int i = 0; i < queueSize - 1; i++) {
		for (int j = 0; j < queueSize - i - 1; j++) {
			if (queueArray[j]->Data.priority > queueArray[j + 1]->Data.priority) {
				// Swap pointers
				link temp = queueArray[j];
				queueArray[j] = queueArray[j + 1];
				queueArray[j + 1] = temp;
			}
		}
	}

	// Display the sorted messages
	printf("Queue contents (Priority Order):\n");
	for (int i = 0; i < queueSize; i++) {
		Item currentItem = queueArray[i]->Data;
		printf("Message: %s, Sender ID: %d, Receiver ID: %d, Priority: %c\n",
			currentItem.message, currentItem.sid, currentItem.rid,
			currentItem.priority);

	}

	// Free the allocated memory for the array
	free(queueArray);
}


void DequeueLIFO(void) {
	
		link currentNode = phead;
		link previousNode = NULL;

		// Traverse to the last node
		while (currentNode->pNext != NULL) {
			previousNode = currentNode;
			currentNode = currentNode->pNext;
		}

		// If there is only one node in the queue
		if (previousNode == NULL) {
			phead = ptail = NULL;
		}
		else {
			previousNode->pNext = NULL;
			ptail = previousNode;
		}

		printf("Dequeued in LIFO: Message: %s, Sender ID: %d, Receiver ID: %d, Priority: %c\n",
			currentNode->Data.message, currentNode->Data.sid, currentNode->Data.rid,
			currentNode->Data.priority);

		free(currentNode);  // Free the memory allocated for the dequeued node
	
}

void DequeueByPriority(void) {
	
	
		link* queueArray = (link*)malloc(sizeof(link) * 0);

		if (queueArray == NULL) {
			perror("Memory allocation error");
			exit(EXIT_FAILURE);
		}

		link currentNode = phead;
		int queueSize = 0;

		while (currentNode != NULL) {
			queueArray = (link*)realloc(queueArray, sizeof(link) * (queueSize + 1));

			if (queueArray == NULL) {
				perror("Memory reallocation error");
				exit(EXIT_FAILURE);
			}

			queueArray[queueSize] = currentNode;
			queueSize++;
			currentNode = currentNode->pNext;
		}

		// Sort the array based on priority
		for (int i = 0; i < queueSize - 1; i++) {
			for (int j = 0; j < queueSize - i - 1; j++) {
				if (queueArray[j]->Data.priority > queueArray[j + 1]->Data.priority) {
					// Swap pointers
					link temp = queueArray[j];
					queueArray[j] = queueArray[j + 1];
					queueArray[j + 1] = temp;
				}
			}
		}

		// Dequeue the highest priority item
		link dequeuedNode = queueArray[0];
		printf("Dequeued by Priority: Message: %s, Sender ID: %d, Receiver ID: %d, Priority: %c\n",
			dequeuedNode->Data.message, dequeuedNode->Data.sid, dequeuedNode->Data.rid,
			dequeuedNode->Data.priority);

		// Update the queue (remove the dequeued item)
		phead = dequeuedNode->pNext;
		free(dequeuedNode);

		// Free the allocated memory for the array
		free(queueArray);
	
}