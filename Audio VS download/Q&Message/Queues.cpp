/*  Queues.cpp - Implementation for the Queue functionality
 *  By: Michael Galle
 *
 */

#include "Queues.h"
#include <stdlib.h>
#include <stdio.h>

static link pHead, pTail;

void InitQueue(void) {
	pHead = pTail = NULL;
}

int IsQueueEmpty(void) {
	return(pHead == NULL);
}

void AddToQueue(link pn) {
	if (IsQueueEmpty()) {
		pHead = pTail = pn;
	}
	else {
		pTail->pNext = pn;		// Old tail now points to pn (the node we added)
		pTail = pn;				// pTail points to pn
	}
	pTail->pNext = NULL;
}


link DeQueue(void) {
	link pTemp;					// Hold the current Head
	if (IsQueueEmpty()) {
		return(NULL);
	}
	else {
		pTemp = pHead;			// pHead is returned and then incremented to next node in list
		pHead = pHead->pNext;	// pHead points to next Node in list
		return(pTemp);			// pTemp is original head
	}
}


// Assignment #2 - Recursive traverse functions

// Recursive Queue functions
link returnHead() {
	return pHead;
}

int count(link x) {
	if (x == NULL) return(0);
	return (1 + count(x->pNext));
}

link deleteR(link parent, link child, Item v) {
	if (child == NULL) return(NULL);
	if (child->Data.sid == v.sid) {
		parent->pNext = child->pNext;
		free(child);
		deleteR(parent, parent->pNext, v);
	}
	else {
		deleteR(child, child->pNext, v);
	}
}

void visit(link h) { //function to print the character at each 'location' in the message
	//	printf("SID of node: %d\n", h->Data.sid); 
	printf("%c", h->Data.sid); // Print the character stored in the node
}



void traverse(link h, void (*visit)(link)) { //Function to navigate the message and print the contents 
	if (h == NULL) return;   //check if node is empty
	(*visit)(h); //prints the contents of the node
	traverse(h->pNext, visit); //goes to the next node
} //repeat this procces until reaches the last node aka null, then goes down and closes them

void traverseR(link h, void (*visit)(link)) { //Function to navigate the message and print the contents in reverse
	if (h == NULL) return; //check if node is empty
	traverseR(h->pNext, visit); //Visit next node
	(*visit)(h); //Print contents of node
}



