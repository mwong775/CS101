/****************************************************************************************
*  Graph.h
*  Melanie Wong
*  melywong
*  pa5
*  Graph header file: Graph "interface" - contains prototypes for Graph ADT Operations
*  
*****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "List.h"

#define NIL 0
#define UNDEF -1

typedef struct GraphObj* Graph;

/* Constructors-Destructors */ 
Graph newGraph(int n); 
void freeGraph(Graph* pG); 

/* Access functions */ 
int getOrder(Graph G); 
int getSize(Graph G); 
int getParent(Graph G, int u);  /* Pre: 1<=u<=n=getOrder(G) */ 
int getDiscover(Graph G, int u);   /* Pre: 1<=u<=n=getOrder(G) */ 
int getFinish(Graph G, int u);   /* Pre: 1<=u<=n=getOrder(G) */ 
 
/* Manipulation procedures */ 
void addArc(Graph G, int u, int v);  /* Pre: 1<=u<=n, 1<=v<=n */ 
void addEdge(Graph G, int u, int v);  /* Pre: 1<=u<=n, 1<=v<=n */ 
void DFS(Graph G, List S);    /* Pre: length(S)==getOrder(G) */ 
 
/* Other Functions */ 
Graph transpose(Graph G); 
Graph copyGraph(Graph G); 
void printGraph(FILE* out , Graph G); 