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
  Array periods, wcets, tasksFreq, schedTable, delays;

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

  // for (i = 0; i < tasksCount; ++i)
  // {
  //   printf("Period: %d; WCET: %d task: %s\n", periods.array[i], wcets.array[i], (char*)inArgs.filenames.array[i]);
  // }



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
  int wcetsSum = 0;
  initArray(&delays, tasksCount);
  Array oldPeriods;
  initArray(&oldPeriods, 1);
  for (i = 0; i < tasksCount; ++i)
  {
    printf("%d\n",periods.array[i] );
    oldPeriods.array[i] = periods.array[i];
  }
  int j, k;
  for (i = 0; i < minCyclesCount; ++i)
  {
    int schedTemp = 0;
    for (j = 0; j < tasksCount; ++j)
    {
      if (tasksFreq.array[j] > 0)
      {
        // if (time == 0) 
        // {
        //     schedTemp |= (int)(1u << j);
        //     (tasksFreq.array[j])--; 
        // }
        // else 
        // {

            printf("time: %d, period: %d, task: %d, delay: %d\n", time, periods.array[j], j, delays.array[j]);
            if(((time-delays.array[j])%periods.array[j]) == 0) {
              for (k = 0; k < tasksCount; ++k)
              {
                // printf("%d, %d\n", schedTemp, (1u << k));
                if ((schedTemp & (1u << k)))
                {
                  wcetsSum += wcets.array[k];  
                }
              }
              // printf("%d + %d = %d\n", wcetsSum, wcets.array[j], (wcetsSum+wcets.array[j]));
              if ((wcetsSum + wcets.array[j]) <= minCycle)
              {
                printf("%d, cycle: %d\n", j+1, i);

                if(delays.array[j] != 0) {
                  printf("%s\n", "what");
                  periods.array[j] += delays.array[j];
                  delays.array[j] = 0;  
                  printf("%d\n", delays.array[j]);
                }
                schedTemp |= (int)(1u << j);

                
                (tasksFreq.array[j])--;  
                // delays.array[j] = 0;
              }
              else {
                printf("%d\n", periods.array[j]);
                printf("%d\n", oldPeriods.array[j]);
                if (periods.array[j] == oldPeriods.array[j])
                {
                  delays.array[j] += minCycle;
                  printf("Delayed by: %d (task %d), cycle: %d, time: %d\n", delays.array[j], j+1, i, time);
                }
                
              }
              wcetsSum = 0;
            }
        // }        
      }
      // printf("sched temp: %d\n", schedTemp);

    }
    time += minCycle;
    insertArray(&schedTable, schedTemp);
  }
  for (i = 0; i < tasksCount; ++i)
  {
    printf("%d\n", oldPeriods.array[i]);
  }
  for (i = 0; i < minCyclesCount; ++i)
  {
    
  }

  for (i = 0; i < schedTable.used; ++i)
  {
    printf("%d\n", (int)schedTable.array[i]);
  }
  printf("%s\n", "frequencies:");
  for (i = 0; i < tasksFreq.used; ++i)
  {
    if (tasksFreq.array[i] != 0)
    {
      printf("%s\n", "Can't generate schedule table, probably, the WCETs of tasks are too big.");
      exit(EXIT_FAILURE);
    }
    printf("f %d\n", tasksFreq.array[i]);
  }
  // printf("number of min cycles: %d\n", minCyclesCount);
  // // 
  // // reading file

  // for (i = 0; i < inArgs.filenames.used; ++i)
  // {
  //   fp = fopen((const char*)inArgs.filenames.array[i], "r");
  //   if (fp == NULL)
  //        exit(EXIT_FAILURE);
  //   while ((read = getline(&line, &len, fp)) != -1) {
  //        printf("%s", line);
  //    }
  // }
  // for (i = 0; i < inArgs.tasks.used; ++i)
  // {
  //   printf("%s\n", (char*)inArgs.tasks.array[i]);
  // }

  char *header = "#include <sys/time.h> \
                  #include <time.h> \
                  #include <stdint.h> \
                  #include <stdio.h> "
  return 0;
}