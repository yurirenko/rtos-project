#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lib/array.h"
#include "lib/parser.h"
#include "lib/utils.h"
#include "lib/cyclic.h"
#include "lib/threaded.h"





int main(int argc, char **argv)
{
  
  Arguments inArgs;
  inArgs = parseArgs(argc, argv, "t:f:cp");
  if (inArgs.filenames.used != inArgs.tasks.used)
  {
    fprintf(stderr, "Number of filenames and tasks is not equal.\n");
    exit(EXIT_FAILURE);
  }

  if (inArgs.cflag == 1)
  {
    generateCyclic(inArgs); 
  }
  else if (inArgs.pflag == 1) 
  {
    generateThreaded(inArgs);
  }

  return 0;
}