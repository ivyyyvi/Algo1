
#include "SCC.h"

int checkChildrenExplored (vertex VertexToCheck, vertex *V){

  int allExplored = 1;
  int childIndex;
  for (int yyy = 0; yyy < VertexToCheck.degree; yyy++) {
    childIndex = VertexToCheck.connectTo[yyy];
    if (V[childIndex].Explored == 0) {
      allExplored = 0;
      break;
    }
  }

  return allExplored;
}

int isInSet (int v, int *Set, int lenOfSet) {

  for (int i = 0; i < lenOfSet; i++) {
    if (v == Set[i]) {
      return 1;
    }
  }
  return 0;

}

int
DFS_Loop (
  int which_pass,
  vertex *V, // Pointer to the array of vertices to be returned
  int numberVertices, // Pointer to the number of vertices to be returned
  int *index_sequence_by_finish_time,
  int *index_sequence_by_finish_time_2,
  int *leader_group //int *leaderContainingNodes
  )
{
  int ttt = 1; // index_se... is 1-based is 1-based
  int len_max_vertices_to_visit;
  int len_max_vertices_visited;
  int num_vertices_to_visit = 0;
  int num_vertices_visited = 0;
  int currentOuterForLoopVertexIndex;
  int currentLeaderIndex;
  int currentIndex;
  int currentChildIndex;
  int *vertices_to_visit; // only keep index of vertices
  int *vertices_visited;
  vertex *currentVertex;
  vertex *currentChildVertex;

  // only for 1st pass to compute finish_time, in particular, its sequence
  int *temp_ptr;

  //
  // Initialize vertices_to_visit stack
  //
  len_max_vertices_to_visit = numberVertices + 1;
  vertices_to_visit = NULL;
  vertices_to_visit = calloc (len_max_vertices_to_visit, sizeof (int));
  if (vertices_to_visit == NULL) {
    DEBUG_DFS ("vertices_to_visit alloc failed\n");
    return -1;
  }

  //
  // Initialize vertices_visited stack
  //
  len_max_vertices_visited = numberVertices + 1;
  vertices_visited = NULL;
  vertices_visited = calloc (len_max_vertices_visited, sizeof (int));
  if (vertices_visited == NULL) {
    DEBUG_DFS ("vertices_visited alloc failed\n");
    return -1;
  }

  //
  // Traverse the entire graph to call DFS from
  // Note that 1st pass and 2nd pass DFS_Loop is using different sequence
  // 1st pass uses decresing order of vertex.index
  // 2nd pass uses decresing order of vertex index stored in
  // index_sequence_by_finish_time_2
  //
  for (int sss = numberVertices; sss > 0; sss--) {

    switch (which_pass) {
      case 1:
        currentOuterForLoopVertexIndex = sss;
        break;
      case 2:
        currentOuterForLoopVertexIndex = index_sequence_by_finish_time_2[sss];
        break;
      default:
        break;
    }

    DEBUG_DFS ("%d pass. Start to_visit stack from V[%d] ...\n", \
                which_pass,\
                currentOuterForLoopVertexIndex);

    //
    // init vertices_to_visit with (index of) root
    //
    *vertices_to_visit = currentOuterForLoopVertexIndex;
    num_vertices_to_visit++;

    //
    // Bookkeeping leader vertex for 2nd pass DFS_Loop.
    //
    currentLeaderIndex = currentOuterForLoopVertexIndex;

    //DEBUG_DFS ("Outer for loop @ %d (leader)\n",\
    //currentOuterForLoopVertexIndex);
    //
    // while the vertices_to_visit is not empty
    //
    while (num_vertices_to_visit) {

      //
      // pop a vertex from the vertices_to_visit stack;
      // and delete the vertex from the vertices_to_visit stack
      // todo, check if this step should be here
      //
      currentIndex = PopEnd (vertices_to_visit, num_vertices_to_visit);
      currentVertex = &V[currentIndex];
      num_vertices_to_visit--;
      DEBUG_DFS ("  Pop (%d) from to_visit stack\n", currentIndex);

      //
      // if the vertex is not yet explored, mark as explored,
      // and look at its children
      //
      if (isInSet(currentIndex, vertices_visited, num_vertices_visited) == 0) {

        DEBUG_DFS ("  ,and V[%d] not in visited stack\n",\
                   currentIndex);

        if (which_pass == 2) {
          currentVertex->leader = currentLeaderIndex; // set leader

          if (leader_group) {
            //
            // leader group
            //
            leader_group[currentLeaderIndex]++;
          }
          printf ("l%d=%d / %d\n", currentIndex, currentLeaderIndex, leader_group[currentLeaderIndex]);
        }

        append (currentIndex, vertices_visited, num_vertices_visited);
        num_vertices_visited++;

#ifdef _DEBUG_DFS
        DEBUG_DFS ("  ,append V[%d] to visited stack: [[ ", currentIndex);
        for (int ppp = 0; ppp < num_vertices_visited; ppp++) {
          printf ("%d ", vertices_visited[ppp]);
        }
        printf ("]]\n");
#endif

        //
        // Here's the trick for 1st pass
        //
        append (currentIndex, vertices_to_visit, num_vertices_to_visit);
        num_vertices_to_visit++;
#ifdef _DEBUG_DFS
            DEBUG_DFS ("put back V[%d] to vertices_to_visit: [ ", currentIndex);
            for (int k = 0; k < num_vertices_to_visit; k++) {
              printf ("%d ", vertices_to_visit[k]);
            } printf ("]\n\n");
#endif

        //
        // Traverse children vertices,
        // if not yet in visited stack,
        // append it to vertices_to_visit
        //

        for (int j = 0; j < currentVertex->degree; j++) { // diff with lesson
          currentChildIndex = currentVertex->connectTo[j];
          currentChildVertex = &V[currentChildIndex];
          DEBUG_DFS ("    (%d)'s child (%d)\n", currentIndex, currentChildIndex);

          //
          // if the child vertex is not yet explored,
          // append it to vertices_to_visit.
          //
          if (isInSet (
                currentChildIndex,
                vertices_visited,
                num_vertices_visited) == 0) {

            append (currentChildIndex, vertices_to_visit, num_vertices_to_visit);
            num_vertices_to_visit++;

#ifdef _DEBUG_DFS
            DEBUG_DFS ("child V[%d] not in so append to vertices_to_visit: [ ",\
                        currentChildIndex);
            for (int k = 0; k < num_vertices_to_visit; k++) {
              printf ("%d ", vertices_to_visit[k]);
            } printf ("]\n\n");
#endif

          } // if this child is not explored
        } // for all children


      } // if current vertex is not in vertices_visited
      else {
        if (which_pass == 1) {
        DEBUG_DFS ("V[%d] is in visited stack\n",\
                    currentIndex);

        //
        // If current vertex is in vertices_visited
        // and if it has not get finish time
        //
        if (isInSet (currentIndex, index_sequence_by_finish_time, ttt+1) == 0) { //1-base
          DEBUG_DFS ("And If V[%d] is not in finished stack\n",\
                      currentIndex);
          DEBUG_DFS ("#### V[%d] is finished with (%d) time ####\n",currentIndex, ttt);
          printf ("f%d=%d\n",currentIndex, ttt);
          index_sequence_by_finish_time [ttt] = currentIndex;
          ttt++;
        }

        }

      } // if current vertex is in vertices_visited
    } // while

  } // for
  free (vertices_visited);
  return 0;
}
