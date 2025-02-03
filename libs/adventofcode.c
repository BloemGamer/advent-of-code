//V1.0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <inttypes.h>

#include "adventofcode.h"
// #include "queue.c"
// #include "debug.c"
// #include "bloem.c"
// #include "hash.c"

int directions[2][8] = {{1, -1, 0, 0, 1, 1, -1, -1}, {0, 0, 1, -1, 1, -1, 1, -1}};


void run_parts(clock_t begin)
{
    clock_t begin1 = clock();
	printf("\nThings for 1 and 2: %.0lfms\n\n", (double)(begin1 - begin)/CLOCKS_PER_SEC*1000);
	part1();
	clock_t end1 = clock();
	printf("\n%.0lfms\n\n", (double)(end1 - begin1)/CLOCKS_PER_SEC*1000);
	part2();
	clock_t end2 = clock();
	printf("\n%.0lfms", (double)(end2 - end1)/CLOCKS_PER_SEC*1000);
}

void print_binary(void *n, size_t size, bool with_zero)
{
	size = size * 8 - 1;
	printf("0b");
	if(!with_zero)
		while(((*(long long unsigned*)n & ((long long unsigned)1 << size)) == 0) && (size > 0))
			size--;
	while(size + 1 >= 1)
		printf("%llu", (*(long long unsigned*)n & (long long unsigned)1 << size) >> size--);
}

bool in_bounds(long long y, long long min_y, long long max_y, long long dif_y, long long x, long long min_x, long long max_x, long long dif_x)
{
	if(y + dif_y >= min_y && y + dif_y <= max_y && x + dif_x >= min_x && x + dif_x <= max_x)
		return true;
	else
		return false;
}

long long unsigned str_to_llu(char *vstring)
{
	long long unsigned i = 0, number  = 0;
	char game[4096];
	strcpy(game, vstring);
	while(true)
	{
		if(game[i] >= '0' && game[i] <= '9')
			{
				number = 10 * number + (game[i] - '0');
			}
			else
			{
				return number;
			}   
		i++;
	}
}

long long str_ll(char *vstring)
{
	int i = 0;
	long long number = 0;
	char game[4096];
	bool negative = false;
	strcpy(game, vstring);
	while(true)
	{
		if(game[i] == '-')
		{
			negative = true;
			i++;
		}
		if(game[i] >= '0' && game[i] <= '9')
			{
				number = 10 * number + (game[i] - '0');
			}
			else if(negative == true)
			{
				return (number * -1);
			}  
			else
			{
				return number;
			} 
		i++;
	}
}

char** str_split(char* a_str, const char a_delim, bool doublechar) 
{
	char** result    = 0;
	long long unsigned count     = 0;
	char* tmp        = a_str;
	char* last_comma = 0;
	char delim[2];
	delim[0] = a_delim;
	delim[1] = 0;

	while (*tmp)
	{
		if (a_delim == *tmp)
		{
			count++;
			last_comma = tmp;
		}
		tmp++;
	}

	count += last_comma < (a_str + strlen(a_str) - 1);

	count++;

	result = (char**)malloc(sizeof(char*) * count);

	if(result)
	{
		long long unsigned idx  = 0;
		char *token = strtok(a_str, delim);

		while(token)
		{
			assert(idx < count);
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		if(doublechar == true)
			assert(idx == count - 1);
		*(result + idx) = 0;
	}

	return result;
}

char *searchAndReplace(char *text, char *search, char *replace)
{
	char buffer[4096];
	char *ptr;
	char *modText = NULL;

	buffer[0] ='\0';
	while((ptr = strstr(text, search)))
	{
		strncat(buffer, text, ptr - text);
		strcat(buffer, replace);

		text = ptr + strlen(search);
	}
	strcat(buffer, text);

	modText = (char*)malloc(strlen(buffer) + 1);
	strcpy(modText, buffer);
	return modText;
}