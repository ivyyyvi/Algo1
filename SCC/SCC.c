
//
//  SCC.c
//  Algo
//
//  Created by Ivy on 8/7/2016
//  Copyright (c) 2016 ___IVYCANT___. All rights reserved.
//

//todo how to get correct finishing time?


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ASK_FOR_INPUT 0
#define DEFAULT_INPUT_FILENAME "sinput0.txt"

#define _DEBUG

#ifdef _DEBUG
#define DEBUG(format, args...) printf("[%s:%d] "format, __FILE__, __LINE__, ##args)
#else
#define DEBUG(args...)
#endif

int nnn = 875715; // max number of vertices just for alloc // this can be doubled if not enough 

typedef struct _vertex vertex;
typedef struct _edge edge;
//typedef struct _graph graph;
//typedef struct _listClass listClass;
//typedef struct _stackClass stackClass;

struct _vertex {
  int index;
  int finish_time;
  int degree;
  int Explored;
  int leader;
  int *connectTo;
};

struct _edge {
  int u; // vertex index 1st endpoint
  int v; // vertex index 2nd endpoint
};

struct _graph {
  vertex *V;
};

/*struct _listClass {
  vertex* vertex_list;
  vertex* (*first) (listClass *list);
};*/

int
ReadFileToAdjList (
  vertex **V, // Pointer to the array of vertices to be returned
  vertex **rV,
  edge **E, // Pointer to the array of edges to be returned
  int *numberVertices, // Pointer to the number of vertices to be returned
  int *numberEdges  // Pointer to the number of edges to be returned
  )
{
  int num;
  int num_max = 0;
  long int mmm; // max number of edges just for alloc
  long int mmm_reverse; // max number of edges just for alloc
  int degreeMax = 5;

  char ch;
  int ttsize; 
  int collect;
  int edgeStart = 1;

  char file_name[100] = DEFAULT_INPUT_FILENAME;
  char *tracker;
  FILE *fp;

  int i;
  int vertexCount;
  int edgeIndex;
  int edgeIndex_reverse;
  int edgePool;

  vertex *pv; // array of vertices
  edge *pe; // array of edges
  vertex *pv_reverse; // array of vertices
  edge *pe_reverse; // array of edges

  edge *_pie; // current incidented edge
  vertex *pCurrentVertex; // current vertex
  vertex *pCurrentVertex_reverse; // current vertex

#if ASK_FOR_INPUT
  // get file name from user input
  memset (file_name, 0, sizeof(file_name));//todo
  DEBUG ("Type File name:");
  fgets (file_name, 100, stdin);

  tracker = file_name;

  while (*tracker) {
    //DEBUG ("*file_name is  (0x%x)\n", *tracker);
    if (*tracker == 0xA || *tracker == 0xD) {
      //DEBUG ("found new line (0x%x)\n", *tracker);
      *tracker = 0;
      break;
    }
    tracker++;
  }
#endif
  // open file
  fp = fopen (file_name, "r");  // read mode

  if (fp == NULL) {
    //DEBUG ("Error while opening the file.\n");
    return -1;
  }

  //
  // alloc vertices and edges array
  //

  mmm = 5105043;
  mmm_reverse = mmm;
  edgeIndex = 0;
  edgeIndex_reverse = 0;
   
  pv = malloc (sizeof(vertex) * nnn);
  memset (pv, 0, sizeof(vertex)* nnn);
  pe = malloc (sizeof(edge) * mmm);
  memset (pe, 0, sizeof(edge) * mmm);

  pv_reverse = malloc (sizeof(vertex) * nnn);
  memset (pv_reverse, 0, sizeof(vertex)* nnn);
  pe_reverse = malloc (sizeof(edge) * mmm_reverse);
  memset (pe_reverse, 0, sizeof(edge) * mmm_reverse);
  // alloc pool for edge incidented on the vertex
  //DEBUG ("IVY: alloc pool for edge incidented on the vertex....\n");
  for (i = 0; i < nnn; i++) {
    (pv + i)->connectTo = malloc (sizeof (int)*(degreeMax));
    memset ((pv + i)->connectTo, 0, sizeof(int)*(degreeMax));
    (pv_reverse + i)->connectTo = malloc (sizeof (int)*(degreeMax));
    memset ((pv_reverse + i)->connectTo, 0, sizeof(int)*(degreeMax));
  }

  //
  // read file to get graph
  //
  ch = 0;
  i = 0;
  num = 0;
  vertexCount = 0;
  while (((ch = fgetc (fp)) != EOF)) {

    //
    // If get a number, collect it in num.
    //
    if (ch >= 0x30 && ch <= 0x39) {
      //DEBUG ("_%d_ \n", (ch - 0x30));
      num = num * 10 + (ch - 0x30);
      collect = 1;
    } else {

      //
      // If array is running out of space,
      // double it before putting anything in it.
      //
      if (vertexCount >= nnn) {

        //
        // If array size is not enough, double the array size.
        //
        //DEBUG ("-------vertice pool not enough! (%d)\n", degreeMax);
        nnn *= 2;
        free (pv);
        pv = malloc (sizeof (vertex) * nnn);
        free (pv_reverse);
        pv_reverse = malloc (sizeof (vertex) * nnn);
      } // if vertex array is not big enough

      if (collect == 1) { // if a number is read
        collect = 0;
        
        //
        // use max of num saw to be the vertices number
        // this assumes vertices index are continual no skipping
        //
        if (num > num_max) {
          num_max = num;
        }

        if (edgeStart) {
          edgeStart = 0;

          // the num collected here is the start (tail) index of vertex of an edge
          pCurrentVertex = &pv [num];
          pCurrentVertex->index = num;

          //
          // if edge pool not big enough, double it
          //
          if (edgeIndex_reverse > mmm_reverse) {
            //DEBUG ("-------edge pool not enough! (%ld)\n", mmm);
            mmm_reverse *= 2;
            free (pe_reverse);
            pe_reverse = malloc (sizeof(edge) * mmm_reverse);
            memset (pe_reverse, 0, sizeof(edge) * mmm_reverse);
          }
          // for reverse G, the num collected here is the end (head) of vertex of an edge
          pe_reverse[edgeIndex_reverse].v = num;

          
        } else { 
          edgeStart = 1;

          //
          // finish collecting an edge's two vertices
          //

          //
          // if degreeMax not big enough, double it
          //
          if (pCurrentVertex->degree >= degreeMax) {
            //DEBUG ("-------degreeMax (%d) not enough!\n", degreeMax);
            degreeMax *= 2;
            for (i = 0; i < nnn; i++) {
              free ((pv + i)->connectTo);
              (pv + i)->connectTo = malloc (sizeof (int)*(degreeMax));
              memset ((pv + i)->connectTo, 0, sizeof(int)*(degreeMax));
              free ((pv_reverse + i)->connectTo);
              (pv_reverse + i)->connectTo = malloc (sizeof (int)*(degreeMax));
              memset ((pv_reverse + i)->connectTo, 0, sizeof(int)*(degreeMax));
            }
          } // if degree Max not big enough // todo, what about the reverse

          //
          // the num here collected is the end (head) of an edge
          //
          pCurrentVertex->connectTo [pCurrentVertex->degree] = num;//0-base
          pCurrentVertex->degree++;

          //
          // for reverse G, the num collected is the start (tail) of an edge
          //
          pCurrentVertex_reverse = &pv_reverse [num];
          pCurrentVertex_reverse->index = num;
          // already knew the end (head) of the edge, now fill it in.
          pCurrentVertex_reverse->connectTo [pCurrentVertex_reverse->degree] = pe_reverse[edgeIndex_reverse].v;
          pe_reverse[edgeIndex_reverse].u = num;
          DEBUG ("  (%d)th reverse edge = (%d, %d)\n", edgeIndex_reverse, pe_reverse[edgeIndex_reverse].u, pe_reverse[edgeIndex_reverse].v);
          edgeIndex_reverse++;
          pCurrentVertex_reverse->degree++;
          

          //
          // if edge pool not big enough, double it
          //
          if (edgeIndex > mmm) {
            //DEBUG ("-------edge pool not enough! (%ld)\n", mmm);
            mmm *= 2;
            free (pe);
            pe = malloc (sizeof(edge) * mmm);
            memset (pe, 0, sizeof(edge) * mmm);
          }

          pe[edgeIndex].u = pCurrentVertex->index;
          pe[edgeIndex].v = num;
          DEBUG ("  (%d)th edge = (%d, %d)\n", edgeIndex, pe[edgeIndex].u, pe[edgeIndex].v);
          edgeIndex++;
        } // whether or not the num is collected after a new line

        num = 0;
      } // if finish collect a number from reading the file
    } // if-else reading number or non-number
  } // while not the end of file

  //*numberVertices = vertexCount;
  *numberVertices = num_max;
  *numberEdges = edgeIndex;
  *V = pv;
  *rV = pv_reverse;
  *E = pe;
  
  fclose (fp);
  return 0;
}

