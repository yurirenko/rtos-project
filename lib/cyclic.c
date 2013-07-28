#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "array.h"
#include "parser.h"
#include "utils.h"
#include "cyclic.h"

int generateCyclic(Arguments inArgs) {

  int tasksCount;
  int minCyclesCount;
  int minCycle, majCycle;

  FILE *fp;

  char *line;
  size_t len = 0;
  ssize_t read;

  Array periods, wcets, tasksFreq, schedTable, delays;
  printf("%s\n", "tests");
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



  char *minCycleDef = "\n#define MINOR_CYCLE";
  char *majCycleDef = "\n#define MAJOR_CYCLE";
  char *table = "\nstatic uint8_t scheduling_table[] = ";

  FILE *out = fopen("out.c", "w");
  fp = fopen("base_code/cyclic/headers.c", "r");
  write(fp, out);
  fprintf(out, "%s %d\n", minCycleDef, minCycle);
  fprintf(out, "%s %d\n", majCycleDef, majCycle);
  fprintf(out, "%s {", table);
  for (i = 0; i < minCyclesCount; ++i)
  {
    if((i + 1) == minCyclesCount) 
      fprintf(out, " %d };\n\n", schedTable.array[i]);
    else
      fprintf(out, " %d,", schedTable.array[i]);
  }
  fp = fopen("base_code/cyclic/func.c", "r");
  write(fp, out);

  for (i = 0; i < inArgs.filenames.used; ++i)
  {
    fp = fopen((const char*)inArgs.filenames.array[i], "r");
    if (fp == NULL)
         exit(EXIT_FAILURE);
    while ((read = getline(&line, &len, fp)) != -1) {
         fprintf(out, "%s", line);
    }
    fprintf(out, "\n");

  }

  fp = fopen("base_code/cyclic/main.c", "r");
  write(fp, out);

  for (i = 0; i < tasksCount; ++i)
  {
    fprintf(out, "\n\t\t\t\tif (scheduled_tasks & %d)\t\t\t\t\t\n", (1u << i));
    fprintf(out, "\t\t\t\t\t\t%s();\n", strtok((char*)inArgs.filenames.array[i], "."));
  }
  fprintf(out, "\n\t\t\t\tt = (t + MINOR_CYCLE)%%MAJOR_CYCLE;\n");
  fprintf(out, "\t\t}");
  fprintf(out, "\t return 0;\n}");


//   int main(int argc, char *argv[])
// {
//     int res;
//     int t = 0;

//     res = start_periodic_timer(1000000, MINOR_CYCLE * 1000);
//     if (res < 0) {
//         perror("Start Periodic Timer");

//         return -1;
//     }

//     while(1) {
//         uint8_t scheduled_tasks;

//         wait_next_activation();
//         scheduled_tasks = scheduling_table[t / MINOR_CYCLE];
//         if (scheduled_tasks & 0x01) {
//       task1();
//         }
//         if (scheduled_tasks & 0x02) {
//       task2();
//         }
//         if (scheduled_tasks & 0x04) {
//       task3();
//         }
//         t = (t + MINOR_CYCLE) % MAJOR_CYCLE;
//     }

//     return 0;
  Array methodsNames;
  initArray(&methodsNames, 1);
  for (i = 0; i < inArgs.filenames.used; ++i)
  {
    insertArray(&methodsNames, (int)strtok((char*)inArgs.filenames.array[i], "."));
    printf("%s\n", (char*)methodsNames.array[i]);
  }
  

  // fprintf(f, "%s\n", );

  // char *resultStr1;
  // char *resultStr2;
  // asprintf(&resultStr1, "%s\n%s", header, fnanoSleep);
  // // printf("%s\n", resultStr1);

  // char *cyclesDef;
  // asprintf(&cyclesDef, "%s %i\n %s %i\n", minCycleDef, minCycle, majCycleDef, majCycle);
  // asprintf(&resultStr2, "%s\n %s", resultStr1, cyclesDef);
  // printf("%s\n", resultStr2);
  // char tableString[minCyclesCount];
  // for (i = 0; i < minCyclesCount; ++i)
  // {
  //   tableString[i] = (char)schedTable.array[i];
  // }
  // 
  // printf("%d\n", schedTable.array);
  return 0;
  
}