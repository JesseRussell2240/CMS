#include "phoneBook.h"
#include <stdio.h>
#include <stdlib.h>

#define ADJLST

void graph_init(PhoneBook* phonebook) {
	*phonebook = NULL;
}

void graph_insert_edge(PhoneBook* phonebook, Item use, Msg mes, Length len, PayloadType load) {

	Link vptr = *phonebook;
	Link hptr;

	while (vptr->vlinks != NULL && vptr->user != use) {
		vptr = vptr->vlinks;
	}
	hptr = vptr;
	if (hptr->hlinks == NULL) {
		hptr->hlinks = (Link)malloc(sizeof(phoneBook));
		hptr = hptr->hlinks;
		hptr->hlinks = NULL;
		hptr->msg = mes;
		hptr->user = use;
		hptr->length = len;
	}
	else {
		while (hptr->hlinks != NULL) {
			hptr = hptr->hlinks;
		}
		hptr->hlinks = (Link)malloc(sizeof(phoneBook));
		hptr = hptr->hlinks;
		hptr->hlinks = NULL;
		hptr->msg = mes;
		hptr->user = use;
		hptr->length = len;
	}
}


void traverseRH(Link hlink) {
	if (hlink == NULL) return;
	traverseRH(hlink->hlinks);
	free(hlink);
}
void traverseRV(Link vlink) {
	if (vlink == NULL) return;
	traverseRV(vlink->vlinks);
	traverseRH(vlink->hlinks);
	free(vlink);
}
void graph_destroy(PhoneBook* phonebook) {
	traverseRV(*phonebook);
}


void graph_print_adjacency(PhoneBook* phonebook) {
	Link vptr = *phonebook;
	Link hptr;
	printf("\nNow printing adjacency\n");
	while (vptr != NULL) {
		hptr = vptr;
		printf("\nVertex %d is connected to vertices: ", hptr->user);

		hptr = hptr->hlinks;
		while (hptr != NULL) {
			printf(" %d ", hptr->user);
			hptr = hptr->hlinks;
		}
		vptr = vptr->vlinks;
	}
}


void graph_insert_vertex(PhoneBook* phonebook, Item data) {
	Link vptr = *phonebook;

	if (vptr == NULL) {
		vptr = (Link)malloc(sizeof(phoneBook));
		vptr->vlinks = NULL;
		vptr->hlinks = NULL;
		vptr->user = data;
		*phonebook = vptr;
	}
	else {
		while (vptr->vlinks != NULL) {
			vptr = vptr->vlinks;
		}
		vptr->vlinks = (Link)malloc(sizeof(phoneBook));
		vptr = vptr->vlinks;
		vptr->vlinks = NULL;
		vptr->hlinks = NULL;
		vptr->user = data;
	}

}

