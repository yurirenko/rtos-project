#ifndef PARSER_H_ 
#define PARSER_H_

#include "array.h"

typedef struct 
{
  Array filenames;
  Array tasks;
  int cflag;
  int pflag;
} Arguments;

Arguments parseArgs(int argc, char **argv, const char *options);



#endif