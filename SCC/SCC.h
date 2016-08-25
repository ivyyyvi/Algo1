#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>


extern int nnn;


#define ASK_FOR_INPUT 0
//#define DEFAULT_INPUT_FILENAME "sinput3.txt"
//#define DEFAULT_INPUT_FILENAME "input.txt"
//#define DEFAULT_INPUT_FILENAME "HalfInput.txt"

#ifdef DEFAULT_INPUT_FILENAME_A
  #define DEFAULT_INPUT_FILENAME "testInput003.txt"
#endif
#ifdef DEFAULT_INPUT_FILENAME_B
  #define DEFAULT_INPUT_FILENAME "sinput0.txt"
#endif

//#define _DEBUG_PROCESS_INPUT

#ifdef _DEBUG_PROCESS_INPUT
#define DEBUG_PROCESS_INPUT(format, args...) printf("[%s:%d] "format, __FILE__, __LINE__, ##args)
#else
#define DEBUG_PROCESS_INPUT(args...)
#endif

#define _DEBUG_DFS

#ifdef _DEBUG_DFS
#define DEBUG_DFS(format, args...) printf("[%s:%d] "format, __FILE__, __LINE__, ##args)
#else
#define DEBUG_DFS(args...)
#endif

#define _DEBUG

#ifdef _DEBUG
#define DEBUG(format, args...) printf("[%s:%d] "format, __FILE__, __LINE__, ##args)
#else
#define DEBUG(args...)
#endif

typedef struct _vertex vertex;
typedef struct _edge edge;

//
// Function prototypes
//

int
ReadFileToAdjList (
  vertex **V, // Pointer to the array of vertices to be returned
  vertex **rV,
  int *numberVertices // Pointer to the number of vertices to be returned
  );

int
DFS_Loop (
  int which_pass,
  vertex *V, // Pointer to the array of vertices to be returned
  int numberVertices, // Pointer to the number of vertices to be returned
  int *index_sequence_by_finish_time,
  int *index_sequence_by_finish_time_2,
  int *leader_group //int *leaderContainingNodes
  );

//
// Struct definitions
//


struct _vertex {
  int index;
  int degree;
  int Explored;
  int leader;
  int parent_add_it_to_vertices_to_visit;
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
