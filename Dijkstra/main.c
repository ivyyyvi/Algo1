#include "Dijkstra.h"

int main ()
{
  int NeedOutput[10] = {7, 37, 59, 82, 99, 115, 133, 165, 188, 197};
  int estNumEdges, exactNumEdges, numEdges_for_alloc;
  edge *_E, *_E_exact; // 1-based

  // computed shortest path distances for each vertex; 1-base
  int *_A;
  int totalVertices; // number of nodes
  // because A is 1-base
  int totalVertices_for_alloc = totalVertices + 1;

  //
  // to show time spent
  //
  double time_spent_readFile, time_spent_Dij, time_spent_total;
  clock_t end, t_afterReadFile, t_afterDij;
  clock_t begin = clock();

  totalVertices = 200;
  // use the max possible first
  estNumEdges = totalVertices * (totalVertices - 1);

  //
  // alloc for edges array
  //
  _E = NULL;
  _E = calloc (estNumEdges, sizeof (edge));
  if (_E == NULL) {
    DEBUG_MAIN ("_E calloc failed. Abort.\n");
    goto End;
  }

  //
  // Read input into edges array
  //
  if (ReadInput (_E, &exactNumEdges, &totalVertices)) {
    return -1;
  }
  DEBUG_MAIN ("m = %d, n = %d\n", exactNumEdges, totalVertices);

  t_afterReadFile = clock();
  time_spent_readFile =
    (double)(t_afterReadFile - begin) / CLOCKS_PER_SEC;

  //
  // Realloc edges array to exactly the size it needs
  //
  _E_exact = NULL;
  numEdges_for_alloc = exactNumEdges + 1;

  DEBUG_MAIN ("------------------------------------\n\n");
  DEBUG_MAIN ("_E:\n");
  for (int i = 1; i < numEdges_for_alloc; i++) {
      DEBUG_MAIN ("_E[%d] = (%d -> %d)\n", i, _E[i].tailIndex, _E[i].headIndex);
  }

  _E_exact = realloc (_E, numEdges_for_alloc * sizeof (edge));
  if (_E_exact == NULL) {
    DEBUG_MAIN ("_E_exact calloc failed. Abort.\n");
    goto End;
  }
  _E = NULL;
  DEBUG_MAIN ("_E_exact:\n");
  for (int i = 1; i < numEdges_for_alloc; i++) {
      DEBUG_MAIN ("_E_exact[%d] = (%d -> %d)\n",\
                   i,\
                   _E_exact[i].tailIndex,\
                   _E_exact[i].headIndex);
  }

  //
  // Alloc shortest path distance array
  // todo: is it better to memset all to 1,000,000
  //       that denotes no path required by PA?
  //
  totalVertices_for_alloc = totalVertices + 1;
  _A = NULL;
  _A = calloc (totalVertices_for_alloc, sizeof (int));
  if (_A == NULL) {
    DEBUG_MAIN ("_A calloc failed. Abort.\n");
    goto End;
  }

  //
  // Calculate the shortest path distances
  // from source vertex = 1
  // to all the other vertices
  // using Dijkstra's algo
  //
  
  DEBUG_MAIN ("Calling Dijkstra...\n");
  if (Dijkstra (_A, totalVertices, _E_exact, exactNumEdges)) {
    DEBUG_MAIN ("Fail at Dijkstra()\n");
    goto End;
  }
  DEBUG ("------------------------------------\n\n");


#ifdef _DEBUG
  DEBUG ("Shortest path distances are : ...\n");
  for (int i = 1; i < totalVertices_for_alloc; i++) {
    printf ("A[%d] = %d\n", i, _A[i]);
  }
#endif
  printf ("++Here's answer:\n");
  for (int i = 0; i < 10; i++) {
    printf ("%d,", _A[NeedOutput[i]]);
  }
  printf ("\n");

  end = clock();
  time_spent_total = (double)(end - begin) / CLOCKS_PER_SEC;

  DEBUG ("------------------------------------\n\n");
  DEBUG ("Totally takes  (%f)\n",\
    time_spent_total);
  DEBUG ("\tReadFile takes %f %%\n",\
    time_spent_readFile * 100.0/time_spent_total);
  DEBUG ("\tDijkstra's algo takes %f %%\n",\
    time_spent_Dij * 100.0/time_spent_total);
End:
  if (_A) free (_A);
  if (_E) free (_E);
  if (_E_exact) free (_E_exact);
  return 0;
}
