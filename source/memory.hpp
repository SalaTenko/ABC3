#ifndef MEMORY
#define MEMORY

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

int Process_parameters(int argc, char *argv[], char* mem_type,
        long long &mem_size, long long &launch_cnt,
        bool &max_buffer);
int Tests_handler(char* mem_type, long long launch_cnt,
        long long mem_size, bool max_buffer);

#endif
