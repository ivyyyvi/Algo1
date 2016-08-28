#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#include "ProcessInput.h"

#ifdef _DEBUG_PROCESS_INPUT
#define DEBUG_PROCESS_INPUT(format, args...) printf("[%s:%s:%d] \t"format, __FILE__, __FUNCTION__, __LINE__, ##args)
#else
#define DEBUG_PROCESS_INPUT(args...)
#endif


#ifdef _DEBUG_DFS
#define DEBUG_DFS(format, args...) printf("[%s:%s:%d] \t"format, __FILE__, __FUNCTION__, __LINE__, ##args)
#else
#define DEBUG_DFS(args...)
#endif

#ifdef _DEBUG_MAIN
#define DEBUG_MAIN(format, args...) printf("[%s:%s:%d] \t"format, __FILE__, __FUNCTION__, __LINE__, ##args)
#else
#define DEBUG_MAIN(args...)
#endif

#ifdef _DEBUG
#define DEBUG(format, args...) printf("[%s:%s:%d] \t"format, __FILE__, __FUNCTION__, __LINE__, ##args)
#else
#define DEBUG(args...)
#endif

typedef struct _vertex vertex;
typedef struct _edge edge;

//
// Function prototypes
//

int
ReadInput (edge *E, int *numEdge, int *numVertices);

int
Dijkstra (int *A, int num_nodes, edge* E, int num_edges);

/*
int
PopEnd (int *wholedeal, int numTotal);

int
first (int *wholedeal, int numTotal);

void
append (int vertex_to_append, int *vertices_to_visit, int num_vertices_to_visit);

void
prepend (int vertex_to_prepend, int *vertices_to_visit, int num_vertices_to_visit);

void
takeout (int vertex_to_takeout, int *vertices, int num_vertices);
*/

//
// Struct definitions
//

struct _vertex {
  int index;
  int degree;
  int Explored;
  int leader;
  int parentVertex; // parent_add_it_to_vertices_to_visit
  int *connectTo;
};

struct _edge {
  char Explored;
  int tailIndex; // start
  int headIndex; // end
  int weight; // length
};

struct _graph {
  vertex *V;
};

