#include <stdlib.h>
#include <stdio.h>

typedef struct _edge edge;
struct _edge {

};

void
ReadInput ()
{

  FILE *f = fopen("testInput001.txt", "rb");
  fseek(f, 0, SEEK_END);
  long pos = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *bytes = malloc(pos);
  fread(bytes, pos, 1, f);
  fclose(f);

  //hexdump(bytes); // do some stuff with it
  printf ("Start printing bytes\n");
  for (int i = 0; i < pos; i++) {
    printf ("%c ",bytes[i]);
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

  ReadInput ();
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
