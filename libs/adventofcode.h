//V1.0

#ifndef ADVENT_OF_CODE_BLOEM_H
#define ADVENT_OF_CODE_BLOEM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <inttypes.h>

#include "bloem.h"
#include "queue.h"
#include "debug.h"
#include "hash.h"

int directions[2][8] = {{1, -1, 0, 0, 1, 1, -1, -1}, {0, 0, 1, -1, 1, -1, 1, -1}};

extern void run_parts(clock_t begin);
void print_binary(void *n, size_t size, bool with_zero);
extern long long unsigned str_to_llu(char *vstring);
extern long long str_ll(char *vstring);
extern char **str_split(char* a_str, const char a_delim, bool doublechar);
extern char *searchAndReplace(char *text, char *search, char *replace);

void part1();
void part2();

#endif //ADVENT_OF_CODE_BLOEM_H