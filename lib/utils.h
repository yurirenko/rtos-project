#ifndef UTILS_H_ 
#define UTILS_H_

#include <stdio.h>


void write(FILE *fp, FILE *out);
int gcd(int a, int b);
int gcds(int l, int *a);
int lcm(int a, int b);
int lcms(int l, int * a);
int compare(const void* a, const void* b);

#endif