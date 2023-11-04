#pragma warning(disable: 4996)

/* Implementation: Functions for File IO - Getting random messages from a file
*  By: Michael Galle
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>			// Dynamic memory allocation for arrays that store quote location and length
#include "message.h"
#include <string.h>
#include <time.h>

// Function gets a random number between min and max (max is the number of quotes in the file)
int frandNum(int min, int max) {

	//srand(time(NULL));
	return min + rand() % (max - min + 1);
}

// Function returns number of quotes in the file (only need to run once)
int fnumQuotes(void) {

	FILE* file = fopen("FortuneCookies.txt", "r");					//Open FortuneCookies.txt and read file
	//FILE* file = fopen("FortuneCookies.txt", "w");
	if (file == NULL) {												
		printf("Error: File cannot be found");						//If file does not exist print statment			
		return -1;													//Error flag if it did not find file
	}

	char line[4000];												//Set character limit to 4000
	int quoteCount = -1;											//Start at negative 1 because there is one more "%%" than quote number

	while (fgets(line, sizeof(line), file)) {						//Reads the file and stores in a character array
		if (strstr(line, "%%") != NULL)								//Checks if %% is on the line, returns nothing if there is no "%%" on that line
			quoteCount++;											//if "%%" is found it increments quoteCount, tracking the number of quotes in the file
	}

	fclose(file);													//close fortunceCookies.txt
	return quoteCount;												//Return number of quotes
}



// Function returns an array that indicates the start of every quote in the file (number of characters from the start of the file) 
long int* fquoteIndices(int numQuotes) {

	FILE* file = fopen("FortuneCookies.txt", "r");							//open FortuneCookies.txt and read file

	// Allocate memory for the indices array with an inital estimiate int maxQuotes 
	int maxQuotes = 2000;

	// Adjust this bases on your expected maximum number of quotes 
	long int* indices = (long int*)malloc(maxQuotes * sizeof(long int));	//Allocates memory to long int to store indices

	if (indices == NULL) {													//Checks if allocation worked correctly
		perror("Memory Allocation error");									//If not, it prints erroy message
		fclose(file);
		return NULL;
	}

	int quoteCount = 0;														//Start of quote count is 0
	char line[3000];														//Set character limit															

	while (fgets(line, sizeof(line), file)) {

		if (strcmp(line, "%%\n") == 0) {									//Checks if %% is on the line, returns nothing if there is no "%%" on that line

			//check if we need to reallocate memory for indices
			if (quoteCount >= maxQuotes) {									//Checks uf quote count is bigger than the set max quote
				maxQuotes *= 2;												//double the capacity if not enough room
				long int* newIndices = (long int*)realloc(indices, maxQuotes * sizeof(long int));	//then reallocates memory for extra data
				if (newIndices == NULL) {									//checks if unable to reallocate memory 
					perror("Memory Allocation Error");						//Prints error message
					free(indices);
					fclose(file);
					return NULL;
				}
				indices = newIndices;										//If reallocation is succesful sets new indices
			}

			//store the starting position of the quote

			indices[quoteCount] = ftell(file);								//Records current file Position				
			quoteCount++;													//increment as it track quotes
		}											
	}

	fclose(file);															//Close file
	numQuotes = quoteCount;													//set number of quotes equal to the quote count
	return indices;															//returns indices 
}

// Function returns the smaller of the actual quote length or MAX_QUOTE_LENGTH
int* fquoteLength(int numQuotes, long int* quoteIndices) {
	
	FILE* file = fopen("FortuneCookies.txt", "r");							//open FortuneCookies.txt and read file

	// Allocate memory for the result
	int* quoteLength = (int*)malloc(numQuotes * sizeof(int));				//Allocates memory for integer quote length

		char ch;															//Variable for current character

	//loop through quotes and count their length
		for (int i = 0; i < numQuotes; i++) {								//for loop goes trhough every quote
			fseek(file, quoteIndices[i], SEEK_SET);							//Moves the pointer to quiteIndices and starts reading from that current quote
			quoteLength[i] = 0;												//sets the length at the start of every quote to 0

			while (((ch = fgetc(file)) != EOF) && ch < MAX_QUOTE_LENGTH) {	//Checks if the current character is the EOF, and if current quote length ch is less the max quote length 
				quoteLength[i]++;											//incremnts quote length for every character

				if (ch == '%' && ch++ == '%') {								//checks if there is a "%%". If there is it breaks from the loop

					// Check for "%%" to stop reading
					break;
			}
		}
	}
		fclose(file);														//Close the file
		return quoteLength;													//return the character length of the quote
} 

// Function that gets q random quote from the FortuneCookies file 
int GetMessageFromFile(char* buff, int iLen, int randNum, int numQuotes, long int* quoteIndices, int* quoteLengths) {

	FILE* file = fopen("FortuneCookies.txt", "r");

	// Set file position to the start of the selected quote
	fseek(file, quoteIndices[randNum], SEEK_SET);

	int bytesRead = 0;														//Track the number of bytes read
	char ch;

	while (bytesRead < iLen-1 && (ch = fgetc(file)) != EOF) {

		if (ch == '%' && ch++ == '%') {
			//Check for "%%" to stop reading
			break;															//break the loop
		}
		// Append the character to the buffer
		buff[bytesRead++] = ch;
	}

	// NULL-terminate the buffer
	buff[bytesRead] = '\0';													

	fclose(file);															//Close file

	return bytesRead;														//Return the number of characters read
}