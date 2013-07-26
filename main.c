#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "./array.h"



int main(int argc, char **argv)
{
  char *fvalue, *tvalue, *cvalue = NULL;
  int c;
  Array filenames, tasks;
  initArray(&filenames, 1);
  initArray(&tasks, 1);
  while((c = getopt(argc, argv, "t:f:c:")) != -1) {
    switch(c) {
      case 'f':
        insertArray(&filenames, (int)optarg);
        break;
      case 't':
        insertArray(&tasks, (int)optarg);
        break;
      case 'c':
        cvalue = optarg;
        break;
      case '?':
        if (isprint(optopt))
          fprintf(stderr, "Unknown option '-%c'.\n", optopt);
        else
          fprintf(stderr, "Option '-%c' requires an argument\n", optopt);
        return 1;
      default:
        abort();
    }
  }
  int i;
  for (i = 0; i < filenames.used; ++i)
  {
    printf("%s\n", (char*)filenames.array[i]);
  }
  for (i = 0; i < tasks.used; ++i)
  {
    printf("%s\n", (char*)tasks.array[i]);
  }

  printf("type: %s\n", cvalue);

  return 0;
}