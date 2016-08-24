
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
  // 2nd pass uses decresing order of vertex index stored in index_sequence_by_finish_time_2
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
    DEBUG_DFS ("-This is (%d) pass. %d th vertices is (%d)-\n", which_pass, sss, currentOuterForLoopVertexIndex);

    //
    // if the vertex is already explored, go to the next vertex
    //
    if (V[currentOuterForLoopVertexIndex].Explored) {
      DEBUG_DFS ("V[%d] is already explored\n", currentOuterForLoopVertexIndex);
      continue;
    }

    //
    // if the vertex is not yet explored, mark it as explored
    //
    DEBUG_DFS ("V[%d] is not yet explored\n", currentOuterForLoopVertexIndex);

    //
    // init vertices_to_visit with (index of) root
    //
    *vertices_to_visit = currentOuterForLoopVertexIndex;
    num_vertices_to_visit++;

    //
    // Bookkeeping leader vertex for 2nd pass DFS_Loop.
    //
    currentLeaderIndex = currentOuterForLoopVertexIndex;

    //DEBUG_DFS ("Outer for loop @ %d (leader)\n", currentOuterForLoopVertexIndex);
    //
    // while the vertices_to_visit is not empty
    //
    while (num_vertices_to_visit) {

      //
      // pop a vertex from the vertices_to_visit stack;
      // and delete the vertex from the vertices_to_visit stack // todo, check if this step should be here
      //
      currentIndex = PopEnd (vertices_to_visit, num_vertices_to_visit);
      currentVertex = &V[currentIndex];
      num_vertices_to_visit--;
      //DEBUG_DFS ("Pop (%d) from stack\n", currentIndex);

      //
      // if the vertex is not yet explored, mark as explored, and look at its children
      //
      if (currentVertex->Explored == 0) {

        //DEBUG_DFS ("..And this vertex not yet explored. I mean (%d) who got degree (%d)\n", currentIndex, currentVertex->degree);
        currentVertex->Explored = 1; // mark as explored

        currentVertex->leader = currentLeaderIndex; // set leader

        if (leader_group) {
          //
          // leader group
          //
          leader_group[currentLeaderIndex]++;
        }
        /*
        if (which_pass == 2)
  {DEBUG_DFS ("(%d)'s leader(%d) has total#(%d) \n", currentIndex, currentLeaderIndex, leader_group[currentLeaderIndex]);}
        */

        //if (true) {// ##forPass2
          if (num_vertices_to_visit == len_max_vertices_visited) {
            len_max_vertices_visited *= 2;
            DEBUG_DFS ("vertices_visited (%p) size (%d) is not big enough.\n", vertices_visited, len_max_vertices_visited);
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
        //}

#ifdef _DEBUG
        //DEBUG_DFS ("        => Trace of visited vertex: { ");
        //for (int ppp = 0; ppp < num_vertices_visited; ppp++) {
          //DEBUG_DFS ("%d ", vertices_visited[ppp]);
        //}
        //DEBUG_DFS ("}\n");
#endif

        //
        // Traverse children vertices,
        // if none of the children is unexplored, count the finishing time of the currentVertex
        //
        notYetFinish = 0;
        //for (int j = currentVertex->degree - 1;j >= 0; j--) add a brace // to mimic class note
        for (int j = 0; j < currentVertex->degree; j++) { // different with class note
          currentChildIndex = currentVertex->connectTo[j];
          currentChildVertex = &V[currentChildIndex];
          currentChildVertex->parent_add_it_to_vertices_to_visit = currentIndex;
          //DEBUG_DFS ("  (%d)'s child (%d)\n", currentIndex, currentChildIndex);

          //
          // if the child vertex is not yet explored, append it to vertices_to_visit.
          // todo, should I mark as explored here?
          // I think NO. It is when its child vertice are about to be explored, when it is marked explored.
          //
          if (currentChildVertex->Explored == 0) {
            notYetFinish = 1; // having child that is not yet explored means this vertex is not finished exploring
            append (currentChildIndex, vertices_to_visit, num_vertices_to_visit);
            num_vertices_to_visit++;
            //DEBUG_DFS ("  ..And this child is not yet explored. appended it to vertices_to_visit...\n");
            if (num_vertices_to_visit == len_max_vertices_to_visit) {
              DEBUG_DFS ("vertices_to_visit (%p) size (%d) is not big enough.\n", vertices_to_visit, len_max_vertices_to_visit);
              len_max_vertices_to_visit *= 2;
              temp_ptr = NULL;
              temp_ptr = realloc (vertices_to_visit, len_max_vertices_to_visit);
              if (temp_ptr) {
                vertices_to_visit = temp_ptr;
              } else {
                DEBUG_DFS ("Realloc vertices_to_visit failed.\n");
                return -1;
              }
              DEBUG_DFS ("Realloc gives vertices_to_visit (%p)\n", vertices_to_visit);
            }

#ifdef _DEBUG
            //DEBUG_DFS ("        => vertices_to_visit: [ ");
            //for (int k = 0; k < num_vertices_to_visit; k++) {
              //DEBUG_DFS ("%d ", vertices_to_visit[k]);
            //} DEBUG_DFS ("]\n\n");
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
            //   ... when it is living before its parent who add it to vertices_to_visit before it was taken out
            //   i.e. is its parent now sit at the very beginning of the vertices_visited
            // if not, take out all that block its parent's way, awards the taken out ones first
            // child need to be before its parent in the vertices_visited stack world.
            // Or it would on a wrong place to award them! Its parent needs to see.
            //
            //DEBUG_DFS ("Who is at front seat of vertices_visited when (%d) is awarded finish_time ...?\n", currentIndex);
            if (vertices_visited[0] == currentVertex->parent_add_it_to_vertices_to_visit) {
              //DEBUG_DFS ("oh yeah. it is its parent! good!\n");
            } else {
              //DEBUG_DFS ("Well, not currentVertex's parent (%d) instead it is ..\n", currentVertex->parent_add_it_to_vertices_to_visit);
              for (int xxx = 0; num_vertices_visited > 0;) {

                if (vertices_visited[xxx] != currentVertex->parent_add_it_to_vertices_to_visit) {

                  //
                  // Remember to award (give finish_time to) them
                  //
                  index_sequence_by_finish_time [ttt] = vertices_visited[xxx]; //index_se... is 1-based
#ifdef _DEBUG
                  DEBUG_DFS ("  -3- here?? finish on f(%d) = (%d) *****\n\n", vertices_visited[xxx], ttt);
#endif
                  ttt++;

                  //
                  // Take it out of vertices_visited
                  //
                  takeout (vertices_visited[xxx], vertices_visited, num_vertices_visited);
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
            index_sequence_by_finish_time [ttt] = currentIndex; //index_se... is 1-based
#ifdef _DEBUG
            DEBUG_DFS ("  -1- here?? finish on f(%d) = (%d) *****\n\n", currentIndex, ttt);
#endif
            ttt++;

          } // if finished this vertex
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
          DEBUG_DFS ("  -2- here?? finish on f(%d) = (%d) *****\n\n", takeoutIndex, ttt);
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

  DEBUG ("Calling 1st pass DFS_Loop, #v (%d)\n", _numberVertices);

  t_afterReadFile = clock();
  time_spent_readFile = (double)(t_afterReadFile - begin) / CLOCKS_PER_SEC;
  DEBUG ("time spent on reading file (%f)\n", time_spent_readFile);



  reverse_seq_for_secondpass = malloc (sizeof (int) * (_numberVertices + 1)); // 1-based
  intArray = malloc (sizeof (int) * (_numberVertices + 1)); // 1-based
  reverse_seq_for_secondpass [0] = 0; // this slot is not used


  ret = DFS_Loop (1, _rV, _numberVertices, reverse_seq_for_secondpass, intArray, NULL);

  if (ret != 0) {
    DEBUG ("DFS 1st loop returned error.\n");
    return -1;
  }
  t_afterDFSLoopPass1 = clock();

  time_spent_Pass1 = (double)(t_afterDFSLoopPass1 - t_afterReadFile) / CLOCKS_PER_SEC;
  DEBUG ("time spent on DFS_Loop 1st pass (%f)\n", time_spent_Pass1);

#ifdef _DEBUG
  //DEBUG ("reverse_seq_for_secondpass is: [ ");
  //for (int i = 1; i <= _numberVertices; i++) {
    //DEBUG ("%d ", reverse_seq_for_secondpass [i]);
  //}
  //DEBUG ("]\n");
#endif

  free (_rV);

  //
  // Processing vertices in decreasing order of finishing times
  //
  leaderContainingNodes = NULL;
  leaderContainingNodes = calloc ((_numberVertices + 1), sizeof (int));

  ret = DFS_Loop (2, _V, _numberVertices, reverse_seq_for_secondpass, intArray, leaderContainingNodes);

  if (ret != 0) {
    DEBUG ("DFS 2nd loop returned error.\n");
    return -1;
  }

  t_afterDFSLoopPass2 = clock();
  time_spent_Pass2 = (double)(t_afterDFSLoopPass2 - t_afterDFSLoopPass1) / CLOCKS_PER_SEC;
  DEBUG ("time spent on DFS_Loop 2nd pass (%f)\n", time_spent_Pass2);

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
  DEBUG ("time spent totally (%f)\n", time_spent_total);
  DEBUG ("\tReadFile takes %f %% \n", time_spent_readFile * 100.0/time_spent_total);
  DEBUG ("\t1st Pass takes %f %% \n", time_spent_Pass1 * 100.0/time_spent_total);
  DEBUG ("\t2nd Pass takes %f %% \n", time_spent_Pass2 * 100.0/time_spent_total);

  return 0;
}
