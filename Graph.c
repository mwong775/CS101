/****************************************************************************************
*  Graph.c
*  Melanie Wong
*  melywong
*  pa5
*  Graph ADT using an array of Lists and involving vertices and edges,  
*  with DFS and transpose functions. Print function produces adjacency list representation.
*
*****************************************************************************************/
#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct GraphObj {
	// adj list
	List* adj;
	// color of vertices
	int* color;
	// discover time of vertices
	int* d;
	// finish time of vertices
	int* f;
	// parents of vertices
	int* p;
	// order = #b of vertices
	int order;
	// size = # of edges
	int size;

} GraphObj;

typedef GraphObj* Graph;

/* Constructors-Destructors */ 
Graph newGraph(int n) {
	Graph G = malloc(sizeof(GraphObj));
	G->color = malloc((n+1)*sizeof(int));
	G->d = malloc((n+1)*sizeof(int));
	G->f = malloc((n+1)*sizeof(int));
	G->p = malloc((n+1)*sizeof(int));

	G->adj = malloc((n+1)*sizeof(List));
	for(int i = 1; i < n+1; i++) {
		G->adj[i] = newList();
		// discover & finish times of vertices undefined before DFS is called
		G->d[i] = UNDEF;
		G->f[i] = UNDEF;
		G->p[i] = NIL;
	}
	
	G->order = n;
	G->size = 0;

	return(G);
} 

void freeGraph(Graph* pG) {
	if((*pG)->color != NULL) {
		free((*pG)->color);
		(*pG)->color = NULL;
	}	
	if((*pG)->d != NULL) {
		free((*pG)->d);
		(*pG)->d = NULL;
	}
	if((*pG)->f != NULL) {
		free((*pG)->f);
		(*pG)->f = NULL;
	}
	if((*pG)->p != NULL) {
		free((*pG)->p);
		(*pG)->p = NULL;
	}
	for(int i = 1; i < getOrder(*pG)+1; i++) {
		freeList(&((*pG)->adj[i]));
	}
	if((*pG)->adj != NULL) {
		free((*pG)->adj);
		(*pG)->adj = NULL;
	}

	if(pG != NULL && *pG != NULL) {
		free(*pG);
		*pG = NULL;
	}
} 

/* Access functions */ 
int getOrder(Graph G) {
	return(G->order);
} 

int getSize(Graph G) {
	return(G->size);
}

int getParent(Graph G, int u) {  /* Pre: 1<=u<=n=getOrder(G) */
	if(u < 1 || u > getOrder(G)) {
		printf("Graph Error: invalid vertex u in getParent()");
		exit(1);
	} 
	return(G->p[u]);
}

int getDiscover(Graph G, int u) {   /* Pre: 1<=u<=n=getOrder(G) */ 
	if(u < 1 || u > getOrder(G)) {
		printf("Graph Error: invalid vertex u in getDiscover()");
		exit(1);
	} 
	return(G->d[u]);
}

int getFinish(Graph G, int u) {   /* Pre: 1<=u<=n=getOrder(G) */ 
	if(u < 1 || u > getOrder(G)) {
		printf("Graph Error: invalid vertex u in getFinish()");
		exit(1);
	} 
	return(G->f[u]);
}
 
/* Manipulation procedures */ 
void addArc(Graph G, int u, int v) {  /* Pre: 1<=u<=n, 1<=v<=n */ 
	// inserts directed edge u to v
	// (adds v to adj List of u)
	if(u < 1 || u > getOrder(G)) {
		printf("Graph Error: invalid vertex u in addArc()");
		exit(1);
	} 
	if(v < 1 || v > getOrder(G)) {
		printf("Graph Error: invalid vertex v in addArc()");
		exit(1);
	} 
	moveFront(G->adj[u]);
	while(index(G->adj[u]) != -1 && v > get(G->adj[u])) {
		moveNext(G->adj[u]);
	}
	if(index(G->adj[u]) == -1) {
		append(G->adj[u], v);
	}
	else {
		insertBefore(G->adj[u], v);
	}
	G->size++;
}

void addEdge(Graph G, int u, int v) {  /* Pre: 1<=u<=n, 1<=v<=n */ 
	// inserts new edge, joining u to v
	// (add to each other's adj List)
	if(u < 1 || u > getOrder(G)) {
		printf("Graph Error: invalid vertex u in addEdge()");
		exit(1);
	} 
	if(v < 1 || v > getOrder(G)) {
		printf("Graph Error: invalid vertex v in addEdge()");
		exit(1);
	} 
	moveFront(G->adj[u]);
	while(index(G->adj[u]) != -1 && v > get(G->adj[u])) {
		moveNext(G->adj[u]);
	}
	if(index(G->adj[u]) == -1) {
		append(G->adj[u], v);
	}
	else {
		insertBefore(G->adj[u], v);
	}

	moveFront(G->adj[v]);
	while(index(G->adj[v]) != -1 && u > get(G->adj[v])) {
		moveNext(G->adj[v]);
	}
	if(index(G->adj[v]) == -1) {
		append(G->adj[v], u);
	}
	else {
		insertBefore(G->adj[v], u);
	}

	G->size++;
}

void Visit(Graph G, List S, int x, int* time) {
	G->color[x] = 1; // GRAY
	G->d[x] = (++*time); 
	moveFront(G->adj[x]);
	while(index(G->adj[x]) != -1) {
		int y = get(G->adj[x]);
		if (G->color[y] == 0) {
			G->p[y] = x;
			Visit(G, S, y, time);
		}
		G->color[get(G->adj[x])] = 2; // BLACK
		moveNext(G->adj[x]);
	}
	G->f[x] = (++*time);
	prepend(S, x);
}

void DFS(Graph G, List S) {    /* Pre: length(S)==getOrder(G) */ 
	// 0 = WHITE, 1 = GRAY, 2 = BLACK
	if(length(S) != getOrder(G)) {
		printf("Graph Error: List length %d not equal to Graph order %d in DFS()", length(S), getOrder(G));
		exit(1);
	}
	for(int i = 1; i < getOrder(G)+1; i++) {
		G->color[i] = 0; // WHITE
		G->p[i] = NIL;
	}
	int time = 0;
	moveFront(S);
	while(index(S) != -1) {
		if(G->color[get(S)] == 0) {
			Visit(G, S, get(S), &time);
		}
		moveNext(S);
	}
	for(int j = 0; j < getOrder(G); j++) { // deletes initial vertex order of List S
		deleteBack(S);
	}
}
 
/* Other Functions */ 
Graph transpose(Graph G) {
	Graph Gt = newGraph(getOrder(G));

	for(int i = 1; i < getOrder(G)+1; i++) {
		moveFront(G->adj[i]);
		while(index(G->adj[i]) != -1) {
			addArc(Gt, get(G->adj[i]), i);
			moveNext(G->adj[i]);
		}
	}
	return(Gt);
}

Graph copyGraph(Graph G) {
	Graph copy = newGraph(getOrder(G));
	
	for(int i = 1; i < getOrder(G)+1; i++) {
		moveFront(G->adj[i]);
		while(index(G->adj[i]) != -1) {
			addArc(copy, i, get(G->adj[i]));
			moveNext(G->adj[i]);
		}
	}

	return(copy);
}

void printGraph(FILE* out , Graph G) {
	for(int i = 1; i < G->order+1; i++) {
		fprintf(out, "%d: ", i);
		moveFront(G->adj[i]);
		while(index(G->adj[i]) != -1) {
			fprintf(out, "%d ", get(G->adj[i]));
			moveNext(G->adj[i]);
		}
		fprintf(out, "\n");
	}
}