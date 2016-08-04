//
//  main.c
//  Algo
//
//  Created by Ivy on 5/30/2016
//  Copyright (c) 2016 ___IVYCANT___. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
int nnn = 201; // max number of vertices just for alloc // this can be doubled if not enough 
typedef struct _vertex vertex;
typedef struct _edge edge;

struct _vertex {
  int degree;
  int *connectTo;
};

struct _edge {
  int u; // vertex index 1st endpoint
  int v; // vertex index 2nd endpoint
};

// Assumes 0 <= max <= RAND_MAX
// Returns in the closed interval [0, max]
long random_at_most(long max) {
  unsigned long
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    num_bins = (unsigned long) max + 1,
    num_rand = (unsigned long) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  long x;
  do {
   x = random();
  }
  // This is carefully written not to overflow
  while (num_rand - defect <= (unsigned long)x);

  // Truncated division is intentional
  return x/bin_size;
}


void 
printArray (int *A, int l ,int r)
{
  int i;
  i = 0;
  printf ("\n");
  for (i = l;i <= r; i++){
    printf ("%d ", A[i]);
  }
  printf ("\n");
}

void 
Swap (int* A, int index1, int index2) 
{
  int temp;
  temp = A[index1];
  A [index1] = A [index2];
  A [index2] = temp;
}



