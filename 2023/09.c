//V3.2

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "..\libraries\adventofcode.c"

//too low 1551709655
//too low 1533864819
//too low 1533864819

//#define TEST1
//#define TEST2

#ifdef TEST1
    #define filename "txt/9.test1.txt"
#elif defined TEST2
	#define filename "txt/9.test2.txt"
#else
    #define filename "txt/9.txt"
#endif

#define stringlength 4096
#define arraylength 128

void part1();
void part2();

int difference();
int difference2();
int str_to_int();
void fill_int_min();

int main(void)
{
	clock_t begin = clock();
	file = readfile(filename);

	
	clock_t begin1 = clock();
	printf("\n%s\nThings for 1 and 2: %.0lfms\n\n", filename, (double)(begin1 - begin));
	part1();
	clock_t end1 = clock();
	printf("\n%.0lfms\n\n", (double)(end1 - begin1));
	part2();
	clock_t end2 = clock();
	printf("\n%.0lfms", (double)(end2 - end1));
}

void part1()
{
	long long int answer = 0;
	char game[stringlength];
	char **tokens;
	int numbers[arraylength];
	size_t j;

	for(size_t i = 0; i < file.lengthfile; i++)
	{
		j = 0;
		fill_int_min(numbers);
		strcpy(game, file.file[i]);
		tokens = str_split(game, ' ', false);
		while(*(tokens + j) != NULL)
		{
			numbers[j] = str_to_int(*(tokens + j));
			j++;
		}
		answer = answer + difference(numbers);
	}


	printf("Part 1: %llu", answer);
}

void part2()
{
	long long int answer = 0;
	char game[stringlength];
	char **tokens;
	int numbers[arraylength];
	size_t j;

	for(size_t i = 0; i < file.lengthfile; i++)
	{
		j = 0;
		fill_int_min(numbers);
		strcpy(game, file.file[i]);
		tokens = str_split(game, ' ', false);
		while(*(tokens + j) != NULL)
		{
			numbers[j] = str_to_int(*(tokens + j));
			j++;
		}
		answer = answer + difference2(numbers);
	}


	printf("Part 2: %llu", answer);
}


int difference(int *vstring)
{
	int diff[arraylength];
	fill_int_min(diff);
	int i = 0;
	bool all_zero = true;
	for(int j = 0; j < arraylength; j++)
	{
		if(vstring[j] == INT_MIN)
		{
			break;
		}
		if(vstring[j] != 0)
		{
			all_zero = false;
			break;
		}
	}
	if(all_zero == true)
	{
		return 0;
	}
	while(vstring[i + 1] != INT_MIN)
	{
		diff[i] = vstring[i + 1] - vstring[i];
		i++;
	}
	return vstring[i] + difference(diff);
}

int difference2(int *vstring)
{
	int diff[arraylength];
	fill_int_min(diff);
	int i = 0;
	bool all_zero = true;
	for(int j = 0; j < arraylength; j++)
	{
		if(vstring[j] == INT_MIN)
		{
			break;
		}
		if(vstring[j] != 0)
		{
			all_zero = false;
			break;
		}
	}
	if(all_zero == true)
	{
		return 0;
	}
	while(vstring[i + 1] != INT_MIN)
	{
		diff[i] = vstring[i + 1] - vstring[i];
		i++;
	}
	return vstring[0] - difference2(diff);
}

int str_to_int(char *vstring)
{
	int i = 0;
	int number  = 0;
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

void fill_int_min(int *vstring)
{
	for(size_t i = 0; i < arraylength; i++)
	{
		vstring[i] = INT_MIN;
	}
}