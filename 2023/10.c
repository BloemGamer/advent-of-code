#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "..\libraries\adventofcode.c" //V4.3

#define QUEUELENGTH 4095

#include "..\libraries\queue.c" //V4.2

//#define TEST2

#if defined TEST1
    #define filename "txt/10.test1.txt"
	#define mapheight ((10 + 2) * 3)
	#define maplength ((20 + 2) * 3 + 1)
#elif defined TEST2
	#define filename "txt/10.test2.txt"
	#define mapheight ((10 + 2) * 3)
	#define maplength ((20 + 2) * 3 + 1)
#else
	#define filename "txt/10.txt"
	#define mapheight ((140 + 2) * 3)
	#define maplength ((140 + 2) * 3 + 2)
#endif

char map[mapheight][maplength];

typedef struct Position
{
	size_t x, y;
	char next[3];
}Position_t;

Position_t start;
Position_t current;

long_long_unsigned_Queue_t queue_x;
long_long_unsigned_Queue_t queue_y;



void part1();
void part2();

void search_start();
size_t follow_loop();
void get_new_current();
void fill();
size_t amount_in_loop();



int main(void)
{
	clock_t begin = clock();
	file = readfile(filename);

	for(size_t i = 0; i < mapheight; i++)
	{
		memset(map[i], '.', maplength - 1);
	}

	search_start();

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
	size_t answer;

	answer = follow_loop() / 2;


	printf("Part 1: %llu", answer);
}

void part2()
{
	size_t answer;

	fill();
	answer = amount_in_loop();

	printf("Part 2: %llu", answer);
}

void search_start()
{
	for(size_t i = 0; i < file.lengthfile; i++)
	{
		for(size_t j = 0; j < file.maxlengthfile; j++)
		{
			if(file.file[i][j] == 'S')
			{
				start.x = j;
				start.y = i;
				get_new_current(start);
				return;
			}
		}
	}
}

size_t follow_loop()
{
	size_t amount_steps = 1;

	while(file.file[current.y][current.x] != 'S' || amount_steps == 1)
	{
		switch(current.next[1])
		{
		case 'U':
			current.y--;
			switch(file.file[current.y][current.x])
			{
				case '|':
					strcpy(current.next, "|U");
					map[(current.y + 1) * 3][(current.x + 1) * 3 + 1] = '#';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = '|';
					map[(current.y + 1) * 3 + 2][(current.x + 1) * 3 + 1] = '#';
					break;
				case '7':
					strcpy(current.next, "7L");
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3] = '#';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = '7';
					map[(current.y + 1) * 3 + 2][(current.x + 1) * 3 + 1] = '#';
					break;
				case 'F':
					strcpy(current.next, "FR");
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 2] = '#';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'F';
					map[(current.y + 1) * 3 + 2][(current.x + 1) * 3 + 1] = '#';
					break;
				default:
					break;
			}
			break;
		case 'R':
			current.x++;
			switch(file.file[current.y][current.x])
			{
				case '-':
					strcpy(current.next, "-R");
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3] = '#';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = '-';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 2] = '#';
					break;
				case 'J':
					strcpy(current.next, "JU");
					map[(current.y + 1) * 3][(current.x + 1) * 3 + 1] = '#';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'J';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3] = '#';
					break;
				case '7':;
					strcpy(current.next, "7D");
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3] = '#';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = '7';
					map[(current.y + 1) * 3 + 2][(current.x + 1) * 3 + 1] = '#';
					break;
				default:
					break;
			}
			break;
		case 'D':
			current.y++;
			switch(file.file[current.y][current.x])
			{
				case '|':
					strcpy(current.next, "|D");
					map[(current.y + 1) * 3][(current.x + 1) * 3 + 1] = '#';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = '|';
					map[(current.y + 1) * 3 + 2][(current.x + 1) * 3 + 1] = '#';
					break;
				case 'L':
					strcpy(current.next, "LR");
					map[(current.y + 1) * 3][(current.x + 1) * 3 + 1] = '#';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'L';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 2] = '#';
					break;
				case 'J':;
					strcpy(current.next, "JL");
					map[(current.y + 1) * 3][(current.x + 1) * 3 + 1] = '#';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'J';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3] = '#';
					break;
				default:
					break;
			}

			break;
		case 'L':
			current.x--;
			switch(file.file[current.y][current.x])
			{
				case '-':
					strcpy(current.next, "-L");
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3] = '#';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = '-';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 2] = '#';
					break;
				case 'L':
					strcpy(current.next, "LU");
					map[(current.y + 1) * 3][(current.x + 1) * 3 + 1] = '#';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'L';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 2] = '#';
					break;
				case 'F':
					strcpy(current.next, "FD");
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 2] = '#';
					map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'F';
					map[(current.y + 1) * 3 + 2][(current.x + 1) * 3 + 1] = '#';
					break;
				default:
					break;
			}
			break;
		default:
			break;
		}
		amount_steps++;
	}
	switch(current.next[1])
	{
		case 'U':
			map[(current.y + 1) * 3 + 2][(current.x + 1) * 3 + 1] = '#';
			break;
		case 'R':
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3] = '#';
			break;
		case 'D':
			map[(current.y + 1) * 3][(current.x + 1) * 3 + 1] = '#';
			break;
		case 'L':
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 2] = '#';
			break;
		default:
			break;
	}

	return amount_steps;
}

