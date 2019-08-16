/****************************************************************************************
*  List.c
*  Melanie Wong
*  melywong
*  pa5
*  List ADT with doubly linked list underlying data structure
*  
*****************************************************************************************/
#include "List.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct NodeObj { // private NodeObj type
	int data;
	struct NodeObj* next;
	struct NodeObj* prev;
} NodeObj;

typedef NodeObj* Node; // private NodeObj type

Node newNode(int data) { // Node constructor
	Node new_node = malloc(sizeof(NodeObj));

	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = NULL;

	return(new_node);
}

void freeNode(Node* pN) { // Node destructor
	if(pN != NULL && *pN != NULL) {
		free(*pN);
		*pN = NULL;
	}
}

typedef struct ListObj { // private ListObj type
	Node front;
	Node back;
	Node cursor;
	int length;
	int index;
} ListObj;


List newList() { // List constructor
	List L = malloc(sizeof(ListObj));		// returns a List which points to new empty list object
	
	L->front= NULL;
	L->back= NULL;
	L->cursor = NULL; // code which initializes L
	L->length = 0;
	L->index = 0;
	
	return(L);
}

void freeList(List* pL) { // List destructor
	if(pL != NULL && *pL != NULL) {
		// free all heap memory associated with *pL
		if(length(*pL) > 0)
			clear(*pL); // calls delete calls freeNode until empty
		free(*pL);
		*pL = NULL;
	}
}

// Access functions

int length(List L) {
	if(L == NULL) {
		printf("List Error: calling length() on NULL List reference\n");
		exit(1);
	}
	return(L->length);
}

int index(List L) {
	if(L == NULL) {
		printf("List Error: calling index() on NULL List reference\n");
		exit(1);
	}
	if(L->cursor != NULL)
		return L->index;
	else				// index undef.
		return -1;
}

int front(List L) {	
	if(L == NULL) {
		printf("List Error: calling front() on NULL List reference\n");
		exit(1);
	}
	if(length(L) <= 0) { //Pre: if length > 0 violation
		printf("List Error: calling front() on empty List\n");
		exit(1);
	}
	return(L->front->data);
}

int back(List L) { 
	if(L == NULL) {
		printf("List Error: calling back() on NULL List reference\n");
		exit(1);
	}
	if(length(L) <= 0) { //Pre: if length > 0 violation
		printf("List Error: calling back() on empty List\n");
		exit(1);
	}
	return(L->back->data);
}

int get(List L) { 
	if(L == NULL) {
		printf("List Error: calling get() on NULL List reference\n");
		exit(1);
	}
	 if(length(L) <= 0 || index(L) < 0) { //Pre: length() > 0, index() >= 0 violation
		printf("List Error: calling get() on empty List and/or no cursor\n");
		exit(1);
	}
	return L->cursor->data;
}

int equals(List A, List B) { // Note: 1 = true,  0 = false
	if(A == NULL || B == NULL) {
		printf("List Error: calling equals() on NULL List reference(s)\n");
		exit(1);
	}
	if(length(A) != length(B))
		return 0;

	Node tempC = A->cursor; // saves current cursor settings
	int tempI = index(A);

	moveFront(A);
	moveFront(B);
	while(index(A) >= 0) {
		if(get(A) == get(B)) {
			moveNext(A);
			moveNext(B);
		}
		else {
			A->cursor = tempC; // return cursor settings
			A->index = tempI;

			return 0; // found difference - not equal
		}
	}
	A->cursor = tempC; // return cursor settings
	A->index = tempI;

	return 1; // compared whole sequence = equal
}

// Manipulation procedures

void clear(List L) {
	if(L == NULL) {
		printf("List Error: calling clear() on NULL List reference\n");
		exit(1);
	}
	while(length(L) > 0) {
		deleteFront(L); // no elements = cursor undefined
	}

}

void moveFront(List L) {
	if(L == NULL) {
		printf("List Error: calling moveFront() on NULL List reference\n");
		exit(1);
	}
	if(length(L) > 0) {
		L->cursor = L->front;
		L->index = 0;
	}
}

void moveBack(List L) {
	if(L == NULL) {
		printf("List Error: calling moveBack() on NULL List reference\n");
		exit(1);
	}
	if(length(L) > 0) {
		L->cursor = L->back;
		L->index = length(L) - 1;
	}
}

void movePrev(List L) {
	if(L == NULL) {
		printf("List Error: calling movePrev() on NULL List reference\n");
		exit(1);
	}
	if(L->cursor != NULL && L->cursor != L->front) {
		L->cursor = L->cursor->prev;
		L->index--;
	}
	else if(L->cursor == L->front) { // cursor "runs off" front = undef.
		L->cursor = NULL;
		L->index = -1;
	}
}

void moveNext(List L) {
	if(L == NULL) {
		printf("List Error: calling moveNext() on NULL List reference\n");
		exit(1);
	}
	if (L->cursor != NULL && L->cursor != L->back) {
		L->cursor = L->cursor->next;
		L->index++;
	}
	else if(L->cursor == L->back) { // cursor "runs off" back = undef.
		L->cursor = NULL;
		L->index = -1;
	}
}

void prepend(List L, int data) {
	if(L == NULL) {
		printf("List Error: calling prepend() on NULL List reference\n");
		exit(1);
	}
	if(length(L) == 0) { // 1st element = front & back
		Node n = newNode(data);
		L->back = n;
		L->front = n;
	}
	else {
		Node n = newNode(data);
		n->next = L->front;
		n->prev = NULL;
		if(L->front != NULL) {
			L->front->prev = n;
		}
		L->front = n;
	}

	if(index(L) >= 0 && L->cursor != NULL) {
		L->index++; // if cursor def, inc. index for added element 
	}
	L->length++;
}

