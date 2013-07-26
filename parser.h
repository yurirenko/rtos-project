#ifndef PARSER_H_ 
#define PARSE_H_

#include "array.h"

typedef struct 
{
  Array filenames;
  Array tasks;
  int cflag;
  int pflag;
} Arguments;

Arguments parse_args(int argc, char **argv, const char *options);



#endif