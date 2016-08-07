
//
//  SCC.c
//  Algo
//
//  Created by Ivy on 8/7/2016
//  Copyright (c) 2016 ___IVYCANT___. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ASK_FOR_INPUT 0
#define DEFAULT_INPUT_FILENAME "input2.txt"

#define _DEBUG

#ifdef _DEBUG
#define DEBUG(format, args...) printf("[%s:%d] "format, __FILE__, __LINE__, ##args)
#else
#define DEBUG(args...)
#endif

int nnn = 875715; // max number of vertices just for alloc // this can be doubled if not enough 

typedef struct _vertex vertex;
typedef struct _edge edge;
typedef struct _graph graph;

struct _vertex {
  int index;
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

void
ReadFileToAdjList (
  vertex **V, // Pointer to the array of vertices to be returned
  edge **E, // Pointer to the array of edges to be returned
  int *numberVertices, // Pointer to the number of vertices to be returned
  int *numberEdges  // Pointer to the number of edges to be returned
  )
{
  int num;
  long int mmm; // max number of edges just for alloc
  int degreeMax = 5;

  char ch;
  int ttsize; 
  int collect;
  int newline = 1;

  char file_name[100] = DEFAULT_INPUT_FILENAME;
  char *tracker;
  FILE *fp;

  int i;
  int vertexIndex;
  int edgeIndex;
  int edgePool;

  vertex *pv; // array of vertices
  edge *pe; // array of edges

  edge *_pie; // current incidented edge
  vertex *pCurrentVertex; // current vertex

#if ASK_FOR_INPUT
  // get file name from user input
  memset (file_name, 0, sizeof(file_name));//todo
  DEBUG ("Type File name:");
  fgets (file_name, 100, stdin);

  tracker = file_name;

  while (*tracker) {
    DEBUG ("*file_name is  (0x%x)\n", *tracker);
    if (*tracker == 0xA || *tracker == 0xD) {
      DEBUG ("found new line (0x%x)\n", *tracker);
      *tracker = 0;
      break;
    }
    tracker++;
  }
#endif
  // open file
  fp = fopen (file_name, "r");  // read mode

  if (fp == NULL) {
    DEBUG ("Error while opening the file.\n");
    return;
  }

  //
  // alloc vertices and edges array
  //

  mmm = 5105043;
  edgeIndex = 0;
  pv = malloc (sizeof(vertex) * nnn);
  memset (pv, 0, sizeof(vertex)* nnn);
  pe = malloc (sizeof(edge) * mmm);
  memset (pe, 0, sizeof(edge) * mmm);

  // alloc pool for edge incidented on the vertex
  //DEBUG ("IVY: alloc pool for edge incidented on the vertex....\n");
  for (i = 0; i < nnn; i++) {
    (pv + i)->connectTo = malloc (sizeof (int)*(degreeMax));
    memset ((pv + i)->connectTo, 0, sizeof(int)*(degreeMax));
  }

  //
  // read file to get graph
  //
  ch = 0;
  i = 0;
  num = 0;
  vertexIndex = 0;
  pCurrentVertex = &pv[vertexIndex];
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
      if (vertexIndex >= nnn) {

        //
        // If array size is not enough, double the array size.
        //
        DEBUG ("-------vertice pool not enough! (%d)\n", degreeMax);
        nnn *=2;
        free (pv);
        pv = malloc (sizeof (vertex) * nnn);
      }

      if (ch == 0xd || ch == 0xa) { // if a new line is read  //todo
        //DEBUG ("\n[DEBUG] new line found...(0x%x)\n", ch);
        newline = 1;
      }

      if (collect == 1) { // if a number is read
        collect = 0;
        //DEBUG ("Got non-numeric char!");
        //DEBUG (" %d ", num);

        if (newline) { // a newline not yet cleared means this is a new EDGE 
          newline = 0; // clear the newline flag

          // every line in the input file is an edge (u, v)
          // after a new line, the number read is the vertex index
          // so switch to that vertex
          pCurrentVertex = &pv [num];
          pCurrentVertex->index = num;
          //pCurrentVertex->degree++;
        } else {

          // a new edge of the vertex is found
          if (pCurrentVertex->degree >= degreeMax) {
            DEBUG ("-------degreeMax (%d) not enough!\n", degreeMax);
            degreeMax *= 2;
            for (i = 0; i < nnn; i++) {
              free ((pv + i)->connectTo);
              (pv + i)->connectTo = malloc (sizeof (int)*(degreeMax));
              memset ((pv + i)->connectTo, 0, sizeof(int)*(degreeMax));
            }
          }
          pCurrentVertex->connectTo [pCurrentVertex->degree] = num;//0-base
          pCurrentVertex->degree++;

          if (edgeIndex > mmm) {   // todo, double the size
            DEBUG ("-------edge pool not enough! (%ld)\n", mmm);
            free (pe);
            mmm *= 2;
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

  *numberVertices = vertexIndex;
  *numberEdges = edgeIndex;
  *V = pv;
  *E = pe;
  
  fclose (fp);
  return;
}

int DFS_Loop ()
{
  return 0;
} 

int main ()
{
  char file_name[100] = DEFAULT_INPUT_FILENAME;
  vertex *_V; // Pointer to the array of vertices to be returned
  edge *_E; // Pointer to the array of edges to be returned
  int _numberVertices; // Pointer to the number of vertices to be returned
  int _numberEdges; // Pointer to the number of edges to be returned
  //graph G;
  DEBUG ("char array content: %s\n", DEFAULT_INPUT_FILENAME);
  DEBUG ("char array file_name size (%ld)\n", sizeof(file_name));

  ReadFileToAdjList (
    &_V, // Pointer to the array of vertices to be returned
    &_E, // Pointer to the array of edges to be returned
    &_numberVertices, // Pointer to the number of vertices to be returned
    &_numberEdges  // Pointer to the number of edges to be returned
    );

  DFS_Loop (); 


  return 0;
}
