#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "array.h"
#include "parser.h"


int main(int argc, char **argv)
{
  Arguments inArgs;
  inArgs = parse_args(argc, argv, "t:f:cp");
  int i;
  for (i = 0; i < inArgs.filenames.used; ++i)
  {
    printf("%s\n", (char*)inArgs.filenames.array[i]);
  }
  for (i = 0; i < inArgs.tasks.used; ++i)
  {
    printf("%s\n", (char*)inArgs.tasks.array[i]);
  }
  return 0;
}