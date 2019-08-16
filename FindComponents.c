/****************************************************************************************
*  FindComponents.c
*  Melanie Wong
*  melywong
*  pa5
*  Prints adjacency list of Graph G from input file, as well as its strongly connected
*  components to an output file after running DFS on G and its tranpose
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

    while (!feof (in)) {  
      fscanf (in, "%d", &n); 
      fscanf (in, "%d", &i);
      if(n == 0 || i == 0)
      	break;
      addArc(G, n, i);       
    }

    fprintf(out, "Adjacency list representation of G:\n");
    printGraph(out, G);
    fprintf(out, "\n");

    List S = newList();
    for(int i = 1; i <= getOrder(G); i++) { // loads List with vertices, ascending order
    	append(S, i);
    }

    DFS(G, S);

    Graph Gt = transpose(G);
    
    DFS(Gt, S);

    // SCC part
    moveFront(S);
    int num = 0;
    while(index(S) != -1) {
      if(getParent(Gt, get(S)) == 0) { // counts SCC's
        num++;
      }
      moveNext(S);
    }

    fprintf(out, "G contains %d strongly connected components:\n", num);
    int count = 0;
    for(int j = 1; j <= num; j++) {
      moveBack(S);
      fprintf(out, "Component %d:", j);
      for(int k = 0; k < count; k++) { // moves to next SCC in List
        movePrev(S);
      }
      while(getParent(Gt, get(S)) != 0) {
        movePrev(S);
      }
      if(getParent(Gt, get(S)) == 0) {
        fprintf(out, " %d", get(S));
        count++;
        moveNext(S);
        while(index(S) != -1 && getParent(Gt, get(S)) != 0) {
          fprintf(out, " %d", get(S));
          count++;
          moveNext(S);
        }
      }
      fprintf(out, "\n");
    }

    freeGraph(&G);
    freeGraph(&Gt);
    freeList(&S);

    fclose(in);
    fclose(out);

    return(0);
}