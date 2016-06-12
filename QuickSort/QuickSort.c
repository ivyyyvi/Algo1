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
long gComparison;

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
ChoosePivot (int* inputArray, int leftmost, int rightmost, int* pIndex)
{
  int index;
  int a,b,c;
  //index = rightmost;
  //index = leftmost; //right now we will use the first element as pivot 
  a = inputArray [leftmost];
  c = inputArray [rightmost];
  b = inputArray [(rightmost-leftmost)/2];
  if ((a > b && a < c) || (a > c && a < b)) {
    index = leftmost;
  } else if ((b > a && b < c) || (b > c && b < a)) {
    index = (rightmost - leftmost) / 2;
  } else {
    index = rightmost;
  }
  
  *pIndex = index;
  return;
}

void
Partition (
  int *A, 
  const int l, 
  const int r)
{
  int i; // leftmost index of the bigger half 
  int j; // leftmost index of unpartitioned half
  int k;
  int pivotIndex;
  int length;

  /*
  printf ("hi\n");
  printf ("  l=%d, r=%d\n", l, r);
  */
  if (l >= r) {
    return;
  }
  length = r - l + 1;
  gComparison += length - 1;
  ChoosePivot(A, r, l, &pivotIndex);
  /*
  printf ("  chose pivot :A[%d]=%d\n", pivotIndex, A[pivotIndex]);
  */

  //
  // If pivot is not the leftmost index,
  // swap it with the leftmost index
  //

  if (pivotIndex) {
    Swap (A, l, pivotIndex);
  }

  i = l + 1;
  for (j=l + 1; j <= r; j++) {
    if (A[j] < A[l]) {

      //
      // Swap the element smaller than pivot
      // with the leftmost element of the bigger half
      //
      /*
      printf ("Before swapping...");
      printArray (A, l, r);
      printf ("  A[%d]=%d < %d\n", j, A[j], A[l]);
      */
      
      Swap (A, i, j);
      /*
      printf ("After swapping...");
      printArray (A, l, r);
      */
      
      i++; // advance the bigger half leftmost index
    }
  } 
  Swap (A, l, i - 1);
  /*
  printArray (A, l, r);
  printf ("finished partitioning subroutine with i-1=%d A[i-1]=%d\n", i-1, A[i-1]);
  */
  
  Partition (A, l, i - 2);
  Partition (A, i, r);
}

int 
main_onsmalltest (
  int argc, 
  const char * argv[]
  )
{
  int inputArray[] = {3,7,2,5,1,4,7,6};

  Partition (inputArray, 0, 7);
  printArray (inputArray, 0, 7);

  return 0;

}

int
ReadFileToArray (
  int **pArray
)
{
  int num;
  int ttsize; 
  long ttInv;
  int poolSize;
  int collect;
  int *tt;
  int *ttAlt;
  char file_name[100];
  FILE *fp;
  char ch;
  int i;

  printf("Sort numbers by Quick Sort. Type File name:");
  gets (file_name);
  fp = fopen (file_name, "r");  // read mode
  if (fp == NULL) {
    printf ("Error while opening the file.\n");
    return 0;
  }
  poolSize = 0x7FFF0000;
  tt = malloc (sizeof(int) * poolSize);
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
    } else {
      //printf ("Got non-numeric char!\n");

      //
      // If get a non-numeric char, put num into array.
      //

      if (i >= poolSize) {

        //
        // If array size is not enought, reallocate to double its size.
        //

        printf ("poolSize (%d) not enough, double it!\n", poolSize);
        ttAlt = tt;
        poolSize *= 2;
        tt = malloc (poolSize);
        memcpy (tt, ttAlt, poolSize/2);
        free (ttAlt);
      }
      if (collect == 1) {
        collect = 0;
        printf ("%d\n", num);
        tt [i] = num;
        num = 0;
        i++;
      }
    }
  }
  fclose (fp);
  ttsize = i;
  //ttsize = 999;
  *pArray = tt;
  return ttsize;

}


int 
main (
  int argc, 
  const char * argv[]
  )
{
  int *inputArray;
  int arraySize;

  arraySize = ReadFileToArray (&inputArray);
  //printArray (inputArray, 0, arraySize-1);
  gComparison = 0;
  Partition (inputArray, 0, arraySize-1);
  printArray (inputArray, 0, arraySize-1);
  printf ("arraySize = %d\n", arraySize);
  printf ("Comparied (%ld) times.\n", gComparison);

  return 0;
}
