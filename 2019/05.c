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

//to low 53523


struct IntCode
{
	long long *reg;
	size_t current;
	long long input;
};
struct IntCode IC;


void part1();
void part2();

void setup();
long long runIntCode();
void IC_1();
void IC_2();
void IC_3();
long long IC_4();
void IC_5();
void IC_6();
void IC_7();
void IC_8();

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
	IC.input = 1;
	answer = runIntCode();
	free(IC.reg);
	
	printf("Part 1: %lld", answer);
	
}

void part2()
{
	long long answer;
	
	setup();
	IC.input = 5;
	answer = runIntCode();
	
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

long long runIntCode()
{
	long long answer = 0, old = 0;
	long long tmp;
	int instruction;
	int m0, m1, m2;
	while(true)
	{
		if(old != 0)
			printf("Test Failed");
		instruction = *(IC.reg + IC.current) % 100;
		tmp = *(IC.reg + IC.current) / 100;
		m0 = tmp % 10;
		tmp /= 10;
		m1 = tmp % 10;
		tmp /= 10;
		m2 = tmp % 10;
		switch(instruction)
		{
			case 01:
				IC_1(m0, m1, m2);
				break;
			case 02: 
				IC_2(m0, m1, m2);
				break;
			case 03:
				IC_3(m0, m1, m2);
				break;
			case 04:
				old = answer;
				answer = IC_4(m0, m1, m2);
				break;
			case 99:
				return answer;
			default:
				IC.current++;
				break;
		}
	}
}

void IC_1(int m0, int m1, int m2)
{
	long long *a, *b, *c;
	if(m0 == 0)
		a = (IC.reg + *(IC.reg + IC.current + 1));
	else
		a = (IC.reg + IC.current + 1);

	if(m1 == 0)
		b = (IC.reg + *(IC.reg + IC.current + 2));
	else
		b = (IC.reg + IC.current + 2);

	if(m2 == 0)
		c = (IC.reg + *(IC.reg + IC.current + 3));
	else
		c = (IC.reg + IC.current + 3);
	
	*c = *a + *b;

	IC.current += 4;
	return;
}

void IC_2(int m0, int m1, int m2)
{
	long long *a, *b, *c;
	if(m0 == 0)
		a = (IC.reg + *(IC.reg + IC.current + 1));
	else
		a = (IC.reg + IC.current + 1);

	if(m1 == 0)
		b = (IC.reg + *(IC.reg + IC.current + 2));
	else
		b = (IC.reg + IC.current + 2);

	if(m2 == 0)
		c = (IC.reg + *(IC.reg + IC.current + 3));
	else
		c = (IC.reg + IC.current + 3);
	
	*c = *a * *b;
	IC.current += 4;
	return;
}

void IC_3(int m0, int m1, int m2)
{
	long long *a;
	if(m0 == 0)
		a = (IC.reg + *(IC.reg + IC.current + 1));
	else
		a = (IC.reg + IC.current + 1);
	*a = IC.input;

	IC.current += 2;
	return;
}

long long IC_4(int m0, int m1, int m2)
{
	long long *a;
	if(m0 == 0)
		a = (IC.reg + *(IC.reg + IC.current + 1));
	else
		a = (IC.reg + IC.current + 1);

	IC.current += 2;
	return *a;
}