void get_new_current(Position_t old)
{
	current = old;
	switch(file.file[__max(1, old.y) - 1][old.x])
	{
		case '|':
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'S';
			map[(current.y + 1) * 3][(current.x + 1) * 3 + 1] = '#';
			current.y--;
			strcpy(current.next, "|U");
			map[(current.y + 1) * 3][(current.x + 1) * 3 + 1] = '#';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = '|';
			map[(current.y + 1) * 3 + 2][(current.x + 1) * 3 + 1] = '#';
			return;
		case '7':
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'S';
			map[(current.y + 1) * 3][(current.x + 1) * 3 + 1] = '#';
			current.y--;
			strcpy(current.next, "7L");
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3] = '#';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = '7';
			map[(current.y + 1) * 3 + 2][(current.x + 1) * 3 + 1] = '#';
			return;
		case 'F':
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'S';
			map[(current.y + 1) * 3][(current.x + 1) * 3 + 1] = '#';
			current.y--;
			strcpy(current.next, "FR");
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 2] = '#';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'F';
			map[(current.y + 1) * 3 + 2][(current.x + 1) * 3 + 1] = '#';
			return;
		default:
			break;
	}
	switch(file.file[old.y][__min(file.maxlengthfile, old.x + 1)])
	{
		case '-':
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'S';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 2] = '#';
			current.x++;
			strcpy(current.next, "-R");
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3] = '#';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = '-';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 2] = '#';
			return;
		case 'J':
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'S';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 2] = '#';
			current.x++;
			strcpy(current.next, "JU");
			map[(current.y + 1) * 3][(current.x + 1) * 3 + 1] = '#';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'J';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3] = '#';
			return;
		case '7':
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'S';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 2] = '#';
			current.x++;
			strcpy(current.next, "7D");
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3] = '#';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = '7';
			map[(current.y + 1) * 3 + 2][(current.x + 1) * 3 + 1] = '#';
			return;
		default:
			break;
	}
	switch(file.file[__min(file.lengthfile, old.y + 1)][old.x])
	{
		case '|':
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'S';
			map[(current.y + 1) * 3 + 2][(current.x + 1) * 3 + 1] = '#';
			current.y++;
			strcpy(current.next, "|D");
			map[(current.y + 1) * 3][(current.x + 1) * 3 + 1] = '#';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = '|';
			map[(current.y + 1) * 3 + 2][(current.x + 1) * 3 + 1] = '#';
			return;
		case 'L':
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'S';
			map[(current.y + 1) * 3 + 2][(current.x + 1) * 3 + 1] = '#';
			current.y++;
			strcpy(current.next, "LR");
			map[(current.y + 1) * 3][(current.x + 1) * 3 + 1] = '#';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'L';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 2] = '#';
			return;
		case 'J':
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'S';
			map[(current.y + 1) * 3 + 2][(current.x + 1) * 3 + 1] = '#';
			current.y++;
			strcpy(current.next, "JL");
			map[(current.y + 1) * 3][(current.x + 1) * 3 + 1] = '#';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'J';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3] = '#';
			return;
		default:
			break;
	}
	switch(file.file[old.y][__max(1, old.x) - 1])
	{
		case '-':
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'S';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3] = '#';
			current.x--;
			strcpy(current.next, "-L");
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3] = '#';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = '-';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 2] = '#';
			return;
		case 'L':
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'S';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3] = '#';
			current.x--;
			strcpy(current.next, "LU");
			map[(current.y + 1) * 3][(current.x + 1) * 3 + 1] = '#';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'L';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 2] = '#';
			return;
		case 'F':
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'S';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3] = '#';
			current.x--;
			strcpy(current.next, "FD");
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 2] = '#';
			map[(current.y + 1) * 3 + 1][(current.x + 1) * 3 + 1] = 'F';
			map[(current.y + 1) * 3 + 2][(current.x + 1) * 3 + 1] = '#';
			return;
		default:
			break;
	}
}

void fill()
{
	size_t x, y;
	init_llu(&queue_x);
	init_llu(&queue_y);
	push_back_llu(0, &queue_x);
	push_back_llu(0, &queue_y);
	do
	{
		x = pop_front_llu(&queue_x);
		y = pop_front_llu(&queue_y);
		map[y][x] = '"';
		if(map[__max(1, y) - 1][x] == '.')
		{
			push_back_llu(x, &queue_x);
			push_back_llu(__max(1, y) - 1, &queue_y);
			map[__max(1, y) - 1][x] = ' ';
		}
		if(map[__min(mapheight - 1, y) + 1][x] == '.')
		{
			push_back_llu(x, &queue_x);
			push_back_llu(__min(mapheight - 1, y) + 1, &queue_y);
			map[__min(mapheight - 1, y) + 1][x] = ' ';
		}
		if(map[y][__max(1, x) - 1] == '.')
		{
			push_back_llu(__max(1, x) - 1, &queue_x);
			push_back_llu(y, &queue_y);
			map[y][__max(1, x) - 1] = ' ';
		}
		if(map[y][__min(maplength - 1, x) + 1] == '.')
		{
			push_back_llu(__min(maplength - 1, x) + 1, &queue_x);
			push_back_llu(y, &queue_y);
			map[y][__min(maplength - 1, x) + 1] = ' ';
		}
	}while(queue_x.front != queue_x.back);

	x = pop_front_llu(&queue_x);
	y = pop_front_llu(&queue_y);
	map[y][x] = '"';

	return;
}

size_t amount_in_loop()
{
	size_t count = 0, i = 1, j = 1;
	while(i < mapheight)
	{
		while(j < maplength)
		{
			if(map[i][j] == '.')
			{
				count++;
			}
			j += 3;
		}
		j = 1;
		i += 3;
	}
	return count;
}