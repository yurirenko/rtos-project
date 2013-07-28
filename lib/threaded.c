#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "threaded.h"
#include "parser.h"
#include "array.h"
#include "utils.h"


int findPeriod(Array periods, int * oldPeriods, int i) {
  int j;
  for (j = periods.used-1; j >= 0; --j)
  {
      // printf("%i - %i\n", oldPeriods[j], periods.array[k]);

      if (oldPeriods[i] == periods.array[j])
      {
        printf("Equals! - %i - %i, i=%i, j=%i\n", oldPeriods[i], periods.array[j], i, j);
        return j;
        // printf("temp: %i\n", temp);
      }

  }
  return -1;
}

int generateThreaded(Arguments inArgs) {

  Array periods, wcets;

  int tasksCount = inArgs.filenames.used;
  int i;
  float *utilization = malloc(sizeof(float) * tasksCount);
  float totalUtil, ulub;
  initArray(&periods, tasksCount);
  initArray(&wcets, tasksCount);

  for (i = 0; i < tasksCount; ++i)
  {
      insertArray(&periods, atoi(strtok((char*)inArgs.tasks.array[i], ":")));
      insertArray(&wcets, atoi(strtok(NULL, ":")));   
  }



  for (i = 0; i < tasksCount; ++i)
  {
    utilization[i] = (float)wcets.array[i]/(float)periods.array[i];
  }


  for (i = 0; i < tasksCount; ++i)
  {
    totalUtil += utilization[i];
  }

  ulub = (float)tasksCount * (pow(2.0, 1.0/(float)tasksCount) - 1.0);
  // printf("%f\n", ulub);
  if (totalUtil > 1)
  {
    printf("%s\n", "Task is not schedulable.");
    exit(EXIT_FAILURE);
  }
  else {
    if(totalUtil <= ulub) {
      printf("%s\n", "Task is schedulable.");
    }
    else {
      printf("%s\n", "Task is not schedulable.");
      exit(EXIT_FAILURE);
    }
  }

  FILE *out = fopen("out.c", "w");
  FILE *fp;

  char *line;
  size_t len = 0;
  ssize_t read;


  fp = fopen("base_code/threaded/headers.c", "r");
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

  fp = fopen("base_code/threaded/func.c", "r");
  write(fp, out);
  for (i = 0; i < tasksCount; ++i)
  {
    fprintf(out, "\nvoid *thread%i(void *param)\n{", i);  
    fprintf(out, "\n\t\tperiodic_task_t p_d;");
    fprintf(out, "\n\t\tstart_periodic_timer(2000000, %d, &p_d);", periods.array[i]*1000);
    fprintf(out, "\n\t\twhile (1) \n\t\t{ \n\t\t\twait_next_activation(&p_d);");
    fprintf(out, "\n\t\t\t%s();\n\t\t}\n}", strtok((char*)inArgs.filenames.array[i], "."));
  }
  fprintf(out, "\nint main(int argc, char *argv[])\n{");
  fprintf(out, "\n\t\tint err; \n\t\tvoid *returnvalue;\n\t\tpthread_t ");
  for (i = 0; i < tasksCount; ++i)
  {
    if((i + 1) == tasksCount) 
      fprintf(out, " _%iid;", i);
    else
      fprintf(out, " _%iid,", i);
  }
  fprintf(out, "\n");
  fp = fopen("base_code/threaded/main_part.c", "r");
  write(fp, out);
  int *oldPeriods = malloc(sizeof(int)*periods.used);
  for (i = 0; i < periods.used; ++i)
  {
    oldPeriods[i] = periods.array[i];
  }
  //sort the threads by periods
  qsort(periods.array, periods.used, sizeof(int), compare);
  int priority = 1;
  for (i = periods.used-1; i >= 0; --i)
  {
    printf("%i\n", periods.array[i]);
    fprintf(out, "\n\n\t\tsp.sched_priority = pmin + %i;\n", priority);
    fprintf(out, "\t\terr = pthread_attr_setschedparam(&attrs, &sp);\n");    
    fprintf(out, "\t\tif (err != 0)\n \t\t{ \n\t\t\tperror(\"setschedparam\");\n\t\t}");  
    int temp = findPeriod(periods, oldPeriods, i);

    printf("temp: %i\n", temp);
    fprintf(out, "\n\t\terr = pthread_create(&_%iid, &attrs, thread%i, (void *)NULL);\n", temp, temp);
    fprintf(out, "\n\t\tif (err != 0) \n\t\t{\n\t\t\tperror(\"thread %i\");\n\t\t}", i);
    priority++;
  }

  for (i = 0; i < tasksCount; ++i)
  {
    fprintf(out, "\n\t\tpthread_join(_%iid, &returnvalue);", i);
  }

  fprintf(out, "\n\n\t\treturn 0;\n}");

  

  
 
  return 0;
}