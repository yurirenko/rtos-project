#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "./array.h"


int main(int argc, char const *argv[])
{

  Array filenames, tasks;
  initArray(&filenames, 1);
  initArray(&tasks, 1);
  int i;
  for (i = 1; i < argc; ++i)
  {
    //check if args are filenames
    if (isalpha(argv[i][0]))
    {
      insertArray(&filenames, (int)argv[i]);  
    }
    else {
      insertArray(&tasks, atoi(argv[i]));
    }

  }
  for (i = 0; i < filenames.used; ++i)
  {
    // printf("%s\n", (char*)filenames.array[i]);
  }
  for (i = 0; i < tasks.used; ++i)
  {
    printf("%i\n", tasks.array[i]);
  }
  return 0;
}