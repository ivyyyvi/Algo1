#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void merge (int* array, int *left, int *right, int size) 
{
  int l = 0;
  int r = 0;
  printf ("Right before merge, ");
  for (int i = 0; i < size; i++) {
    if (i < size/2) {
      printf ("%d, ",left[i]);
    } else {
      printf ("%d, ",right[i -size/2]);
    }
  }
  for (int i = 0;i < size; i++) {
    if ((r >= (size -size/2)) || ((l < size/2) && (left [l] < right [r]))) {
      array [i] = left [l];
      l++;
    } else {
      array [i] = right [r];
      r++;
    }
  } 

  printf ("-->after merge: ");
  for (int i = 0; i < size; i++) {
    printf ("%d", array[i]);
    printf (", ");
  }
  printf ("\n");
}

void mergeSort (int* array, int size) 
{
  int* left;
  int* right;

  // base case
  if (size == 1) {
    return;
  }

  // divide the array into 2 arrays
  left = (int *) malloc (sizeof(array[0]) * (size /2));
  right = (int *) malloc (sizeof(array[0]) * (size - size /2));
  //left = (int *) malloc (size /2);
  //right = (int *) malloc (size - size /2);
  printf ("left (%p), right (%p)\n", left, right);
  for (int i = 0; i < size; i++) {
    if (i < size/2) {
      printf ("l%d (%d), ",i, array[i]);
      left[i] = array[i];
    } else {
      printf ("r%d (%d), ", i-size/2, array[i]);
      right[i - size/2] = array [i];
    }
  }
  printf ("--\n");

  mergeSort (left, size / 2);
  mergeSort (right, size - size / 2);
  merge (array, left, right, size);
  free (left);
  free (right);
  
  return; 
}

int main (int argc, char** argv) 
{
  int size = 9;
  int array[] = {5, 8, 1, 3, 4, 6, 7, 9, 2}; 

  printf ("original array:\n");
  for (int i = 0; i < sizeof (array)/sizeof (array[0]); i++) {
    printf ("(%d)", array[i]);
    printf (", ");
  }
  printf ("\n");
  mergeSort (array, size); 
  
  printf ("Sorted array:\n");
  for (int i = 0; i < sizeof (array)/sizeof (array[0]); i++) {
    printf ("(%d)", array[i]);
    printf (", ");
  }
  printf ("\n");


  return 0;
}
