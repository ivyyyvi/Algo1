
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
  int currentVertexFinished;
  int len_max_vertices_to_visit;
  int len_max_vertices_visited;
  int num_vertices_to_visit = 0;
  int num_vertices_visited = 0;
  int currentOuterForLoopVertexIndex;
  int currentLeaderIndex;
  int currentIndex;
  int takeoutIndex;
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

    DEBUG_DFS ("<V[%d] by %d pass for %dth loop>\n", \
                currentOuterForLoopVertexIndex,\
                which_pass,\
                sss);

    //
    // if the vertex is already explored, go to the next vertex
    //
    if (V[currentOuterForLoopVertexIndex].Explored) {
      DEBUG_DFS (" ,but V[%d] is already explored\n",\
                  currentOuterForLoopVertexIndex);
      continue;
    }

    DEBUG_DFS (" ,V[%d] is not yet explored\n",\
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
      DEBUG_DFS ("  Pop (%d) from stack\n", currentIndex);

      //
      // if the vertex is not yet explored, mark as explored,
      // and look at its children
      //
      if (currentVertex->Explored == 0) {

        DEBUG_DFS ("  ,and V[%d] not yet explored. who got degree (%d)\n",\
                   currentIndex, \
                   currentVertex->degree);

        currentVertex->Explored = 1; // mark as explored

        currentVertex->leader = currentLeaderIndex; // set leader

        if (which_pass == 2) {
          DEBUG_DFS ("  ,V[%d]'s leader= V[%d]\n",\
                      currentVertex->index,\
                      currentLeaderIndex);
        }


        if (leader_group) {
          //
          // leader group
          //
          leader_group[currentLeaderIndex]++;
        }

        if (num_vertices_visited == len_max_vertices_visited) {
          len_max_vertices_visited *= 2;

          DEBUG_DFS ("vertices_visited (%p) size (%d) is not big enough.\n",\
                      vertices_visited, \
                      len_max_vertices_visited);

          temp_ptr = NULL;
          temp_ptr = realloc (vertices_visited, len_max_vertices_visited);

          if (temp_ptr) {
            vertices_visited = temp_ptr;
          } else {
            DEBUG_DFS ("Realloc vertices_visited failed.\n");
            return -1;
          }

          DEBUG_DFS ("Realloc vertices_vitited gives (%p)\n", vertices_visited);
        }

        append (currentIndex, vertices_visited, num_vertices_visited);
        num_vertices_visited++;

#ifdef _DEBUG
        //DEBUG_DFS ("        => Trace of visited vertex: { ");
        //for (int ppp = 0; ppp < num_vertices_visited; ppp++) {
          //DEBUG_DFS ("%d ", vertices_visited[ppp]);
        //}
        //DEBUG_DFS ("}\n");
#endif

        //
        // Traverse children vertices,
        // if all children are explored,
        // this currentVertex is finished.
        // Count the finishing time of the currentVertex
        //
        currentVertexFinished = 1;

        for (int j = 0; j < currentVertex->degree; j++) { // diff with lesson
          currentChildIndex = currentVertex->connectTo[j];
          currentChildVertex = &V[currentChildIndex];
          currentChildVertex->parentVertex = currentIndex;
          DEBUG_DFS ("    (%d)'s child (%d)\n", currentIndex, currentChildIndex);

          //
          // if the child vertex is not yet explored,
          // append it to vertices_to_visit.
          // todo, should I mark as explored here?
          // I think NO.
          // It is when its child vertice are about to be explored,
          // then it should be marked explored.
          //
          if (currentChildVertex->Explored == 0) {

            // having child that is not yet explored
            // means this vertex is not finished
            currentVertexFinished = 0;
            append (currentChildIndex, vertices_to_visit, num_vertices_to_visit);
            num_vertices_to_visit++;

            DEBUG_DFS ("    ,le child not yet expl'ed.\n");
            DEBUG_DFS ("    ,appended it to vertices_to_visit...\n");

            if (num_vertices_to_visit == len_max_vertices_to_visit) {
              DEBUG_DFS ("vertices_to_visit (%p) size (%d) is not big enough.\n",\
                          vertices_to_visit, len_max_vertices_to_visit);
              len_max_vertices_to_visit *= 2;
              temp_ptr = NULL;
              temp_ptr = realloc (vertices_to_visit, len_max_vertices_to_visit);
              if (temp_ptr) {
                vertices_to_visit = temp_ptr;
              } else {
                DEBUG_DFS ("Realloc vertices_to_visit failed.\n");
                return -1;
              }

              DEBUG_DFS ("Realloc gives vertices_to_visit (%p)\n",\
                          vertices_to_visit);
            } // if vertices_to_visit array size too small

#ifdef _DEBUG
            DEBUG_DFS ("      => vertices_to_visit: [ ");
            for (int k = 0; k < num_vertices_to_visit; k++) {
              printf ("%d ", vertices_to_visit[k]);
            } printf ("]\n\n");
#endif

          } // if this child is not explored
        } // for all children


        if (currentVertexFinished) {
          DEBUG_DFS ("      V[%d] finished.\n",\
                      currentVertex->index);
#ifdef _DEBUG
          DEBUG_DFS ("      => vertices_visited: [ ");
          for (int k = 0; k < num_vertices_visited; k++) {
            printf ("%d ", vertices_visited[k]);
          } printf ("]\n\n");
#endif
          //
          // Alright, currentVertex finished its job!
          // assign its finish time.
          // and take it out from the visited stack
          //
          index_sequence_by_finish_time [ttt] = currentIndex;
          ttt++;
          takeout (currentIndex, vertices_visited, num_vertices_visited);
          num_vertices_visited--;
                DEBUG_DFS ( " f(V[%d]) = %d\n\n",\
                           currentIndex, ttt - 1);

          //
          // assign finish time to the rest of visited vertices carefully
          // check if they are all really finished
          //

          for (int xxx = num_vertices_visited - 1; xxx >= 0; xxx--) {
            if (checkChildrenExplored (V[vertices_visited[xxx]], V)) {
              index_sequence_by_finish_time [ttt] = vertices_visited[xxx];
              ttt++;
                DEBUG_DFS ( " f(V[%d]) = %d\n\n",\
                           vertices_visited[xxx], ttt - 1);
              takeout (vertices_visited[xxx],
                  vertices_visited,
                  num_vertices_visited);
              num_vertices_visited--;

            } else {

              break;
            }
          }
        } // if finished this vertex
      } // if current vertex is not yet explored. else {}
    } // while

    //
    // Finished this one outer for loop, Otsukaresama!
    // Give rewards to these cute little vertices
    // who haven't got it (still in vertices_visited stack)
    // by counting their finishing time.
    // the smaller index in vertices_visited, the shorter its finishing time.
    //
    for (int ppp = 0; num_vertices_visited > 0; ppp++) {

        takeoutIndex = vertices_visited [0];
        //
        // Also take it out from the visited stack
        //
        takeout (takeoutIndex, vertices_visited, num_vertices_visited);
        num_vertices_visited--;

        //
        // Or this? It is time to "giu" back and count finish time!
        //
        index_sequence_by_finish_time [ttt] = takeoutIndex;


#ifdef _DEBUG
        DEBUG_DFS ("  -2- here?? finish on f(%d) = (%d) *****\n\n",\
                    takeoutIndex, \
                    ttt);
#endif
        ttt++;

      } // for loop to give finishing time
  } // for
  free (vertices_visited);
  return 0;
}