void
ReadFileToAdjList (
  vertex **V, // Pointer to the array of vertices to be returned
  edge **E, // Pointer to the array of edges to be returned
  int *numberVertices, // Pointer to the number of vertices to be returned
  int *numberEdges  // Pointer to the number of edges to be returned
  )
{
  int num;
  int mmm; // max number of edges just for alloc

  char ch;
  int ttsize; 
<<<<<<< HEAD
  int collect;
  int newline = 1;

  //char file_name[100] = "";
=======
  long ttInv;
  int nnn; // number of vertices
  short collect;
  short newline;
  int **tt;
  int *ttAlt;
  char file_name[100] = "";
>>>>>>> 4b2c1ad51f38b646562518add6adbc894183c03b
  char *tracker;
  FILE *fp;

  int i;
<<<<<<< HEAD
  int vertexIndex;
  int edgeIndex;
  int edgePool;
=======
  int j;
  int vertexindex;
>>>>>>> 4b2c1ad51f38b646562518add6adbc894183c03b

  vertex *pv; // array of vertices
  edge *pe; // array of edges

  edge *_pie; // current incidented edge
  vertex *pCurrentVertex; // current vertex

/*
  // get file name from user input
  printf ("Type File name:");
  fgets (file_name, 100, stdin);

  tracker = file_name;

  while (*tracker) {
    //printf ("*file_name is  (0x%x)\n", *tracker);
    if (*tracker == 0xA || *tracker == 0xD) {
      //printf ("found new line (0x%x)\n", *tracker);
      *tracker = 0;
      break;
    }
    tracker++;
  }
  // open file
  fp = fopen (file_name, "r");  // read mode
*/
fp = fopen ("k.txt", "r");//hard code file name temporarily
  if (fp == NULL) {
    printf ("Error while opening the file.\n");
    return;
  }

  //
  // alloc vertices and edges array
  //

  mmm = 2516; // mmm <= nnn* (nnn-1) /2 //todo, to make it extensible
  edgeIndex = 0;
  pe = malloc (sizeof(edge) * mmm);
  memset (pe, 0, sizeof(edge) * mmm);
  pv = malloc (sizeof(vertex) * nnn);
  memset (pv, 0, sizeof(vertex)* nnn);

  // alloc pool for edge incidented on the vertex
  //printf ("IVY: alloc pool for edge incidented on the vertex....\n");
  for (i = 0; i < nnn; i++) {
    (pv + i)->connectTo = malloc (sizeof (int)*40);
    memset ((pv + i)->connectTo, 0, sizeof(int)*40);
    //printf ("[Debug]  ");
    //printf ("In (%d)th vertex, edge pool @ (%p) = {%d, %d, %d}\n", i, (pv+i)->connectTo, *((pv+i)->connectTo), *((pv+i)->connectTo + 1), *((pv+i)->connectTo + 2));

  }

  //
  // read file to get graph
  //
  ch = 0;
  i = 0;
  num = 0;
<<<<<<< HEAD
  vertexIndex = 0;
  pCurrentVertex = &pv[vertexIndex];
  pCurrentVertex->degree = 0;
=======
>>>>>>> 4b2c1ad51f38b646562518add6adbc894183c03b
  while (((ch = fgetc (fp)) != EOF)) {

    //
    // If get a number, collect it in num.
    //
    if (ch >= 0x30 && ch <= 0x39) {
      //printf ("_%d_ ", (ch - 0x30));
      num = num * 10 + (ch - 0x30);
      collect = 1;
    } else {
      //printf ("Got non-numeric char!\n");

      /*
      //
      // If array is running out of space,
      // double it before putting anything in it.
      //
<<<<<<< HEAD
      if (vertexIndex >= nnn) {
=======

      if (i >= nnn) {
>>>>>>> 4b2c1ad51f38b646562518add6adbc894183c03b

        //
        // If array size is not enought, reallocate the array of vertices to double its size.
        //
        nnn *=2;
      }

      if (ch == 0xd || ch == 0xa) { // if a new line is read  //todo
        //printf ("\n[DEBUG] new line found...(0x%x)\n", ch);
        newline = 1;
      }
<<<<<<< HEAD

      if (collect == 1) { // if a number is read
        collect = 0;
        //printf (" %d ", num);

        if (newline) { // a newline not yet cleared means this is a new vertex
          newline = 0; // clear the newline flag
/*
if (edgeCount == 0) {
gMaxEdgeIncident = pCurrentVertex->degree;
} else {
if (pCurrentVertex->degree > gMaxEdgeIncident) {
  gMaxEdgeIncident = pCurrentVertex->degree;
}
}
if (vertexIndex - 1 >= 0) {
//printf ("<degree of the (%d)th vertex is (%d) degree>\n", pv[vertexIndex - 1].indexV, pv[vertexIndex-1].degree);
}
*/
          //pCurrentVertex = &pv[vertexIndex]; // advance to next vertex // pv is 0-base
          //put vertex info in the order of vertex index // pv is 0-base
          pCurrentVertex = &pv [num];
          pCurrentVertex->degree = 0; // initialize the degree of this vertex
          vertexIndex++;
        } else {
          // a new edge of the vertex is found
          pCurrentVertex->connectTo[pCurrentVertex->degree] = num;//0-base

          //printf ("vertex (%d)th -> connect with (%d), this is the (%d)th edge of this vertex\n", vertexIndex, pCurrentVertex->inctEdges[pCurrentVertex->degree].v, pCurrentVertex->degree);
          pCurrentVertex->degree++;
          if (vertexIndex < num) { //todo, now ignore self loop edge
            pe[edgeIndex].u = vertexIndex;
            pe[edgeIndex].v = num;
            printf ("  (%d)th edge = (%d, %d)\n", edgeIndex, pe[edgeIndex].u, pe[edgeIndex].v);
            edgeIndex++;

            if (edgeIndex > mmm) {   // todo, double the size
            }
          }
        } // whether or not the num is collected after a new line

        num = 0;
      } // if finish collect a number from reading the file
    } // if-else reading number or non-number
  } // while not the end of file

  *numberVertices = vertexIndex;
  *numberEdges = edgeIndex;
  *V = pv;
  *E = pe;
