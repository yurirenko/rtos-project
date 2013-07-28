#ifndef ARRAY_H_ 
#define ARRAY_H_
#include <stdlib.h>


// dynamic arrays
// from http://stackoverflow.com/a/3536261

typedef struct {
  int *array;
  size_t used;
  size_t size;
} Array;


void initArray(Array *a, size_t initialSize);
void insertArray(Array *a, int element);
void freeArray(Array *a);

#endif