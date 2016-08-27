
#include "SCC.h"

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
  int ttt = 1;
  int notYetFinish;
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
        // if none of the children is unexplored,
        // count the finishing time of the currentVertex
        //
        notYetFinish = 0;
        //for (int j = currentVertex->degree - 1;j >= 0; j--) same w/ lesson
        // remember to add brace
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
            // means this vertex is not finished exploring
            notYetFinish = 1;
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
            }

#ifdef _DEBUG
            DEBUG_DFS ("      => vertices_to_visit: [ ");
            for (int k = 0; k < num_vertices_to_visit; k++) {
              printf ("%d ", vertices_to_visit[k]);
            } printf ("]\n\n");
#endif
          } // if the child is not explored
          else {
            //
            // It is time to "giu" back and count finish time!
            //
            //DEBUG_DFS ("  -- here?? finish on (%d) *****\n", currentChildIndex);
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
            //   when it is living before its parent
            //   who add it to vertices_to_visit before it was taken out
            //   i.e. is its parent now sit at
            //        the very beginning of the vertices_visited
            // if not,
            // take out all that block its parent's way,
            // awards the taken out ones first
            // child need to be before its parent
            // in the vertices_visited stack world.
            // Or it would on a wrong place to award them!
            // Its parent needs to see.
            //
            //DEBUG_DFS ("Who is at front seat of vertices_visited "
            //"when (%d) is awarded finish_time ...?\n", currentIndex);
            if (vertices_visited[0] == currentVertex->parentVertex) {
              //DEBUG_DFS ("oh yeah. it is its parent! good!\n");
            } else {
              //DEBUG_DFS ("Well, not currentVertex's parent (%d) "
              //"instead it is ..\n", currentVertex->parentVertex);
              for (int xxx = 0; num_vertices_visited > 0;) {

                if (vertices_visited[xxx]
                    != currentVertex->parentVertex) {

                  //
                  // Remember to award (give finish_time to) them
                  //
                  index_sequence_by_finish_time [ttt] = vertices_visited[xxx];
                  //index_se... is 1-based
#ifdef _DEBUG
                  DEBUG_DFS ("  -3- finish? wrong need fix."
                             " f(V[%d]) = %d\n\n",\
                             vertices_visited[xxx], ttt);
#endif
                  ttt++;

                  //
                  // Take it out of vertices_visited
                  //
                  takeout (
                      vertices_visited[xxx],
                      vertices_visited,
                      num_vertices_visited);
                  num_vertices_visited--;


                } else {
#ifdef _DEBUG
                  //DEBUG_DFS ("\n");
#endif
                  break;
                }
              } // for
            } // if-else

            //
            // Or this? It is time to "giu" back and count finish time!
            //
            index_sequence_by_finish_time [ttt] = currentIndex;
            //index_se... is 1-based
#ifdef _DEBUG
            DEBUG_DFS ("  -1- here?? finish on f(%d) = (%d) *****\n\n",\
                       currentIndex, \
                       ttt);
#endif
            ttt++;

          } // if finished this vertex
        //} // if 1st pass

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
        //index_se... is 1-based

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
