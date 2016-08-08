
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
#include <time.h>

#define ASK_FOR_INPUT 0
#define DEFAULT_INPUT_FILENAME "sinput3.txt"
//#define DEFAULT_INPUT_FILENAME "input.txt"


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

int
ReadFileToAdjList (
  vertex **V, // Pointer to the array of vertices to be returned
  vertex **rV,
  int *numberVertices // Pointer to the number of vertices to be returned
  )
{
  int num;
  int temp_head;
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
  int edgePool;

  vertex *pv; // array of vertices
  vertex *pv_reverse; // array of vertices

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
   
  pv = calloc (nnn, sizeof(vertex));

  pv_reverse = calloc (nnn, sizeof(vertex));

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

          // for reverse G, the num collected here is the end (head) of vertex of an edge
          temp_head = num;
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
          pCurrentVertex_reverse->connectTo [pCurrentVertex_reverse->degree] = temp_head;
          pCurrentVertex_reverse->degree++;
        } // whether or not the num is collected after a new line

        num = 0;
      } // if finish collect a number from reading the file
    } // if-else reading number or non-number
  } // while not the end of file

  //*numberVertices = vertexCount;
  *numberVertices = num_max;
  *V = pv;
  *rV = pv_reverse;
  
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
prepend (int vertex_to_prepend, int *vertices_to_visit, int num_vertices_to_visit)
{
  int offset = 1;

  //
  // the index in the bigger address is moved first
  //
  for (int i = num_vertices_to_visit - 1; i >= 0; i--) {
    *(vertices_to_visit + i + offset) = *(vertices_to_visit + i);
  }

  //
  // prepend (of course) to the start of the stack
  //
  *vertices_to_visit = vertex_to_prepend;
  return;
}

