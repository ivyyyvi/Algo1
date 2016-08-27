
//
//  SCC.c
//  Algo
//
//  Created by Ivy on 8/7/2016
//  Copyright (c) 2016 ___IVYCANT___. All rights reserved.
//

#include "SCC.h"

int nnn = 875715; // max number of vertices just for alloc // this can be doubled if not enough

int
PopEnd (int *wholedeal, int numTotal)
{
  int vertex_to_pop;
  vertex_to_pop = wholedeal[numTotal - 1];
  // remove
  wholedeal[numTotal] = 0;

  return vertex_to_pop;
}; // removes and returns the end element in the list.


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
append (int vertex_to_append, int *vertices_to_visit, int num_vertices_to_visit)
{
  //
  // append (of course) to the end of the stack
  //
  vertices_to_visit[num_vertices_to_visit] = vertex_to_append;
  return;
}

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
takeout (int vertex_to_takeout, int *vertices, int num_vertices)
{
  for (int ppp = 0; ppp < num_vertices; ppp++) {
    if (vertices[ppp] == vertex_to_takeout) {

      //
      // move everything after this to one slot smaller place
      // do it from smaller address
      //
      for (int qqq = ppp + 1; qqq < num_vertices; qqq++) {
        vertices [qqq - 1] = vertices [qqq];
      }
      break;
    }
  }
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


int
compare_ints (
  const void* a,
  const void* b
  )
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
  int *leaderContainingNodes;
  int ret;

  double time_spent_readFile, time_spent_Pass1, time_spent_Pass2, time_spent_total;
  clock_t end, t_afterReadFile, t_afterDFSLoopPass1, t_afterDFSLoopPass2;
  clock_t begin = clock();

  DEBUG ("Start reading girl~\n");
  if (0 != ReadFileToAdjList (
             &_V,
             &_rV,
             &_numberVertices
             )) {
    return -1;
  }

  t_afterReadFile = clock();
  time_spent_readFile = (double)(t_afterReadFile - begin) / CLOCKS_PER_SEC;
  DEBUG ("time spent on reading file (%f)\n\n", time_spent_readFile);
  DEBUG ("------------------------------------\n");

  reverse_seq_for_secondpass = malloc (sizeof (int) * (_numberVertices + 1)); // 1-based
  intArray                   = malloc (sizeof (int) * (_numberVertices + 1)); // 1-based
  reverse_seq_for_secondpass [0] = 0; // this slot is not used


  DEBUG ("Calling 1st pass DFS_Loop, #v (%d) ...\n", _numberVertices);
  ret = DFS_Loop (
          1,
          _rV,
          _numberVertices,
          reverse_seq_for_secondpass,
          intArray,
          NULL);

  if (ret != 0) {
    DEBUG ("DFS 1st loop returned error.\n");
    return -1;
  }

  t_afterDFSLoopPass1 = clock();
  time_spent_Pass1 =
    (double)(t_afterDFSLoopPass1 - t_afterReadFile) / CLOCKS_PER_SEC;

  DEBUG ("time spent on DFS_Loop 1st pass (%f)\n", time_spent_Pass1);
  DEBUG ("------------------------------------\n");

#ifdef _DEBUG
  DEBUG ("reverse_seq_for_secondpass is: [ ");
  for (int i = 1; i <= _numberVertices; i++) {
    printf ("%d ", reverse_seq_for_secondpass [i]);
  }
  printf ("]\n");
#endif

  free (_rV);

  //
  // Processing vertices in decreasing order of finishing times
  //

  memcpy ((void *)intArray, (void *)reverse_seq_for_secondpass,
    sizeof (int) * (_numberVertices + 1));

  leaderContainingNodes = NULL;
  leaderContainingNodes = calloc ((_numberVertices + 1), sizeof (int));

  DEBUG ("Calling 2nd pass DFS_Loop ...\n\n");
  ret = DFS_Loop (
          2,
          _V,
          _numberVertices,
          reverse_seq_for_secondpass,
          intArray,
          leaderContainingNodes);

  if (ret != 0) {
    DEBUG ("DFS 2nd loop returned error.\n");
    return -1;
  }

  t_afterDFSLoopPass2 = clock();
  time_spent_Pass2 =
    (double)(t_afterDFSLoopPass2 - t_afterDFSLoopPass1) / CLOCKS_PER_SEC;

  DEBUG ("time spent on DFS_Loop 2nd pass (%f)\n", time_spent_Pass2);
  DEBUG ("------------------------------------\n");

  qsort(leaderContainingNodes, (_numberVertices + 1), sizeof(int), compare_ints);

  DEBUG ("leaderContainingNodes is: [ ");
  for (int i = 0; i < 6; i++) {
    printf ("%d ", leaderContainingNodes[_numberVertices - i]);
  }
  printf ("]\n");

  free (_V);
  free (leaderContainingNodes);
  free (intArray);
  free (reverse_seq_for_secondpass);
  end = clock();
  time_spent_total = (double)(end - begin) / CLOCKS_PER_SEC;

  DEBUG ("------------------------------------\n");
  DEBUG ("Totally takes  (%f)\n",\
    time_spent_total);
  DEBUG ("\tReadFile takes %f %% \n",\
    time_spent_readFile * 100.0/time_spent_total);
  DEBUG ("\t1st Pass DFS takes %f %% \n",\
    time_spent_Pass1 * 100.0/time_spent_total);
  DEBUG ("\t2nd Pass DFS takes %f %% \n",\
    time_spent_Pass2 * 100.0/time_spent_total);

  return 0;
}