void append(List L, int data) {
	if(L == NULL) {
		printf("List Error: calling append() on NULL List reference\n");
		exit(1);
	}
	if(length(L) == 0) { // 1st element = front & back
		Node n = newNode(data);
		L->back = n;
		L->front = n;
	}
	else {
		Node n  = newNode(data);
		if(L->back != NULL) {
			L->back->next = n;
		}	
		n->prev = L->back;
		n->next = NULL;	
		L->back = n;
	}
	L->length++;
}

void insertBefore(List L, int data) { // Pre: length() > 0, index() >= 0
	if(L == NULL) {
		printf("List Error: calling insertBefore() on NULL List reference\n");
		exit(1);
	}
	if(length(L) <= 0 || index(L) < 0) {
		printf("List Error: calling insertBefore() on empty List and/or no cursor\n");
		exit(1);
	}
	if(L->cursor == L->front) {
		prepend(L, data);
	}
	else {
		Node n  = newNode(data);
		n->next = L->cursor;
		n->prev = L->cursor->prev;
		L->cursor->prev = n;
		if(n->prev !=NULL)
			n->prev->next = n;

		L->length++;
		L->index++; // added element left of cursor
	}
}

void insertAfter(List L, int data) { // Pre: length() > 0, index() >= 0
	if(L == NULL) {
		printf("List Error: calling insertAfter() on NULL List reference\n");
		exit(1);
	}
	if(length(L) <= 0 || index(L) < 0) {
		printf("List Error: calling insertAfter() on empty List and/or no cursor\n");
		exit(1);
	}
	if(L->cursor == L->back) {
		append(L, data);
	}
	else {
		Node n = newNode(data);
		n->prev = L->cursor;
		n->next = L->cursor->next;
		L->cursor->next = n;
		if(n->next != NULL)
			n->next->prev = n;

		L->length++;
	}
}

void deleteFront(List L) { 
	if(L == NULL) {
		printf("List Error: calling deleteFront() on NULL List reference\n");
		exit(1);
	}
	if(length(L) <= 0) { // Pre: length > 0 violation
		printf("List Error: calling deleteFront() on empty List\n");
		exit(1);
	}
	if(L->cursor == L->front) { // cursor at front = undef.
		L->cursor = NULL;
		L->index = -1;
	}

	Node n = L->front; // saves node to delete & free

	if(length(L) > 1) {
		L->front = L->front->next;
	}
	else {
		L->front = L->back = NULL;
	}
		
	if(L->cursor != NULL && index >= 0) { // dec. index if defined
		L->index--;
	}
	L->length--;
	freeNode(&n);
}

void deleteBack(List L) { 
	if(L == NULL) {
		printf("List Error: calling deleteBack() on NULL List reference\n");
		exit(1);
	}
	if(length(L) <= 0) { // Pre: length > 0 violation
		printf("List Error: calling deleteBack() on empty List\n");
		exit(1);
	}
	if(L->cursor == L->back) { // cursor at back = undef.
		L->cursor = NULL;
		L->index = -1;
	}

	Node n = L->back; // saves node to delete & free

	if(length(L) > 1) {
		L->back = L->back->prev;
	}
	else {
		L->back = L->front = NULL;
	}
	L->length--;
	freeNode(&n);
}

void delete(List L) {
	if(L == NULL) {
		printf("List Error: calling delete() on NULL List reference\n");
		exit(1);
	}
	if(length(L) <= 0 || index(L) < 0) { // Pre: length() > 0, index() >= 0
		printf("List Error: calling delete() on empty List\n");
		exit(1);
	}
	if(L->front == L->cursor || length(L) == 1) { // if cursor at front or one item
		deleteFront(L);
	}
	else if(L->back == L->cursor) { // if cursor at back
		deleteBack(L);
	}
	else {
		Node n = L->cursor; // saves node to delete & free
		if(L->cursor->prev != NULL)
			L->cursor->prev->next = L->cursor->next;
		if(L->cursor->next != NULL)
			L->cursor->next->prev = L->cursor->prev;
		
		freeNode(&n);
		L->index = -1;
		L->length--;
	
	}
}

// Ohter operations
void printList(FILE* out, List L) { // prints to file, formatted as space-separated string
	if(L == NULL) {
		printf("List Error: calling printList() on NULL List reference\n");
		exit(1);
	}
	if(length(L) > 0) {
		moveFront(L);
		while(index(L) >= 0) {
			fprintf(out, "%d ", get(L));
			moveNext(L);
		}
	}

}

List copyList(List L) {
	if(L == NULL) {
		printf("List Error: calling copyList() on NULL List reference\n");
		exit(1);
	}
	Node tempC = L->cursor;	// save cursor's current state/settings
	int tempI = index(L);	// revert after copying

	List list = newList();
	if(length(L) > 0) {
		moveFront(L);
		while(index(L) >= 0) {
			append(list, get(L));
			moveNext(L);
		}
	}
	L->cursor = tempC;
	L->index = tempI; // return cursor's prev. state/position

	return(list);
}

List concatList(List A, List B) { // future reference: function has NOT been tested! :(
	if(A == NULL || B == NULL) {
		printf("List Error: calling printList() on NULL List reference\n");
		exit(1);
	}
	List L = newList();

	moveFront(A);
	while(index(A) >= 0) { // copies list A to list L
		append(L, get(A));
		moveNext(L);
	}

	moveFront(B);
	while(index(B) >= 0) { // copies list B to list L
		append(L, get(B));
		moveNext(L);
	}
	return L;
}

