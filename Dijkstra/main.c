#include <stdlib.h>
#include <stdio.h>

#define ASCII_CODE_TAB 0x09
#define ASCII_CODE_SPACE 0x20
#define ASCII_CODE_COMMA 0x2C
#define ASCII_CODE_NEWLINE 0x0A
#define ASCII_CODE_CARRIAGE_RETURN 0x0D

#define _DEBUG

#ifdef _DEBUG
#define DEBUG(format, args...) printf("[%s:%d] "format, __FILE__, __LINE__, ##args)
#else
#define DEBUG(args...)
#endif

typedef struct _edge edge;

struct _edge {
  char Explored;
  int tailIndex; // start
  int headIndex; // end
  int weight; // length
};

enum readingFlag {
  isAfterNewLineCR = 1,
  isAfterComma = 2,
  isAfterSpace = 3
};

void
ReadInput (edge *E, int *numEdge, int *numVertices)
{
  char ch;
  char collectFlag;
  enum readingFlag flag;
  int currentReadNum;
  int vertexIndexMax = 0;
  int verticesCount = 0;
  int edgesCount = 0;
  edge* currentEdge = NULL;

  char currentChar;
  FILE *f = fopen("data.txt", "rb");
  fseek(f, 0, SEEK_END);
  long pos = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *bytes = malloc(pos);
  fread(bytes, pos, 1, f);
  fclose(f);

  ch = 0;
  currentReadNum = 0;

  DEBUG ("Start processing bytes\n");

  flag = isAfterNewLineCR;
  for (int tracker = 0; tracker < pos; tracker++) {
    //DEBUG ("0x%x = %c \n",bytes[tracker], bytes[tracker]);

    currentChar = bytes[tracker];

    //
    // If get a number, collectFlag it in currentReadNum.
    //
    if (currentChar >= 0x30 && currentChar <= 0x39) {
      //DEBUG ("_%d_ \n", (currentChar - 0x30));
      currentReadNum = currentReadNum * 10 + (currentChar - 0x30);
      collectFlag = 1;
    } else {

      //
      // The character reading here is not a number.
      // so first see what we have collected
      //
      if (collectFlag == 1) {

        //
        // if a number has been collected.
        //
        collectFlag = 0;

        switch (flag) {
          case isAfterNewLineCR:

            //
            // Before collecting this number, there was immedately a newline/CR
            //

            //
            // use max of num saw at the beginnging of lines
            // to be the vertices count
            // this assumes the leading number of the lines are always (tail) vertex index
            //
            if (currentReadNum > vertexIndexMax) {
              vertexIndexMax = currentReadNum;
            }
            verticesCount++;//1-base
            //DEBUG ("verticesCount++ to (%d)\n", verticesCount);
            currentEdge = &E[verticesCount];
            if (currentEdge) {
              //DEBUG ("1 Read (%d) as tailIndex\n", currentReadNum);
              currentEdge->tailIndex = currentReadNum;
            }
            break;
          case isAfterComma:
            if (currentEdge) {
              //DEBUG ("3 Read (%d) as weight\n", currentReadNum);
              currentEdge->weight = currentReadNum;
              edgesCount++;
              //DEBUG ("-----So this is %dth edge = (%d, %d; %d)\n", edgesCount, currentEdge->tailIndex, currentEdge->headIndex, currentEdge->weight);
            }
            break;
          case isAfterSpace:
          default:
            if (currentEdge) {
              //DEBUG ("2 Read (%d) as headIndex\n", currentReadNum);
              currentEdge->headIndex = currentReadNum;
            }
            break;
        } // switch flag to deal with number just collected

        currentReadNum = 0;
        flag = 0;

        switch (currentChar) {

          case ASCII_CODE_CARRIAGE_RETURN:
          case ASCII_CODE_NEWLINE:
            //DEBUG ("The next number is leading number of a line => (tail) vertex index\n");
            flag = isAfterNewLineCR;
            break;

          case ASCII_CODE_COMMA:
            //DEBUG ("The next number is the number after comma => weight\n");
            flag = isAfterComma;
            break;

          case ASCII_CODE_SPACE:
          case ASCII_CODE_TAB:
          default:
            //DEBUG ("The next number is (head) vertex index\n");
            flag = isAfterSpace;
            break;

        } // switch case to deal with number collected after this non-numeric char

      } // if collectFlag is 1

    } // if-else reading number or non-number
  } // for loop traverse bytes

  DEBUG ("End processing bytes\n");
  free(bytes); // free allocated memory

  *numEdge = edgesCount;
  *numVertices = vertexIndexMax;
} // ReadInput

void
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

}

int main ()
{
  int totalVertices; // number of nodes
  int totalEdges;
  int totalVertices_for_alloc = totalVertices + 1; // because A is 1-base
  int *_A; // computed shortest path distances for each vertex; 1-base
  edge *_E, *_E_exact;

  totalVertices = 200; // just estimate first
  totalEdges = totalVertices * (totalVertices - 1); // just estimate first

  _E = calloc (totalEdges, sizeof (edge));

  ReadInput (_E, &totalEdges, &totalVertices);
  DEBUG ("m = %d, n = %d\n", totalEdges, totalVertices);

  //
  // Now alloc with exact size needed.
  //
  _E_exact = realloc (_E, totalEdges * sizeof (edge));
  //DEBUG ("after realloc _E (%p)\n", _E);
  totalVertices_for_alloc = totalVertices + 1;
  _A = calloc (totalVertices_for_alloc, sizeof (int));
  //DEBUG ("after realloc _A (%p)\n", _A);

  Dijkstra (_A, totalVertices, _E_exact, totalEdges);


  /*
  for (int i = 0; i < totalVertices_for_alloc; i++) {
    DEBUG ("A[%d] = %d\n", i, _A[i]);
  }
  */
  free (_A);
  //DEBUG ("after freed _A\n");
  free (_E_exact);
  //DEBUG ("after freed _E_exact\n");
  return 0;
}
