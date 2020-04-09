#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            //my code here (error handling)
            if (seed <= 0) { //checking the seed for positivity
                printf("seed is a positive number\n");
                return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            //my code here (error handling)
            if (array_size <= 0) { //check the size of the array for positivity
                printf("array_size is a positive number\n");
                return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            //my code here (error handling)
            if (pnum <= 0) { //checking the number of sub-arrays for positivity
                printf("pnum is a positive number\n");
                return 1;
            }
            break;
          case 3:
            with_files = true;
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc)
  {
      printf("Has at least one no option argument\n");
      return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1)
  {
      printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n", argv[0]);
      return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

    int pipefd[2];
    pipe(pipefd);

    int sub_array_length= array_size/pnum; //length of the sub-arrays
    int i;

  for (i = 0; i < pnum; i++)
  {
    pid_t child_pid = fork();
    if (child_pid >= 0)
    {
      //successful fork
      active_child_processes += 1;
      if (child_pid == 0)
      {
        //child process
        struct MinMax min_max;
        //parallel somehow
        if (i!=pnum-1)
        {
            //search for the max and min in the sub-array
            min_max = GetMinMax(array, i*sub_array_length, (i+1)*sub_array_length); 
        }
        else  min_max = GetMinMax(array, i * sub_array_length, array_size);
        if (with_files)
        {
            //writing the received data to a file
            //use files here
            FILE * fp = fopen ("my_file.txt", "a");
            if (fp==0)
            {
                printf( "Could not open file\n" );
                return 1;
            }
            else
            {
                fwrite(&min_max, sizeof(struct MinMax), 1, fp);
            }
            fclose (fp);
        }
        else
        {
            //write to the pipe
            //use pipe here
            write(pipefd[1],&min_max,sizeof(struct MinMax));
        }
        return 0;
      }

    }
    else
    {
      printf("Fork failed!\n");
      return 1;
    }
  }

  while (active_child_processes > 0) 
  {
    //my code here
    close(pipefd[1]);
    wait(NULL);
    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (i = 0; i < pnum; i++)
  {
    int min = INT_MAX;
    int max = INT_MIN;

    struct MinMax _min_max;

    if (with_files)
    {
        //read from files
        FILE* fp = fopen("my_file.txt", "r");
        if (fp==0){
            printf( "Could not open file\n" );
            return 1;
        }
        else
        {
            fseek(fp, i*sizeof(struct MinMax), SEEK_SET);
            fread(&_min_max, sizeof(struct MinMax), 1, fp); 
        }
        fclose(fp);
    }
    else
    {
        // read from pipes
        read(pipefd[0], &_min_max, sizeof(struct MinMax));
    }
    min = _min_max.min; max = _min_max.max;

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}