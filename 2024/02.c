#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "..\libraries\adventofcode.c" //V5.1
#include "..\libraries\queue.h" //V4.2
#define _DEBUG
#include "..\libraries\debug.h" //V0.1

//to high 731
//to high 340
//wrong 325
//wrong 332
//goede 337

struct Data
{
	long long **numbers;
	bool *possible;
	bool saved;
};

struct Data data;

void part1();
void part2();

void fill_data();
long long is_safe();
long long is_safe2();

int main(int argc, char *argv[])
{
	clock_t begin = clock();

	data.numbers = calloc(file.amountlines + 1, sizeof(long long*));
	data.possible = calloc(file.amountlines + 1, sizeof(bool));
	
	fix_file(argv, "M");

	fill_data();

	clock_t begin1 = clock();
	printf("\nThings for 1 and 2: %.0lfms\n\n", (double)(begin1 - begin)/CLOCKS_PER_SEC*1000);
	part1();
	clock_t end1 = clock();
	printf("\n%.0lfms\n\n", (double)(end1 - begin1)/CLOCKS_PER_SEC*1000);
	part2();
	clock_t end2 = clock();
	printf("\n%.0lfms", (double)(end2 - end1)/CLOCKS_PER_SEC*1000);

	return 0;
}

void part1() 
{
	long long answer = 0;

	for(size_t i = 0; i < file.amountlines; i++)
	{
		if(is_safe(data.numbers[i]) == 0)
		{
			answer++;
			data.possible[i] = true;
		}
		else
			data.possible[i] = false;
	}

	printf("Part 1: %lld", answer);
}

void part2()
{
	long long answer;

	for(size_t i = 0; i < file.amountlines; i++)
	{
		if(is_safe2(i) == 0)
			answer++;
	}

	printf("Part 2: %lld", answer);
}


void fill_data(void)
{
	char game[4095];
	char **tokens;
	size_t j;

	for(size_t i = 0; i < file.amountlines; i++)
	{
		strcpy(game, file.file[i]);
		j = 0;
		data.numbers[i] = malloc(11 * sizeof(long long));
		tokens = str_split(game, ' ', false);
		while(*(tokens + j) != NULL)
		{
			data.numbers[i][j] = str_ll(*(tokens + j));
			j++;
			assert(!(j >= 10));
		}
		while(j < 11)
		{
			data.numbers[i][j] = LONG_LONG_MIN;
			j++;
		}
	}
}

long long is_safe(long long numbers[])
{
	long long increasing = 0;
	long long decreasing = 0;
	size_t i = 0;
	
	for(size_t j = 0; j < 10 - 1; j++)
	{
		if(numbers[j + 1] == LONG_LONG_MIN)
			break;
		if(numbers[j] - numbers[j + 1] < -3 || numbers[j] - numbers[j + 1] > 3 || numbers[j] - numbers[j + 1] == 0)
		{
			return 1;
		}
		if(numbers[j] - numbers[j + 1] < 0)
		{
			decreasing++;
		}
		if(numbers[j] - numbers[j + 1] > 0)
		{
			increasing++;
		}
		if(increasing > 0 && decreasing > 0)
		{
			return 1;
		}
	}
	return 0;
}

long long is_safe2(size_t place)
{
	if(data.possible[place] == true)
		return 0;
	
	long long game[10];
	int k;

	for(size_t i = 0; i < 10; i++)
	{
		for(size_t j = 0; j < 10; j++)
		{
			if(j == i)
				k = 1;
			game[j] = data.numbers[place][j + k];
		}
		if(is_safe(game) == 0)
			return 0;
		k = 0;
	}

	return 1;
}