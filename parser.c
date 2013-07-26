#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include "array.h"
#include "parser.h"

Arguments parse_args(int argc, char **argv, const char *options) {
    int c;
    Arguments inArgs;
    initArray(&inArgs.filenames, 1);
    initArray(&inArgs.tasks, 1);
    while((c = getopt(argc, argv, options)) != -1) {
      switch(c) {
        case 'f':
          insertArray(&inArgs.filenames, (int)optarg);
          break;
        case 't':
          insertArray(&inArgs.tasks, (int)optarg);
          break;
        case 'c':
          inArgs.cflag = 1;
          break;
        case 'p':
          inArgs.pflag = 1;
          break;
        case '?':
          if (isprint(optopt))
            fprintf(stderr, "Unknown option '-%c'.\n", optopt);
          else
            fprintf(stderr, "Option '-%c' requires an argument\n", optopt);
        default:
          return inArgs;
      }
  }
  return inArgs;
}
