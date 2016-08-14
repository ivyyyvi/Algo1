#include <stdlib.h>
#include <stdio.h>

typedef struct _edge edge;

struct _edge {
  char Explored;
  int tailIndex; // start
  int headIndex; // end
};


void
ReadInput (edge *E, int *numEdge)
{
  char ch;
  char collect;
  int num;
  int edgeCount = 0;

  char currentChar;
  FILE *f = fopen("data.txt", "rb");
  fseek(f, 0, SEEK_END);
  long pos = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *bytes = malloc(pos);
  fread(bytes, pos, 1, f);
  fclose(f);

  ch = 0;
  num = 0;

  printf ("Start processing bytes\n");

  for (int tracker = 0; tracker < pos; tracker++) {
    printf ("0x%x = %c \n",bytes[tracker], bytes[tracker]);

    currentChar = bytes[tracker];
    
    //
    // If get a number, collect it in num.
    //
    if (currentChar >= 0x30 && currentChar <= 0x39) {
      //DEBUG ("_%d_ \n", (currentChar - 0x30));
      num = num * 10 + (currentChar - 0x30);
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
  }

  printf ("End printing bytes\n");
  free(bytes); // free allocated memory

}

void
Dijkstra (int *A, int num_nodes, edge* E, int num_edges)
{

}

int main ()
{

  int nnn = 200; // number of nodes
  int mmm;
  int nnn_for_alloc = nnn + 1; // because A is 1-base 
  int *_A;
  edge *_E;

  _A = calloc (nnn_for_alloc, sizeof (int));
  mmm = nnn * (nnn - 1);

  _E = calloc (mmm, sizeof (edge));

  ReadInput (_E, &mmm);
  Dijkstra (_A, nnn, _E, mmm);


  /*
  for (int i = 0; i < nnn_for_alloc; i++) {
    printf ("A[%d] = %d\n", i, _A[i]);
  }
  */
  free (_A);
  free (_E);
  return 0;
}
