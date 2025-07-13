#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <inttypes.h>

#include "../libraries/adventofcode.c" //V5.1
#include "../libraries/queue.c" //V4.2
#define _DEBUG
#include "../libraries/debug.c" //V0.1


struct IntCode
{
	long long *reg;
	size_t current;
};
struct IntCode IC;


void part1();
void part2();

void setup();
void ICadd();
void ICmul();
long long runIntCode();

int main(int argc, char *argv[])
{
	clock_t begin = clock();
	
	fix_file(argv, "M");

	clock_t begin1 = clock();
	printf("\nThings for 1 and 2: %.0lf ms\n\n", (double)(begin1 - begin)/CLOCKS_PER_SEC*1000);
	part1();
	clock_t end1 = clock();
	printf("\n%.0lf ms\n\n", (double)(end1 - begin1)/CLOCKS_PER_SEC*1000);
	part2();
	clock_t end2 = clock();
	printf("\n%.0lf ms", (double)(end2 - end1)/CLOCKS_PER_SEC*1000);
	return 0;
}

void part1() 
{
	long long answer;
	
	setup();
	*(IC.reg + 1) = 12;
	*(IC.reg + 2) = 2;
	answer = runIntCode();
	free(IC.reg);
	
	printf("Part 1: %lld", answer);
	
}

void part2()
{
	long long answer, tmp;
	for(size_t noun = 0; noun < 100; noun++)
	{
		for(size_t verb = 0; verb < 100; verb++)
		{
			setup();
			*(IC.reg + 1) = noun;
			*(IC.reg + 2) = verb;
			tmp = runIntCode();
			if(tmp == 19690720)
			{
				answer = 100 * noun + verb;
				break;
			}
			free(IC.reg);
		}
		if(tmp == 19690720)
			break;
	}
	free(IC.reg);
	printf("Part 2: %lld", answer);
}

void setup()
{
	char *game;
	char **tokens;
	size_t i = 0;
	game = calloc(file.lengthlines[0], sizeof(char));
	strcpy(game, file.file[0]);
	IC.current = 0;

	tokens = str_split(game, ',', false);
	while(*(tokens + i) != NULL)
	{
		i++;
	}
	IC.reg = calloc(i, sizeof(long long));

	for(size_t j = 0; j < i; j++)
	{
		*(IC.reg + j) = str_ll(*(tokens + j));
	}
	return;
}

void ICadd()
{
	*(IC.reg + *(IC.reg + IC.current + 3)) = *(IC.reg + *(IC.reg + IC.current + 1)) + *(IC.reg + *(IC.reg + IC.current + 2));

	IC.current += 4;
	return;
}

void ICmul()
{
	*(IC.reg + *(IC.reg + IC.current + 3)) = *(IC.reg + *(IC.reg + IC.current + 1)) * *(IC.reg + *(IC.reg + IC.current + 2));

	IC.current += 4;
	return;
}

long long runIntCode()
{
	int tmp;
	while(true)
	{
		switch(*(IC.reg + IC.current))
		{
			case 1:
				ICadd();
				break;
			case 2:
				ICmul();
				break;
			case 99:
				tmp = *IC.reg;
				return tmp;;
			default:
				IC.current++;
				break;
		}
	}
}