#include "Dijkstra.h"

int main ()
{
  int totalEdges;
  edge *_E, *_E_exact; // 0-base

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
  totalEdges = totalVertices * (totalVertices - 1);

  //
  // alloc for edges array
  //
  _E = NULL;
  _E = calloc (totalEdges, sizeof (edge));
  if (_E == NULL) {
    DEBUG_MAIN ("_E calloc failed. Abort.\n");
    goto End;
  }

  //
  // Read input into edges array
  //
  if (ReadInput (_E, &totalEdges, &totalVertices)) {
    return -1;
  }
  DEBUG_MAIN ("m = %d, n = %d\n", totalEdges, totalVertices);
  DEBUG ("------------------------------------\n\n");

  t_afterReadFile = clock();
  time_spent_readFile =
    (double)(t_afterReadFile - begin) / CLOCKS_PER_SEC;

  //
  // Realloc edges array to exactly the size it needs
  //
  _E_exact = NULL;
  _E_exact = realloc (_E, totalEdges * sizeof (edge));
  if (_E_exact == NULL) {
    DEBUG_MAIN ("_E_exact calloc failed. Abort.\n");
    goto End;
  }
  _E = NULL;

  DEBUG_MAIN ("after realloc _E to _E_exact (%p)\n", _E_exact);

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
  DEBUG_MAIN ("after realloc _A (%p)\n", _A);

  //
  // Calculate the shortest path distances
  // from source vertex = 1
  // to all the other vertices
  // using Dijkstra's algo
  //
  if (Dijkstra (_A, totalVertices, _E_exact, totalEdges)) {
    DEBUG_MAIN ("Fail at Dijkstra()\n");
    goto End;
  }
  DEBUG ("------------------------------------\n\n");


#ifdef _DEBUG
  DEBUG ("Shortest path distances are : ...\n");
  for (int i = 0; i < totalVertices_for_alloc; i++) {
    printf ("A[%d] = %d\n", i, _A[i]);
  }
#endif

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
