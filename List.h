/****************************************************************************************
*  List.h
*  Melanie Wong
*  melywong
*  pa5
*  List header file: List "interface" - contains prototypes for List ADT Operations
*  
*****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
typedef struct ListObj* List;

// Constructor-Destructor
List newList(void);
void freeList(List* pL);

// Access functions
int length(List L);
int index(List L);
int front(List L);
int back(List L);
int get(List L);
int equals(List A, List B);

// Manipulation procedures
void clear(List L);

void moveFront(List L); // moves
void moveBack(List L);
void movePrev(List L);
void moveNext(List L);

void prepend(List L, int data); // adds
void append(List L, int data);

void insertBefore(List L, int data); 
void insertAfter(List L, int data);

void deleteFront(List L); // deletes
void deleteBack(List L);
void delete(List L);


// Other functions
void printList(FILE* out, List L);
List copyList(List L);

List concatList(List A, List B); // optional