int
first (int *wholedeal, int numTotal)
{
  int vertex_first;
  int forloop_upperlimit;

  vertex_first = *wholedeal;
  // remove
  forloop_upperlimit = numTotal - 1; // only need to copy 
  for (int i = 0; i < forloop_upperlimit; i++) {
    //*(wholedeal + i + 1) = *(wholedeal + i);
    *(wholedeal + i) = *(wholedeal + i + 1);
  }

  return vertex_first;
}; // removes and returns the first element in the list.

void
prepend (int node_to_prepend, int *vertices_to_visit, int num_vertices_to_visit)
{
  int offset;
  offset = 1;

  //
  // the index in the bigger address is moved first
  //
  for (int i = num_vertices_to_visit - 1; i >= 0; i--) {

    //
    // unnecessarily using memcpy. its clearer. so. whatever.
    //
    memcpy (
      &vertices_to_visit[i] + offset, // dest // move to bigger address, offset 
      &vertices_to_visit[i], // src
      sizeof (int)); // number of bytes to copy
  }

  //
  // prepend (of course) to the start of the stack
  //
  *vertices_to_visit = node_to_prepend;
  return;
}

int DFS_Loop (
  vertex *V, // Pointer to the array of vertices to be returned
  int numberVertices, // Pointer to the number of vertices to be returned
  int numberEdges  // Pointer to the number of edges to be returned
  )
{
  int lengthNV;
  int num_vertices_to_visit = 0;
  int currentLeaderIndex;
  int currentIndex;
  int currentChildIndex;
  vertex *currentVertex;
  vertex *currentChildVertex;
  int *vertices_to_visit; // only keep index of vertices
  int notYetFinish;
  int ttt = 1;

  //
  // Initialize vertices_to_visit stack
  //
  lengthNV = 500; // todo, double this size when not enough
  vertices_to_visit = malloc (sizeof (int) * lengthNV);
  memset (vertices_to_visit, 0, sizeof (int) * lengthNV);

  //
  // Traverse the entire graph to call DFS from
  //
  for (int i = numberVertices; i > 0; i--) {

    //
    // if the vertex is already explored, go to the next vertex
    //
    if (V[i].Explored) {
      DEBUG ("V[%d] is already explored\n", i);
      continue;
    }

    //
    // if the vertex is not yet explored, mark it as explored
    //
    DEBUG ("V[%d] is not yet explored\n", i);
  
    //
    // init vertices_to_visit with (index of) root
    //
    *vertices_to_visit = i;
    num_vertices_to_visit++;

    //
    // Bookkeeping leader vertex for 2nd pass DFS_Loop.
    //
    currentLeaderIndex = i;   

    DEBUG ("Outer for loop @ %d (leader)\n", i);
    //
    // while the vertices_to_visit is not empty
    //
    while (num_vertices_to_visit) {
      
      //
      // pop a vertex from the vertices_to_visit stack;
      // and delete the vertex from the vertices_to_visit stack // todo, check if this step should be here
      //
      currentIndex = first (vertices_to_visit, num_vertices_to_visit);
      currentVertex = &V[currentIndex];
      num_vertices_to_visit--;
      DEBUG ("Pop (%d) from stack\n", currentIndex);

      //
      // if the vertex is not yet explored, mark as explored, and look at its children
      //
      if (currentVertex->Explored == 0) {

        DEBUG ("..And this vertex not yet explored. I mean (%d) who got degree (%d)\n", currentIndex, currentVertex->degree);
        currentVertex->Explored = 1; // mark as explored
        currentVertex->leader = currentLeaderIndex; // set leader

        //
        // Traverse children vertices, 
        // if none of the children is unexplored, count the finishing time of the currentVertex
        //
        //for (int j = 0; j < currentVertex->degree; j++) {
        notYetFinish = 0;
        for (int j = currentVertex->degree - 1;j >= 0; j--) { // to mimic class note
          currentChildIndex = currentVertex->connectTo[j];
          currentChildVertex = &V[currentChildIndex];
          DEBUG ("  (%d)'s child (%d)\n", currentIndex, currentChildIndex);

          //
          // if the child vertex is not yet explored, prepend it to vertices_to_visit.
          // todo, should I mark as explored here?
          // I think NO. It is when its child vertice are about to be explored, when it is marked explored.
          //
          if (currentChildVertex->Explored == 0) {
            notYetFinish = 1; // having child that is not yet explored means this vertex is not finished exploring
            prepend (currentChildIndex, vertices_to_visit, num_vertices_to_visit);
            num_vertices_to_visit++;
            DEBUG ("  ..And this child is not yet explored. prepended it to vertices_to_visit...\n");
#ifdef _DEBUG
            printf ("        => vertices_to_visit: [ ");
            for (int k = 0; k < num_vertices_to_visit; k++) {
              printf ("%d ", vertices_to_visit[k]);
            } printf ("]\n\n");
#endif
          } // if the child is not explored
          else {
            //
            // It is time to "giu" back and count finish time!
            //
            //printf ("  -- here?? finish on (%d) *****\n", currentChildIndex);
          }
        } // for all children
        
        //
        // Check if this vertex is finished. If yes, count finishing time.
        //

        if (notYetFinish == 0) {
          //
          // Or this? It is time to "giu" back and count finish time!
          //
          currentVertex->finish_time = ttt;
          printf ("  -- here?? finish on f(%d) = (%d) *****\n", currentIndex, ttt);
          ttt++;
        }
      } // if current vertex is not yet explored. else {}
    } // while
  } // for
  return 0;
} 

void
CopyFinishTime ()
{
}

vertex *
Reverse (vertex *V)
{
  return NULL;
}

int main ()
{
  char file_name[100] = DEFAULT_INPUT_FILENAME;
  vertex *_V;
  vertex *_rV;
  edge *_E;
  int _numberVertices;
  int _numberEdges;
  //graph G;
  //DEBUG ("char array content: %s\n", DEFAULT_INPUT_FILENAME);
  //DEBUG ("char array file_name size (%ld)\n", sizeof(file_name));

  if (0 != ReadFileToAdjList (
             &_V,
             &_rV,
             &_E,
             &_numberVertices,
             &_numberEdges
             )) {
    return -1;
  }

  DEBUG ("1st pass DFS_Loop, #v (%d), #e (%d)\n", _numberVertices, _numberEdges);
  DFS_Loop (_rV, _numberVertices, _numberEdges); 
  CopyFinishTime ();
  //DFS_Loop ();


  return 0;
}
