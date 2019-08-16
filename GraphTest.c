/****************************************************************************************
*  GraphTest.c
*  Melanie Wong
*  melywong
*  pa5
*  Thoroughly tests functions of Graph ADT by getting vertices from input file
*  
*****************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Graph.h"

int main(int argc, char* argv[]) {
	int n = 0;
	int i = 0;
	FILE *in, *out;

	if(argc != 3) {
		printf("Usage: %s <input file> <output file>\n", argv[0]);
		exit(1);
	}

	in = fopen(argv[1], "r");
   	out = fopen(argv[2], "w");
   	if( in==NULL ){
      printf("Unable to open file %s for reading\n", argv[1]);
      exit(1);
   	}
   	if( out==NULL ){
      printf("Unable to open file %s for writing\n", argv[2]);
      exit(1);
    }
    fscanf(in, "%d", &n);
    Graph G = newGraph(n);
    printf("new Graph G (empty):\n");
    printGraph(stdout, G);
    printf("G has %d vertices\n", getOrder(G));

    while (!feof (in)) {  
      fscanf (in, "%d", &n); 
      fscanf (in, "%d", &i);
      if(n == 0 || i == 0)
      	break;
      addArc(G, n, i);       
    }
    printf("Adjacency list representation of G:\n");
    fprintf(out, "Adjacency list representation of G:\n");
    printGraph(stdout, G); // adj List rep.
    printGraph(out, G);
    printf("\n");
    fprintf(out, "\n");
    Graph copy = copyGraph(G);
    printf("Copy of Graph G:\n");
    printGraph(stdout, copy);

    List S = newList();
    for(int i = 1; i <= getOrder(G); i++) { // loads List with vertices, ascending order
    	append(S, i);
    }
    printf("List before DFS:\n");
    printList(stdout, S); // prints starting List 1...n
    printf("\n");

    DFS(G, S);

    printf("List after DFS:\n");
    printList(stdout, S); // prints vertices in desc. finish times
    printf("\n");

    for(int a = 1; a <= getOrder(G); a++) {
    	printf("v%d D time = %d\n", a, getDiscover(G, a));
    	printf("v%d F time = %d\n", a, getFinish(G, a));
    }

    Graph Gt = transpose(G);
    printf("Tranpose of Graph G (ascending vertex order):\n");
    printGraph(stdout, Gt); // adj. List of transpose G
    printf("\n");

    DFS(Gt, S);

    printf("List after DFS(Gt):\n");
    printList(stdout, S); // sorted List of SCC
    printf("\n");

    for(int b = 1; b <= getOrder(G); b++) {
    	printf("v%d D time = %d\n", b, getDiscover(G, b));
    	printf("v%d F time = %d\n", b, getFinish(G, b));
    }

    // SCC part
    moveFront(S);
    int num = 0;
    while(index(S) != -1) {
      //printf("%d parent is %d\n", get(S), getParent(Gt, get(S)));
      if(getParent(Gt, get(S)) == 0) { // counts SCC's
        num++;
       // printf("%d has parent NIL\n", get(S));
      }
      moveNext(S);
    }
    printf("G contains %d strongly connected components:\n", num);
    fprintf(out, "G contains %d strongly connected components:\n", num);
    int count = 0;
    for(int j = 1; j <= num; j++) {
      moveBack(S);
      printf("Component %d:", j);
      fprintf(out, "Component %d:", j);
      for(int k = 0; k < count; k++) { // moves to next SCC in List
        movePrev(S);
        //printf("C at %d\n", get(S));
      }
      while(getParent(Gt, get(S)) != 0) {
        movePrev(S);
       // printf("C at %d\n", get(S));
      }
      if(getParent(Gt, get(S)) == 0) {
       // printf("%d has parent NIL\n", get(S));
        printf(" %d", get(S));
        fprintf(out, " %d", get(S));
        count++;
        moveNext(S);
        while(index(S) != -1 && getParent(Gt, get(S)) != 0) {
          printf(" %d", get(S));
          fprintf(out, " %d", get(S));
          count++;
          moveNext(S);
        }
      }
      printf("\n");
      fprintf(out, "\n");
    }

    freeGraph(&G);
    freeGraph(&Gt);
    freeGraph(&copy);
    freeList(&S);

    fclose(in);
    fclose(out);

    return(0);   
}