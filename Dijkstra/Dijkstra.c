#include "Dijkstra.h"

int
Dijkstra (int *A, int num_nodes, edge* E, int num_edges)
{
  int sourceVertex = 1;
  int *X; // vertices processed so far

  X = calloc (num_nodes + 1, sizeof (int)); // 1-base
  X[0] = sourceVertex;
  A[sourceVertex] = 0; // 1-base

  //
  // Among all edges (v, w) in E
  // with v in X and w not in X,
  // pick the one that minimizes A[v] + l(v,w)
  //

  //
  // Naively scan through all the edges
  //

  //
  // add w* to X
  //

  //
  // set A[w*] = A[v*] + l(v*, w*)
  //
  return 0;
}
