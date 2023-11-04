/* main.cpp - Main file for testing the File IO functions that get random messages from a file 
*  By: Michael Galle
*/

#pragma warning(disable: 4996)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>			// Needed for srand
#include "message.h"
#include "Queues.h"

int QsMain() {

	int numQuotes;					 // Number of quotes in the file
	long int* quoteIndices;		   	 // Array of quote locations in the file (index correspondes to quote number)
	int* quoteLengths;				 // Array of quote lengths (index correspondes to quote number)
	char testBuff[MAX_QUOTE_LENGTH]; // Buffer to write the message to
	int result;						 // result = 0 if successfully get a message


	// Count the number of quotes in the file and index it for fast access (only need to do this once) 
	numQuotes = fnumQuotes();									// Number of quotes
	quoteIndices = fquoteIndices(numQuotes);					// Index locations of the quotes
	quoteLengths = fquoteLength(numQuotes, quoteIndices);		// Length of each quote (up to MAX_QUOTE_LENGTH) - cut off after 	
  
	// Get the random message from the file
	srand(time(NULL));											// Seed the random number generator (only need to do once)


	// Queues
	link p, q;
	int i;

	InitQueue();

	int QuoteNum;

	printf("Enter the number of quotes you want: ");
	scanf("%d", &QuoteNum);
	for (i = 0; i < QuoteNum; i++) {
		p = (link)malloc(sizeof(Node));


		result = GetMessageFromFile(p->Data.message, MAX_QUOTE_LENGTH, frandNum(1, numQuotes), numQuotes, quoteIndices, quoteLengths);
		p->Data.sid = i;								// (*p).Data.sid = i

		AddToQueue(p);

	}

	while (!IsQueueEmpty()) {
		q = DeQueue();
		if (q != NULL) {
			printf("\n Message: %s", q->Data.message);	// (*q),Data.sid

		}


	}

	free(quoteIndices);
	free(quoteLengths);

	return(0);
}
