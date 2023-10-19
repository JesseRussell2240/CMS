// main.cpp : Mainline for testing the Queue and Recursive Traverse functions - Assignment #2
//

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "Queues.h"

#pragma warning(disable : 4996)


int main()
{
	link p, q;
	int i;

	char message[100];

	printf("Enter a palandrom or message: ");
	scanf("%s", message);

	InitQueue();

	// Create & Add Nodes numbered 1 to 10 to the Queue
	for (int i = 0; message[i] != '\0'; i++) {
		p = (link)malloc(sizeof(Node));
		p->Data.sid = message[i];                     // (*p).Data.sid = i;
		//(*p).Data.sid = i;
		AddToQueue(p);
	}


	// Test count() 
	printf("\nThe number of nodes in the Queue is: %d\n", count(returnHead()));

	printf("\n\nTraversing list in forward direction gives ...\n");
	printf("\n*******************************************************\n");
	traverse(returnHead(), visit);
	printf("\n\nTraversing list in reverse direction gives ...\n");
	printf("\n*******************************************************\n");
	traverseR(returnHead(), visit);

	// Test deleteR()
	Item v;
	v.sid = 7;
	deleteR(returnHead(), returnHead()->pNext, v);

	printf("\n\nThe number of nodes in the Queue after the deleteR() operation is: %d\n", count(returnHead()));

	// Empty  the Queue
	while (!IsQueueEmpty()) {
		q = DeQueue();
		printf("\n SID of Node is: %d ", q->Data.sid);		// (*q).Data.sid
	}


	// Test count() 
	printf("\n\nThe number of nodes in the Queue after dequeuing all nodes is: %d\n", count(returnHead()));



	return(0);
}
