//V3.1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include "..\libraries\adventofcode.c"

#define arraysize 4

struct Data
{
    size_t time[arraysize];
    size_t distance[arraysize];
};

struct Data data;

void part1();
void part2();

void string_to_array();
size_t strtoint_custom();



int main(void)
{
	clock_t begin = clock();
	const char *filename = "txt/6.txt";
	file = readfile(filename);

        

    clock_t begin1 = clock();
	printf("\nThings for 1 and 2: %lfms\n\n", (double)(begin1 - begin));
	part1();
	clock_t end1 = clock();
	printf("\n%lfms\n\n", (double)(end1 - begin1));
	part2();
	clock_t end2 = clock();
	printf("\n%lfms", (double)(end2 - end1));

}

void part1()
{
    size_t j = 0, time, speed, count,  answer = 1;
    string_to_array();

    while(j < arraysize)
    {
        count = 0;
        for(size_t i = 0; i < data.time[j]; i++)
        {   
            speed = i;
            time = data.time[j] - i;
            if(time*speed > data.distance[j])
            {
                count++;
            }
        }
        if(count != 0)
        {
            answer = answer * count;
        }
        j++;
    }
    printf("Part 1: %llu", answer);
}

void part2()
{
    size_t timedata, time, distancedata, speed, answer = 0;
    timedata = strtoint_custom(file.file[0]);
    distancedata = strtoint_custom(file.file[1]);

    for(size_t i = 0; i < timedata; i++)
    {   
        speed = i;
        time = timedata - i;
        if(time*speed > distancedata)
        {
            answer++;
        }
    }
    printf("Part 2: %llu", answer);
}

void string_to_array()
{
	size_t j = 0;
	char game[4096];
	char tmp[4096];
	char **tokens;
	char **tokensg;

	strcpy(game, file.file[0]);
	tokens = str_split(game, ':', false);
	strcpy(tmp, *(tokens + 1));
	tokensg = str_split(tmp, ' ', false);
	while(NULL != *(tokensg + j))
	{
		strcpy(tmp, *(tokensg + j));
		data.time[j] = strtoint(tmp);
		j++;
	}

    j = 0;
	strcpy(game, file.file[1]);
	tokens = str_split(game, ':', false);
	strcpy(tmp, *(tokens + 1));
	tokensg = str_split(tmp, ' ', false);
	while(NULL != *(tokensg + j))
	{
		strcpy(tmp, *(tokensg + j));
		data.distance[j] = strtoint(tmp);
		j++;
	}
}

size_t strtoint_custom(char *vstring)
{
	size_t i = 0, number  = 0;
	char game[4096];
	strcpy(game, vstring);
	while(i < file.maxlengthfile)
	{
		if(game[i] >= '0' && game[i] <= '9')
			{
				number = 10 * number + (game[i] - '0');
			}  
		i++;
	}
    return number;
}