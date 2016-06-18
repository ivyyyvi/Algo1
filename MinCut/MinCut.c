//
//  main.c
//  Algo
//
//  Created by Ivy on 5/30/2016
//  Copyright (c) 2016 ___IVYCANT___. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void 
printArray (int *A, int l ,int r)
{
  int i;
  i = 0;
  printf ("\n");
  for (i = l;i <= r; i++){
    printf ("%d ", A[i]);
  }
  printf ("\n");
}

void 
Swap (int* A, int index1, int index2) 
{
  int temp;
  temp = A[index1];
  A [index1] = A [index2];
  A [index2] = temp;
}



void
ReadFileToAdjList (
  int ***pArray
)
{
  int num;
  int ttsize; 
  long ttInv;
  int nnn; // number of vertices
  int collect;
  int **tt;
  int *ttAlt;
  char file_name[100] = "";
  char *tracker;
  FILE *fp;
  char ch;
  int i;
  int vertexindex;

  printf ("Type File name:");
  fgets (file_name, 100, stdin);
  tracker = file_name;
  vertexindex = 0;
  while (*tracker) {
    printf ("*file_name is  (0x%x)\n", *tracker);  
    if (*tracker == 0xA || *tracker == 0xD) {
      printf ("found new line (0x%x)\n", *tracker);  
      *tracker = 0;
      break;
    }
    tracker++;
  }
  fp = fopen (file_name, "r");  // read mode
  if (fp == NULL) {
    printf ("Error while opening the file.\n");
    return;
  }
  nnn = 200; // todo
  tt = malloc (sizeof(int *) * nnn);
  for (i = 0; i < 200; i++) {
    tt [i] = malloc (sizeof (int) * nnn);
  }
  ch = 0;
  i = 0;
  while (((ch = fgetc (fp)) != EOF)) {
    //
    // If get a number, collect it in num.
    //

    if (ch >= 0x30 && ch <= 0x39) {
      //printf ("_%d_ ", (ch - 0x30));
      num = num * 10 + (ch - 0x30);
      collect = 1;
    } 
    else {
      //printf ("Got non-numeric char!\n");

      //
      // If get a non-numeric char, put num into array.
      //
      /*
      if (i >= nnn) {

        //
        // If array size is not enought, reallocate to double its size.
        //

        printf ("nnn (%d) not enough, double it!\n", nnn);
        ttAlt = tt;
        nnn *= 2;
        tt = malloc (nnn);
        memcpy (tt, ttAlt, nnn/2);
        free (ttAlt);
      }
      */
      if (collect == 1) {
        collect = 0;
        printf ("%d\n", num);
        tt [i] = num;
        num = 0;
        i++;
      }
    }
  }
  ttsize = i;
  //ttsize = 999;
  **pArray = tt;
  
  fclose (fp);
  return;
}


int 
main (
  int argc, 
  const char * argv[]
  )
{
  int **input;
  int arraySize;

  printf ("CountMinCut.\n");
  ReadFileToAdjList (&input);

  return 0;
}
