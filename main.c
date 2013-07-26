#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "array.h"
#include "parser.h"



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


int main(int argc, char **argv)
{
  int minCyclesCount;
  int minCycle, majCycle;
  FILE *fp;
  int tasksCount;
  char *line;
  Array periods, wcets, tasksFreq, schedTable;

  size_t len = 0;
  ssize_t read;
  Arguments inArgs;
  inArgs = parseArgs(argc, argv, "t:f:cp");
  if (inArgs.filenames.used != inArgs.tasks.used)
  {
    fprintf(stderr, "Number of filenames and tasks is not equal.\n");
    exit(EXIT_FAILURE);
  }
  tasksCount = inArgs.filenames.used;
  initArray(&periods, tasksCount);
  initArray(&wcets, tasksCount);


  int i;
  for (i = 0; i < tasksCount; ++i)
  {
      insertArray(&periods, atoi(strtok((char*)inArgs.tasks.array[i], ":")));
      insertArray(&wcets, atoi(strtok(NULL, ":")));    
  }


  int result = periods.array[0];
  for (i = 1; i < periods.used; i++)
  {
      result = gcd(result, periods.array[i]);
  }
  minCycle = result;
  majCycle = lcms(periods.used, periods.array);

  for (i = 0; i < tasksCount; ++i)
  {
    printf("Period: %d; WCET: %d task: %s\n", periods.array[i], wcets.array[i], (char*)inArgs.filenames.array[i]);
  }



  initArray(&tasksFreq, tasksCount);
  // for (i = 0; i < tasksCount; ++i)
  // {
  //   insertArray(&tasksFreq, majCycle/periods.array[i]);
  //   printf("%d\n", majCycle/periods.array[i]);
  // }

  minCyclesCount = majCycle/minCycle;
  initArray(&schedTable, minCyclesCount);
  for (i = 0; i < tasksCount; ++i)
  {
    insertArray(&tasksFreq, majCycle/periods.array[i]);
  }
  int time = 0;
  int j;
  for (i = 0; i < minCyclesCount; ++i)
  {
    int schedTemp = 0;
    for (j = 0; j < tasksCount; ++j)
    {
      if (tasksFreq.array[j] > 0)
      {
        if (time == 0)
        {
            schedTemp |= (int)(1u << j);
            (tasksFreq.array[j])--; 
        }
        else 
        {
            printf("time: %d, period: %d, task: %d\n", time, periods.array[j], j);
            if((time%periods.array[j]) == 0) {
              schedTemp |= (int)(1u << j);
              (tasksFreq.array[j])--;
            }
        }        
      }

    }
    time += minCycle;
    insertArray(&schedTable, schedTemp);

  }
  for (i = 0; i < schedTable.used; ++i)
  {
    printf("%d\n", (int)schedTable.array[i]);
  }
  printf("%s\n", "frequencies:");
  for (i = 0; i < tasksFreq.used; ++i)
  {
    printf("f %d\n", tasksFreq.array[i]);
  }
  printf("number of min cycles: %d\n", minCyclesCount);
  // 
  // reading file

  for (i = 0; i < inArgs.filenames.used; ++i)
  {
    fp = fopen((const char*)inArgs.filenames.array[i], "r");
    if (fp == NULL)
         exit(EXIT_FAILURE);
    while ((read = getline(&line, &len, fp)) != -1) {
         printf("%s", line);
     }
  }
  for (i = 0; i < inArgs.tasks.used; ++i)
  {
    printf("%s\n", (char*)inArgs.tasks.array[i]);
  }
  return 0;
}