void
takeout (int vertex_to_takeout, int *vertices_visited, int num_vertices_visited)
{
  for (int ppp = 0; ppp < num_vertices_visited; ppp++) {
    if (vertices_visited[ppp] == vertex_to_takeout) {
      
      //
      // move everything after this to one slot smaller place
      // do it from smaller address
      //
      for (int qqq = ppp + 1; qqq < num_vertices_visited; qqq++) {
        vertices_visited [qqq - 1] = vertices_visited [qqq];
      }
      break;
    }
  }
}
int DFS_Loop (
  int which_pass,
  vertex *V, // Pointer to the array of vertices to be returned
  int numberVertices, // Pointer to the number of vertices to be returned
  int *index_sequence_by_finish_time,
  int *index_sequence_by_finish_time_2,
  int *leader_group //int *largestFiveLeader
  )
{
  int lengthNV;
  int num_vertices_to_visit = 0;
  int num_vertices_visited = 0;
  int currentOuterForLoopVertexIndex;
  int currentLeaderIndex;
  int currentIndex;
  int takeoutIndex;
  int currentChildIndex;
  vertex *currentVertex;
  vertex *currentChildVertex;
  int *vertices_to_visit; // only keep index of vertices

  int ttt = 1;
  int notYetFinish;
  // only for 1st pass to compute finish_time, in particular, its sequence
  int *vertices_visited;

  //
  // Initialize vertices_to_visit stack
  //
  lengthNV = 500; // todo, double this size when not enough
  vertices_to_visit = malloc (sizeof (int) * lengthNV);
  memset (vertices_to_visit, 0, sizeof (int) * lengthNV);

  //if (which_pass == 1) {
    vertices_visited = malloc (sizeof (int) * lengthNV);
    memset (vertices_visited, 0, sizeof (int) * lengthNV);

  //
  // Traverse the entire graph to call DFS from
  // Note that 1st pass and 2nd pass DFS_Loop is using different sequence
  // 1st pass uses decresing order of vertex.index
  // 2nd pass uses decresing order of vertex index stored in index_sequence_by_finish_time_2
  //
  for (int sss = numberVertices; sss > 0; sss--) {

    switch (which_pass) {
      case 1:
        currentOuterForLoopVertexIndex = sss;
        break;
      case 2:
        currentOuterForLoopVertexIndex = index_sequence_by_finish_time_2[sss];
      //DEBUG ("This is 2nd pass.%d th vertices is (%d)\n", sss, currentOuterForLoopVertexIndex);
        break;
      default:
        break;
    }

    //
    // if the vertex is already explored, go to the next vertex
    //
    if (V[currentOuterForLoopVertexIndex].Explored) {
      //DEBUG ("V[%d] is already explored\n", currentOuterForLoopVertexIndex);
      continue;
    }

    //
    // if the vertex is not yet explored, mark it as explored
    //
    //DEBUG ("V[%d] is not yet explored\n", currentOuterForLoopVertexIndex);
  
    //
    // init vertices_to_visit with (index of) root
    //
    *vertices_to_visit = currentOuterForLoopVertexIndex;
    num_vertices_to_visit++;

    //
    // Bookkeeping leader vertex for 2nd pass DFS_Loop.
    //
    currentLeaderIndex = currentOuterForLoopVertexIndex;

    //DEBUG ("Outer for loop @ %d (leader)\n", currentOuterForLoopVertexIndex);
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
      //DEBUG ("Pop (%d) from stack\n", currentIndex);

      //
      // if the vertex is not yet explored, mark as explored, and look at its children
      //
      if (currentVertex->Explored == 0) {

DEBUG ("%d pass Explore (%d)\n", which_pass, currentIndex);
        //DEBUG ("..And this vertex not yet explored. I mean (%d) who got degree (%d)\n", currentIndex, currentVertex->degree);
        currentVertex->Explored = 1; // mark as explored

        currentVertex->leader = currentLeaderIndex; // set leader
        
        //
        // leader group
        //
        leader_group[currentLeaderIndex]++;
        if (which_pass == 2) 
  {DEBUG ("(%d)'s leader(%d) has total#(%d) \n", currentIndex, currentLeaderIndex, leader_group[currentLeaderIndex]);}

        prepend (currentIndex, vertices_visited, num_vertices_visited);
        num_vertices_visited++;

#ifdef _DEBUG
        //printf ("        => Trace of visited vertex: { ");
        //for (int ppp = 0; ppp < num_vertices_visited; ppp++) {
          //printf ("%d ", vertices_visited[ppp]);
        //}
        //printf ("}\n");
#endif

        //
        // Traverse children vertices, 
        // if none of the children is unexplored, count the finishing time of the currentVertex
        //
        notYetFinish = 0;
        //for (int j = currentVertex->degree - 1;j >= 0; j--) { // to mimic class note
        for (int j = 0; j < currentVertex->degree; j++) { // different with class note
          currentChildIndex = currentVertex->connectTo[j];
          currentChildVertex = &V[currentChildIndex];
          currentChildVertex->parent_add_it_to_vertices_to_visit = currentIndex;
          //DEBUG ("  (%d)'s child (%d)\n", currentIndex, currentChildIndex);

          //
          // if the child vertex is not yet explored, prepend it to vertices_to_visit.
          // todo, should I mark as explored here?
          // I think NO. It is when its child vertice are about to be explored, when it is marked explored.
          //
          if (currentChildVertex->Explored == 0) {
            notYetFinish = 1; // having child that is not yet explored means this vertex is not finished exploring
            prepend (currentChildIndex, vertices_to_visit, num_vertices_to_visit);
            num_vertices_to_visit++;
            //DEBUG ("  ..And this child is not yet explored. prepended it to vertices_to_visit...\n");
            if (num_vertices_to_visit == lengthNV) {
              printf ("vertices_to_visit is not big enough.\n");
            }

#ifdef _DEBUG
            //printf ("        => vertices_to_visit: [ ");
            //for (int k = 0; k < num_vertices_to_visit; k++) {
              //printf ("%d ", vertices_to_visit[k]);
            //} printf ("]\n\n");
#endif
          } // if the child is not explored
          else {
            //
            // It is time to "giu" back and count finish time!
            //
            //printf ("  -- here?? finish on (%d) *****\n", currentChildIndex);
          }
        } // for all children
        
        //if (which_pass == 1) {

          //
          // Check if this vertex is finished. If yes, count finishing time.
          //

          if (notYetFinish == 0) {
            
            //
            // Alright, currentVertex finished its job!
            //

            //
            // Let's take it out from the visited stack
            //
            takeout (currentIndex, vertices_visited, num_vertices_visited);
            num_vertices_visited--;

            //
            // To compute the correct finish_time
            // before giving awards (finishing time) to currentVertex,
            // we should check if 
            //   currentIndex is awarded ...
            //   ... when it is living before its parent who add it to vertices_to_visit before it was taken out
            //   i.e. is its parent now sit at the very beginning of the vertices_visited
            // if not, take out all that block its parent's way, awards the taken out ones first
            // child need to be before its parent in the vertices_visited stack world. 
            // Or it would on a wrong place to award them! Its parent needs to see.
            //
            //DEBUG ("Who is at front seat of vertices_visited when (%d) is awarded finish_time ...?\n", currentIndex);
            if (vertices_visited[0] == currentVertex->parent_add_it_to_vertices_to_visit) {
              //DEBUG ("oh yeah. it is its parent! good!\n");
            } else {
              //DEBUG ("Well, not currentVertex's parent (%d) instead it is ..\n", currentVertex->parent_add_it_to_vertices_to_visit);
              for (int xxx = 0; num_vertices_visited > 0;) {

                if (vertices_visited[xxx] != currentVertex->parent_add_it_to_vertices_to_visit) {

                  //
                  // Remember to award (give finish_time to) them
                  //
                  index_sequence_by_finish_time [ttt] = vertices_visited[xxx]; //index_se... is 1-based
#ifdef _DEBUG
                  //printf ("  -3- here?? finish on f(%d) = (%d) *****\n\n", vertices_visited[xxx], ttt);
#endif
                  ttt++;

                  //
                  // Take it out of vertices_visited
                  //
                  takeout (vertices_visited[xxx], vertices_visited, num_vertices_visited);  
                  num_vertices_visited--;
                  

                } else {
#ifdef _DEBUG
                  //printf ("\n");
#endif
                  break;
                }
              } // for
            } // if-else

            //
            // Or this? It is time to "giu" back and count finish time!
            //
            index_sequence_by_finish_time [ttt] = currentIndex; //index_se... is 1-based
#ifdef _DEBUG
            //printf ("  -1- here?? finish on f(%d) = (%d) *****\n\n", currentIndex, ttt);
#endif
            ttt++;

          }
        //} // if 1st pass

      } // if current vertex is not yet explored. else {}
    } // while

    //if (which_pass == 1) {
      //
      // Finished this one outer for loop, Otsukaresama!
      // Give rewards to these cute little vertices who haven't got it (still in vertices_visited stack)
      // by counting their finishing time.
      // the smaller index in vertices_visited, the shorter its finishing time.
      //
      for (int ppp = 0; num_vertices_visited > 0; ppp++) { // todo, can this work?

          takeoutIndex = vertices_visited [0];
          //
          // Also take it out from the visited stack
          //
          takeout (takeoutIndex, vertices_visited, num_vertices_visited);
          num_vertices_visited--;

          //
          // Or this? It is time to "giu" back and count finish time!
          //
          index_sequence_by_finish_time [ttt] = takeoutIndex; //index_se... is 1-based
#ifdef _DEBUG
          //printf ("  -2- here?? finish on f(%d) = (%d) *****\n\n", takeoutIndex, ttt);
#endif
          ttt++;

      } // for loop to give finishing time
    //} // if 1st pass
  } // for
  free (vertices_visited);
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


