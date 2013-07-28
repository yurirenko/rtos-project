#include <stdio.h>
#include <stdlib.h>
#include "utils.h"


void write(FILE *fp, FILE *out) {
  char *line;
  size_t len = 0;
  ssize_t read;
  while((read = getline(&line, &len, fp)) != -1) {
    fprintf(out, "%s", line);
  }
}

int compare(const void* a, const void* b)
{
  int int_a = * ( (int*) a );
  int int_b = * ( (int*) b );

  if ( int_a == int_b ) return 0;
  else if ( int_a < int_b ) return -1;
  else return 1;
}

int gcd(int a, int b)
{
  if (a==0) return b;
    return gcd(b%a, a);
}

int gcds(int l, int *a) 
{
  int i, result;
  result = 1;
  for (i = 0; i < l; i++)
    result = gcd(result, a[i]);
  return result;
}

int lcm(int a, int b)
{
  return a/gcd(a, b) * b;
}

int lcms(int l, int * a)
{
  int i, result;
  result = 1;
  for (i = 0; i < l; i++) 
    result = lcm(result, a[i]);
  return result;
}