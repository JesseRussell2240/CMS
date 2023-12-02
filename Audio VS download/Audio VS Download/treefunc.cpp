/* treefunc.cpp - Implementation of the tree functions in the BST
 * By: Michael
 *
 */

#include "treenode.h"
#include "treefunc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

// Private variables 
static link root;																// Root of the BST
static Item NullItem{ "Not found - Leaf has been reached" };					// Indicate that a leaf was reached

// Function Implementations

link NEW(Item item, link left, link right) {									// Create a new BST node
	link pNew = (link)malloc(sizeof(Node));										//Allocate memory for new node
	pNew->msg = item;															//Set data of new node to item
	pNew->pLeft = left;															//set left branch of new node
	pNew->pRight = right;														//set right branch of new node
	return(pNew);																//Return new node
}

void BSTInit(void) {															// Initialize a BST
	root = NULL;																// Use this so that content will be added at same time root is created
}
	
Item BSTSearch(link h, char* szKey) {											// Private search function called by 'Search()'
	int rc;
	if (h == NULL) return(NullItem);											// If the current node is null return the Nullitem
	rc = strcmp(szKey, h->msg.buff);											// Compare the search key with the current node key
	if (rc == 0) return(h->msg);												// If the keys match return current node

	// Recursive subcalls
	if (rc < 0) return(BSTSearch(h->pLeft, szKey));								// Go left
	else        return(BSTSearch(h->pRight, szKey));							// Go right if RC < 0
}

Item Search(char* szKey) {														// Public search function
	return(BSTSearch(root, szKey));												// Start search from root of tree
}

link BSTInsert(link h, Item item) {												// Private insert function called by 'Insert()'
	int rc; 
	if (h == NULL) return(NEW(item, NULL, NULL));								// If current node is NULL create new node
	rc = strcmp(item.buff, h->msg.buff);

	// Recursive subcalls for traversing the tree to get to a leaf
	if (rc < 0) h->pLeft = BSTInsert(h->pLeft, item);							// If the new item should be inserted to the left
	else        h->pRight = BSTInsert(h->pRight, item);							// Or the new item should be insertedto the right 

	return(h);																	//Return current node after insertion
}

void Insert(Item item) {														// Public insert function 
	root = BSTInsert(root, item);												// Start insertion from root
}

void BSTPrint(link h) {															// Private print function called by Print() - Inorder traversal
	if (h == NULL) return; 

	// Recursive subcalls
	BSTPrint(h->pLeft);						// Left
	printf("\nKey: %s", h->msg.buff);		// Center
	BSTPrint(h->pRight);					// Right
}

void Print(void) {
	BSTPrint(root);																// Does not require user define root
}

int height(link h) {															// Returns the height of the BST
	int iLeftH, iRightH;

	if (h == NULL) return(0);				// Base / Terminal condition - reached leaf
											// Updated from video where this returned -1
	// Recursive subcalls 
	iLeftH = height(h->pLeft);				// Recursively find height of left subtree
	iRightH = height(h->pRight);			// Recusively find height of right subtree

	if (iLeftH > iRightH) 
	return(iLeftH + 1);						//Return max height of left subtree
	else return(iRightH + 1);				//Return max height of right subtree
}

int count(link h) {																// Returns number of nodes in the BST
	if (h == NULL) return(0);													// Baser case / terminal condition 
	return(count(h->pLeft) + count(h->pRight) + 1);
}

link getRoot(void) {															// Returns a pointer to the root of the BST
	return(root);
}


// New function - free nodes in BST via 'postorder' (Left child, right child, parent) traversal of tree to delete from bottom up
void freeBST(link node) {
	if (node == NULL) return;		// Base case - Reached NULL child of leaf node to be free()'d

	// 'Postorder' (L, R, C) traversal
	freeBST(node->pLeft);			// Left
	freeBST(node->pRight);			// Right
	printf("\nNow freeing: %s", node->msg.buff);  // Delete parent
	free(node);						// Centre (free parent of NULL leaf node)
	
	return;
}