int compare_ints(const void* a, const void* b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
 
    // return (arg1 > arg2) - (arg1 < arg2); // possible shortcut
    // return arg1 - arg2; // erroneous shortcut (fails if INT_MIN is present)
}

int main ()
{
  char file_name[100] = DEFAULT_INPUT_FILENAME;
  vertex *_V;
  vertex *_rV;
  int _numberVertices;
  int *reverse_seq_for_secondpass;
  int *intArray;
  int *largestFiveLeader;

  double time_spent;
  clock_t end;
  clock_t begin = clock();

  DEBUG ("Start reading girl~\n");
  if (0 != ReadFileToAdjList (
             &_V,
             &_rV,
             &_numberVertices
             )) {
    return -1;
  }

  DEBUG ("Calling 1st pass DFS_Loop, #v (%d)\n", _numberVertices);

  reverse_seq_for_secondpass = malloc (sizeof (int) * (_numberVertices + 1)); // 1-based
  intArray = malloc (sizeof (int) * (_numberVertices + 1)); // 1-based
  reverse_seq_for_secondpass [0] = 0; // this slot is not used
  largestFiveLeader = malloc (sizeof (int) * (_numberVertices + 1));
  DFS_Loop (1, _rV, _numberVertices, reverse_seq_for_secondpass, intArray, largestFiveLeader); 
  //CopyFinishTime ();
#ifdef _DEBUG
  printf ("reverse_seq_for_secondpass is: [ ");
  for (int i = 1; i <= _numberVertices; i++) {
    printf ("%d ", reverse_seq_for_secondpass [i]);
  }
  printf ("]\n");
#endif

  free (_rV);
  
  //
  // Processing vertices in decreasing order of finishing times
  //
  memset (largestFiveLeader, 0, sizeof (int) * (_numberVertices + 1));
  memcpy (intArray, reverse_seq_for_secondpass, sizeof (int) * (_numberVertices + 1));
  DFS_Loop (2, _V, _numberVertices, reverse_seq_for_secondpass, intArray, largestFiveLeader); 
 
  qsort(largestFiveLeader, (_numberVertices + 1), sizeof(int), compare_ints);
  printf ("largestFiveLeader is: [ ");
  for (int i = 0; i < 5; i++) {
    printf ("%d ", largestFiveLeader[_numberVertices - i]);
  }
  printf ("]\n");

  free (_V);
  free (largestFiveLeader);
  free (intArray);
  free (reverse_seq_for_secondpass);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf ("time spent (%f)\n", time_spent);
 
  return 0;
}
