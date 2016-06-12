//
//  main.c
//  Algo005
//
//  Created by Ivy on 5/5/14.
//  Copyright (c) 2014 ___IVYCANT___. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

//
// Merge subroutine.
//

void
MergeCountInv (int *fh, int *sh, int s, int *mm, long *Inn)
{
  int i, j, k;
  long count;
  int sizeF, sizeS;

  count = 0; // count inversions.
  *Inn = 0; // initialize returned inversion count.
  sizeF = s - s/2;
  sizeS = s/2;

  for (i = 0, j = 0, k = 0; k < s; k++) {

    //
    // if i not drained yet,
    //

    if (i < sizeF) {

      //
      // if j has been drained, then choose i
      //

      if (j >= sizeS) {
        mm[k] = fh[i];
        i++;
      } else {
        
        // 
        // if j has not been drained
        //

        if (fh[i] < sh[j]) {

          //
          // and fh[i] < sh[j], choose i,
          //

          mm[k] = fh[i];
          i++; 
        } else if (fh[i] == sh[j]) {
          mm[k] = fh[i];
          i++;
          k++;
          mm[k] = sh[j];
          j++;
        } else if (fh[i] > sh[j]) {
          mm[k] = sh[j];
          // Inversion! i<j and A[i]>A[j]. 
          count = count + (sizeF - i) ;
          j++;
        }
      }
    } else {
      mm[k] = sh[j];
      j++;
    }
  }
  *Inn = count;
  printf ("%ld\n", *Inn);
  return;
}

//
// Given an input array of size "size", sort it and return the sorted array.
//
void
SortIt (int *target, int size, long *Inv) 
{
  int i;
  int size1, size2;
  long leftInv, rightInv, splitInv;
  int *left;
  int *right;
  int *temp;
 
  leftInv = 0;
  rightInv = 0;
  splitInv = 0;
  *Inv = 0;

  if (size == 1) { // base case
    return;
  }

  temp = malloc (size * sizeof (int));

  size2 = size / 2;
  size1 = size - size2;
  
  left = malloc (size1 * sizeof (int));
  right = malloc (size2 * sizeof (int));

  for (i = 0; i < size1; i++) {
    left[i] = target[i]; 
  }
  for (i = 0; i < size2; i++) {
    right[i] = target[size1 + i];
  }
  SortIt (left, size1, &leftInv);
  SortIt (right, size2, &rightInv);

  MergeCountInv (left, right, size, temp, &splitInv);
  *Inv = leftInv + rightInv + splitInv;

  while (size != 0) {
    size--;
    target [size] = temp[size];
  }
  free (left);
  free (right);
  free (temp);
  return;
}

int 
main (int argc, const char * argv[])
{
  int i;
  int num;
  int *tt;
  int *ttAlt;
  FILE *fp;
  char file_name[100];
  char ch;
  int ttsize; 
  long ttInv;
  int poolSize;
  int collect;

  ttInv = 0;
  collect = 0;
  poolSize = 0x7FFF0000;

  printf("Count inversion. File name:");
  gets (file_name);
  fp = fopen (file_name, "r");  // read mode
  if (fp == NULL) {
    printf ("Error while opening the file.\n");
    return 0;
  }
  //printf ("size of int is (0x%lx)\n", sizeof (int));
  //printf ("INT_MAX is (0x%x)\n", INT_MAX);

  i = 0;
  num = 0;
  ch = 0;
  tt = malloc (sizeof(int) * poolSize);
  //printf ("array: \n");
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
        //printf ("%d\n", num);
        tt [i] = num;
        num = 0;
        i++;
      }
    }
  }
  fclose (fp);

  ttsize = i;
  //printf ("\n\n\nBefore sorted, array is ..");
  //for (i = 0; i < ttsize; i++) {
    //printf ("%d, ", tt[i]); 
  //}

  SortIt(tt, ttsize, &ttInv);

  //printf ("After sorted, array is ..");
  //for (i = 0; i < ttsize; i++) {
    //printf ("%d\n", tt[i]); 
  //}
  printf ("# of Inversion is (%ld)\n", ttInv);
  return 0;
}
