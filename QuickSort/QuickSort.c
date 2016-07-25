//
//  QuickSort.c
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
char gPivotPolicy;

#define CHOOSE_PIVOT_LEFTMOST 'l'
#define CHOOSE_PIVOT_RIGHTMOST 'r'
#define CHOOSE_PIVOT_MEDIAN 'm'

//
//  Description: 
//    print array with comma and semicolon
//

void 
printArray (
  int *A,
  int l ,
  int r, 
  int commaBefore, 
  int semicolonBefore
  )
{
  int i;
  i = 0;
  
  for (i = l;i <= r; i++){
    if (i == commaBefore) {
      printf (",");
    }
    if (i == semicolonBefore) {
      printf (";");
    }
    printf ("%d ", A[i]);
  }
  printf ("\n");
 
}

//
//  Description: 
//    swap two elements in an array
//

void 
Swap (
  int* A, 
  int index1, 
  int index2
  ) 
{
  int temp;
  temp = A[index1];
  A [index1] = A [index2];
  A [index2] = temp;
}

//
//  Description: 
//    Choose a pivot vased on pivot policy passed in as
//    the fourth parameter. There are 3 options.
//    1) CHOOSE_PIVOT_LEFTMOST if user input 'l'
//    2) CHOOSE_PIVOT_RIGHTMOST if user input 'r'
//    3) CHOOSE_PIVOT_MEDIAN if user input 'm'
//    If not specified, then CHOOSE_PIVOT_LEFTMOST policy is used.
//

int 
ChoosePivot (
  int* inputArray, 
  int leftmost, 
  int rightmost, 
  int policy
  )
{
  int index;
  int a,b,c;
  int middleIndex;
  switch (policy) {
    case CHOOSE_PIVOT_RIGHTMOST:
      index = rightmost;
      break;
    case CHOOSE_PIVOT_MEDIAN:
      printf ("!!Choose kinda 'median' from the following array...\n");
      printf ("  ");
      printArray (inputArray, leftmost, rightmost, leftmost, leftmost);
      a = inputArray [leftmost];
      c = inputArray [rightmost];
      middleIndex = (rightmost-leftmost)/2 + leftmost;
      if (middleIndex == 0) {
        printf ("  ");
        printf ("%dth, %dth\n", leftmost, rightmost);
        printf ("  ");
        printf ("%d  , %d  \n", a, c);
        return leftmost;
      } else {
        b = inputArray [middleIndex];
      }
      printf ("  ");
      printf ("%dth, %dth, %dth\n", leftmost, middleIndex, rightmost);
      printf ("  ");
      printf ("%d  , %d  , %d  \n", a, b, c);
      if ((a > b && a < c) || (a > c && a < b)) {
        index = leftmost;
      } else if ((b > a && b < c) || (b > c && b < a)) {
        index = middleIndex;
      } else {
        index = rightmost;
      }
      break;
    case CHOOSE_PIVOT_LEFTMOST:
    default:
      index = leftmost; 
      
  }

  return index;
}

void
Partition (
  int *A, 
  const int l, 
  const int r)
{
  int i; // leftmost index of the half bigger than the pivot
  int j; // leftmost index of unpartitioned half
  int k;
  int pivotIndex;
  int pivot;
  int length;

  printf ("< Partition begin for A[%d] ~ A[%d] ......\n", l, r);

  if (l >= r) {
    return;
  }

  length = r - l + 1;
  printf ("# Comparison = %ld -> ", gComparison);
  gComparison += length - 1;
  printf ("%ld\n", gComparison);

  pivotIndex = ChoosePivot(A, l, r, gPivotPolicy);
  pivot = A[pivotIndex];
  
  printf (" pivot: A[%d]=%d\n", pivotIndex, pivot);
  
  //
  // If pivot is not the leftmost index,
  // swap it with the leftmost index
  //

  if (pivotIndex != l) {
    Swap (A, l, pivotIndex);
  }

  i = l + 1; 
  j = l + 1;
  for (; j <= r; j++) {
    /*
    printf ("  Start to look at unpartitioned half start from A[%d] = %d\n", j, A[j]);
    printf ("  ");
    printArray (A, l, r, i, j);
    */
    if (A[j] < pivot) {

      //
      // Swap the element smaller than pivot
      // with the leftmost element of the bigger half
      //
      
      //printf ("  A[%d]=%d < pivot %d. Let's swap with the leftmost of the big half A[%d]=%d!\n", j, A[j], pivot, i, A[i]);
      Swap (A, i, j);
      
      i++; // advance the bigger half leftmost index
    }
  } 
  Swap (A, l, i - 1);
  
  printf (".. Partition ends with pivot A[%d] = %d\n", i - 1, A[i - 1]);
  printf (".. ");
  printArray (A, l, r, i, j);
 
  Partition (A, l, i - 2);
  Partition (A, i, r);
}

int 
__main (
  int argc, 
  const char * argv[]
  )
{
  //int inputArray[] = {3,7,2,5,1,4,7,6};
  int inputArray[] = {3,8,2,5,1,4,7,6};

  printf ("Choose a pivot policy: \n('l' = leftmost; 'r' = rightmost; 'm' = median specified by assignment): ");
  gPivotPolicy = getchar();
  getchar();
  printf ("getchar '%c'\n", gPivotPolicy);

  gComparison = 0;
  Partition (inputArray, 0, 7);
  printArray (inputArray, 0, 7,7,7);
  printf ("Total comparison time (%ld)\n", gComparison);

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
  char file_name[100] = "";
  char *defaultFileName = "ArrayToBeSorted.txt";
  FILE *fp;
  char ch;
  int i;

  printf("Sort numbers by Quick Sort. Type File name (default is %s): ", defaultFileName);
  gets (file_name);
  if (strncpy (file_name, "", 1)) { 
    fp = fopen (defaultFileName, "r");  // read mode
  } else {
    fp = fopen (file_name, "r");  // read mode
  }
  if (fp == NULL) {
    printf ("Error while opening the file.\n");
    return -1;
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

  printf ("Choose a pivot policy: \n('l' = leftmost; 'r' = rightmost; 'm' = median specified by assignment: )");
  gPivotPolicy = getchar();
  getchar();
  printf ("getchar '%c'\n", gPivotPolicy);
  
  arraySize = ReadFileToArray (&inputArray);
  //printArray (inputArray, 0, arraySize-1, arraySize - 1);
  if (arraySize < 1) {
    printf ("\nCan't get input array\n\n");  
    return 0;
  }
  gComparison = 0;
  Partition (inputArray, 0, arraySize - 1);
  printArray (inputArray, 0, arraySize - 1, arraySize - 1, arraySize - 1);

  printf ("Summary ...\n");
  printf ("  ");
  printf ("arraySize = %d\n", arraySize);
  printf ("  ");
  printf ("Pivot policy is '%c'\n", gPivotPolicy);
  printf ("  ");
  printf ("Comparied (%ld) times.\n", gComparison);

  //printf ("size of int is (%ld)\n", sizeof(int));

  return 0;
}
