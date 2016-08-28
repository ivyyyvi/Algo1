#include "Dijkstra.h"

char
isVertexInThisSet (int index, int *Set, int sizeOfSet)
{
  for (int i = 0; i < sizeOfSet; i++) { //todo E is 1-base, V is 1-base so its ok
    if (index == Set[i]) {
      return 1;
    }
  }
  return 0;
}

//
// the goal is at the end of the algorithm,
// A will be populated with the correct shortest path distances.
//
int
Dijkstra (int *A, int num_nodes, edge* E, int num_edges)
{
  int sourceVertex = 1;
  int lenX = 0;
  int *X; // vertices processed so far
  int minimumScore, Score;
  int v, w;
  int v_star, w_star, edge_star;

  //
  // X is the vertices that we dealt with so far.
  // by dealing with, it means correctly compute
  // its shortest-path distance from the source vertex
  // to every vertex in X.
  //
  X = calloc (num_nodes + 1, sizeof (int)); // 1-base
  X[0] = sourceVertex;
  lenX = 1;
  A[sourceVertex] = 0;

  //
  // each loop explores one additional possible destination vertex
  // i.e. augment X by one new vertex in each iteration of the main loop.
  // and compute its shortest-path distance from source vertex
  //
  while (lenX <= num_nodes) {
    DEBUG_DIJK ("while loop (%d)th..\n", lenX);
    //
    // Responsible for computing the shortest path distance from s to this vertex
    //

    //
    // Which one do we add to X next?
    // pick from the frontier.
    // i.e. to look at the vertices that are just one hop away
    // from vertices we've already put into X.
    // i.e. look at the edges
    //      whose tails are in X and heads are outside of X (in V-X)
    // give each vertex a score. denoting how close that
    // the vertex seems to the source vertex s
    // and to pick among all candidates vertices,
    // the one that has the minimum score.
    //

    minimumScore = 0;
    // Naively scan through all the edges
    for (int i = 1; i <= num_edges; i++) {

      //
      //
      // Among all edges (v, w) in E
      // with v in X and w not in X,
      // pick the one that minimizes A[v] + l(v,w)
      // "Dijkstra's greedy criterion"
      //
      // Dijkstra's Greedy score for a given edge (v,w)
      // that's crossing the frontier is just
      // the previously computed shortest path distance for the tail of the arc
      // plus the length of the arc itself
      //
      v = E[i].tailIndex;
      w = E[i].headIndex;
      if (isVertexInThisSet(v, X, lenX)) {
        DEBUG_DIJK ("> Found edge whose tail in X, E[%d]=(%d, %d)\n",\
                     i,\
                     v,\
                     w);
        if (isVertexInThisSet(w, X, lenX) == 0) {
          DEBUG_DIJK (">> Found edge whose head not in X, E[%d]=(%d, %d)\n",
                       i,\
                       v,\
                       w);
          //
          // Calculate the A[v]+ l(v, w)
          //
          Score = A[v] + E[i].weight;

          //
          // if this is the first score, assign it to minimum
          // if it is not, then has to compete with the current minimum
          //
          if (minimumScore == 0 || Score < minimumScore) {
            minimumScore = Score;
            edge_star = i;
            DEBUG_DIJK ("  Current min score (%d) by E[%d]\n",\
                         minimumScore,
                         edge_star);
          } // remember the minimum score so far and the vertices of the edge

        } // this edges head outside of X
      } // this edge's tail in X

    } // naively scan through all edges to find minimum score

    //
    // add w* to X
    //
    v_star = E[edge_star].tailIndex;
    w_star = E[edge_star].headIndex;
    X[lenX] = w_star;
    lenX++;

    //
    // set A[w*] = A[v*] + l(v*, w*)
    //
    A[w_star] = A[v_star] + E[edge_star].weight;

  } // while X != V

  return 0;
}
