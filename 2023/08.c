//V3.2

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "..\libraries\adventofcode.c"

//#define TEST1
//#define TEST2

#ifdef TEST1
    #define filename "txt/8.test1.txt"
	#define amount_starts 2
#elif defined TEST2
	#define filename "txt/8.txt1.txt"
	#define amount_starts 2
#else
    #define filename "txt/8.txt"
	#define amount_starts 6
#endif

#define arraylength 1024
#define stringlength 4096


struct Data
{
	char steps[stringlength];
	size_t start[arraylength];
	size_t left[arraylength];
	size_t right[arraylength];
	size_t value[amount_starts];
	size_t new_start[amount_starts];
	size_t amount_loops[amount_starts];
	size_t previous[amount_starts];
	bool already_end[amount_starts];
};

struct Data data;
struct Data data2;

void part1();
void part2();

void fill_data();
size_t string_size_t();
size_t check_steps();
void fill_data2();
size_t string_size_t2();
void check_steps2();
size_t amount_steps();


int main(void)
{
	clock_t begin = clock();
	file = readfile(filename);

	fill_data();
	fill_data2();

	clock_t begin1 = clock();
	printf("\nThings for 1 and 2: %lfms\n\n", (double)(begin1 - begin));
	#ifndef TEST2
		part1();
	#endif
	clock_t end1 = clock();
	printf("\n%lfms\n\n", (double)(end1 - begin1));
	part2();
	clock_t end2 = clock();
	printf("\n%lfs", (double)(end2 - end1)/CLOCKS_PER_SEC);
}

void part1()
{
	size_t answer;

	answer = check_steps();

	printf("Part 1: %llu", answer);
}

void part2()
{
	size_t answer;

	check_steps2();
	answer = amount_steps();

	printf("Part 2: %llu", answer);
}

void fill_data()
{
	size_t i;
	char game[stringlength];
	char tmp[stringlength];
	char **tokens;
	char **tokensg;

	strcpy(data.steps, file.file[0]);

	for(i = 2; i < file.lengthfile; i++)
	{
		strcpy(game, file.file[i]);
		strcpy(game, searchAndReplace(game, " ", ""));
		strcpy(game, searchAndReplace(game, "(", ""));
		strcpy(game, searchAndReplace(game, ")", ""));
		tokens = str_split(game, '=', false);
		data.start[i - 2] = string_size_t(*(tokens));
		tokensg = str_split(*(tokens + 1), ',', false);
		data.left[i - 2] = string_size_t(*(tokensg));
		data.right[i - 2] = string_size_t(*(tokensg + 1));
	}
}

size_t string_size_t(char *vstring)
{
	size_t i = 0, number  = 0;
	char game[4096];
	strcpy(game, vstring);
	while(true)
	{
		if(game[i] >= 'A' && game[i] <= 'Z')
			{
				number = 100 * number + (game[i] - 'A' + 1);
			}
			else
			{
				return number;
			}   
		i++;
	}
}

size_t check_steps()
{
	size_t amount = 0, i = 0, j = 0, new_start;
	size_t value_start = string_size_t("AAA");
	size_t value_end = string_size_t("ZZZ");
	while(value_start != data.start[i])
	{
		i++;
	}
	while(true)
	{
		amount++;
		switch(data.steps[j])
		{
			case 'R':
				new_start = data.right[i]; break;
			case 'L':
				new_start = data.left[i]; break;
			default:
				break;
		}
		i = 0;
		while(new_start != data.start[i])
		{
			i++;
		}
		if(data.start[i] == value_end)
		{
			return amount;
		}
		j++;
		if(j == strlen(data.steps))
		{
			j = 0;
		}
	}
}

void fill_data2()
{
	size_t i;
	char game[stringlength];
	char tmp[stringlength];
	char **tokens;
	char **tokensg;

	strcpy(data.steps, file.file[0]);

	for(i = 2; i < file.lengthfile; i++)
	{
		strcpy(game, file.file[i]);
		strcpy(game, searchAndReplace(game, " ", ""));
		strcpy(game, searchAndReplace(game, "(", ""));
		strcpy(game, searchAndReplace(game, ")", ""));
		tokens = str_split(game, '=', false);
		data2.start[i - 2] = string_size_t2(*(tokens));
		tokensg = str_split(*(tokens + 1), ',', false);
		data2.left[i - 2] = string_size_t(*(tokensg));
		data2.right[i - 2] = string_size_t(*(tokensg + 1));
	}
}

size_t string_size_t2(char *vstring)
{
	size_t i = 0, number  = 0;
	char game[4096];
	strcpy(game, vstring);
	number = game[2] - 'A' + 1;
	return number;  
}

void check_steps2()
{
	size_t amount = 0, i = 0, j = 0, k = 0, amount_start = 0, new_start;
	size_t value_start = string_size_t2("XXA");
	size_t value_end = string_size_t2("XXZ");
	bool all_on_end;
	for(k = 0; k < amount_starts; k++)
	{
		data2.already_end[k] = false;
	}
	while(i < file.lengthfile - 2)
	{
		if(value_start == data2.start[i])
		{
			data2.value[amount_start] = i;
			amount_start++;
		}
		i++;
	}
	assert(amount_start == amount_starts);
	while(all_on_end == false)
	{
		amount++;
		all_on_end = true;
		for(k = 0; k < amount_start; k++)
		{
			if(data2.amount_loops[k] == 0)
			{
				switch(data.steps[j])
				{
					case 'R':
						data2.new_start[k] = data.right[data2.value[k]]; break;
					case 'L':
						data2.new_start[k] = data.left[data2.value[k]]; break;
					default:
						break;
				}
				data2.value[k] = 0;
				while(data2.new_start[k] != data.start[data2.value[k]])
				{
					data2.value[k]++;
				}
				if(data2.start[data2.value[k]] != value_end)
				{
					all_on_end = false;
				}
				if(data2.start[data2.value[k]] == value_end)
				{
					if(data2.already_end[k] == true)
					{
						data2.amount_loops[k] = amount - data2.previous[k];
					}
					else
					{
						all_on_end = false;
						data2.previous[k] = amount;
					}
					data2.already_end[k] = true;
				}
			}
		}
		j++;
		if(j >= strlen(data.steps))
		{
			j = 0;
		}
	}
}

size_t amount_steps()
{
	size_t amountsteps;
	bool all_the_same = false;
	while(all_the_same == false)
	{
		all_the_same = true;
		amountsteps = 0;
		for(size_t k = 1; k < amount_starts; k++)
		{
			while(data2.previous[0] < data2.previous[k])
			{
				data2.previous[0] = data2.previous[0] + data2.amount_loops[0];
			}
			while(data2.previous[0] > data2.previous[k])
			{
				data2.previous[k] = data2.previous[k] + data2.amount_loops[k];
			}
		}


		for(size_t k = 0; k < amount_starts; k++)
		{
			amountsteps = data2.previous[0];
			if(data2.previous[0] != data2.previous[k])
			{
				all_the_same = false;
			}
		}
	}
	return amountsteps;
}