=======
      */

      //if ((ch <= 0xD && ch >= 0xA) || ch == 0x85 || ch == 0x2028 || ch == 0x2029) {
      if (ch <= 0xD && ch >= 0xA) {
      
        //
        // a line terminator is found,
        // the next num if any should be store in next tt[i]
        //

        if (newline == 0) {
          i++;
          j = 0;
        }
        newline = 1;
      }

      //
      // If a non-numeric char is reached, put num into array.
      //

      if (collect == 1) {
        printf ("%d\n", num);
        *(tt [i] + j) = num;
        num = 0;
        collect = 0;
        newline = 0;

        printf ("tt [%d] + %d (%p) = %d\n", i, j, tt[i]+j, *(tt[i]+j));
        j++;
      }
    }
  }
  ttsize = i;
  //ttsize = 999;
  *pArray = tt;
>>>>>>> 4b2c1ad51f38b646562518add6adbc894183c03b
  
  fclose (fp);
  return;
}

int
RandomContractionAlgo (
  vertex *V,
  edge *E,
  int n,
  int m
  )
{
  int x;
  int vIndex;
  vertex *StayingVertex; // current vertex
  vertex *EatenVertex; // current vertex
  int movingEdgeVertexIndex;
  int remain_n;
  int lastDegree;

  // ***While there are more than 2 vertices:
  remain_n = n;
  while (remain_n > 2) {
    // ***pick a remaining edge (u,v) uniformly at random
    //x = random () %n;
    x = random_at_most(m - 1);
    //printf ("fre, %d vertices left, x=%d\n", remain_n, x);
    // ***merge (or "contract" ) u and v into a single vertex
    if (E[x].u) { // only merge still remaining edges

      //printf ("%d vertices left, going to contract by one vertex, E[%d] = (%d, %d)\n", remain_n, x, E[x].u, E[x].v);
      // add the second index's vertex's connecting vertices to first's
      vIndex = E[x].u;
      StayingVertex = &V[vIndex];
      vIndex = E[x].v;
      EatenVertex = &V[vIndex];

      // connect edges incident on EatenVertex to StayingVertex
      // this means adding edges from EatenVertex to StayingVertex
      //printf ("original (%d)vertex v has degree %d\n", E[x].v, EatenVertex->degree);
      if (EatenVertex->degree <= 0) continue;
      for (int i = 0; i < EatenVertex->degree; i++) {
        movingEdgeVertexIndex = EatenVertex->connectTo[i]; // one edge from EatenVertex

        if (movingEdgeVertexIndex != E[x].u) {
          StayingVertex->connectTo[StayingVertex->degree] = movingEdgeVertexIndex; // contract!!
          StayingVertex->degree++;
          lastDegree = StayingVertex->degree;//???
        }
      }

      EatenVertex->degree = 0;
      E[x].u = 0; // delete this edge by assign 0 in vertex index
      E[x].v = 0; // delete this edge by assign 0 in vertex index

      remain_n--;
    }
    // remove self-loops
    // return cut represented by final 2 vertices.
  }
  return lastDegree;
} // RandomContractionAlgo

int 
main (
  int argc, 
  const char * argv[]
  )
{
  int **input;
  vertex* V;
  edge* E;
  int n, m, k;
  int currentCut;
  int arraySize;

  printf ("(%d) vertices, (%d) edges\n", n, m);

  //
  // input => Adjacency list
  //

  printf ("Start to Count MinCut. n = %d, m = %d\n", n, m);
  k = 0;
  for (int i = 0; i < (nnn*nnn); i++) {
  //for (int i = 0; i < (nnn); i++) {
  ReadFileToAdjList (&V, &E, &n, &m);
    currentCut = RandomContractionAlgo (V, E, n, m);
    printf ("--Cut this time (%d)\n", currentCut);
    if (currentCut < k || k == 0) {
      k = currentCut;
    }
  }

printf ("MinCut = (%d)\n", k);
  for (int i = 0; i < nnn; i++) {
    //printf ("In (%d)th vertex, edge pool @ (%p)\n", i, (V+i)->inctEdges);
    //free ((V+i)->inctEdges); //todo todo!!! why this is wrong...?????
  }
  //free (V);
  //free (E);

  //
  // find minimum cut number.
  //

  return 0;
}
