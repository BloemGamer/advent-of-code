//V3.1
//part 2 took me about 5 minutes to calculate

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include "..\libraries\adventofcode.c"

#define arraysize 128

struct mapdata
{
	size_t seeds[arraysize];
	size_t seed_soil[arraysize];
	size_t soil_fertilizer[arraysize];
	size_t fertilizer_water[arraysize];
	size_t water_light[arraysize];
	size_t light_temperature[arraysize];
	size_t temperature_humidity[arraysize];
	size_t humidity_location[arraysize];
};
struct number
{
	size_t number, size, place;
	char *str;
	size_t *numberstring;
};

struct filecontent file;
struct mapdata data = { 0 };

void part1();
void part2();

void make_file_to_string();
size_t find_location();
size_t change_destination();


int main(void)
{
	clock_t begin = clock();
	const char *filename = "txt/5.txt";
	file = readfile(filename);
	size_t amountnumbers = 1;
	size_t i = 0, j = 0, k = 0;
	char game[4096];
	char tmp[4096];
	char **tokens;
	char **tokensg;

	make_file_to_string();
	
	clock_t begin1 = clock();
	printf("\nThings for 1 and 2: %lfms\n\n", (double)(begin1 - begin));
	part1();
	clock_t end1 = clock();
	printf("\n%lfms\n\n", (double)(end1 - begin1));
	part2();
	clock_t end2 = clock();
	printf("\n%lfs", (double)(end2 - end1)/CLOCKS_PER_SEC);
}

void part1()
{
	size_t answer = ULONG_LONG_MAX, i = 0;

	while(data.seeds[i] != 0)
	{
		answer = __min(answer, find_location(data.seeds[i]));
		i++;
	}
	printf("Part 1: %llu", answer);
}

void part2()
{
size_t answer = ULONG_LONG_MAX, i = 0, k, seed;

	while(data.seeds[i] != 0)
	{
		for(k = 0; k < data.seeds[i + 1]; k++)
		{
			seed = data.seeds[i] + k;
			answer = __min(answer, find_location(seed));
		}
		i = i + 2;
	}
	printf("Part 2: %llu", answer);
}


void make_file_to_string()
{
	//can be made efficiënter
	size_t k, j = 0;
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
		data.seeds[j] = strtoint(tmp);
		j++;
	}

	for(size_t i = 2; i < file.lengthfile; i++)
	{
		strcpy(game, file.file[i]);
		if(strcmp(game, "seed-to-soil map:") == 0)
		{
			//printf("seed-to-soil map:\n");
			i++;
			k = 0;
			while(strcmp(file.file[i], "") != 0)
			{
				strcpy(game, file.file[i]);
				tokens = str_split(game, ' ', false);
				j = 0;
				while(NULL != *(tokens + j))
				{
					strcpy(tmp, *(tokens + j));
					data.seed_soil[3 * k + j] = strtoint(tmp);
					j++;
				}
				i++;
				k++;
			}
		}
		if(strcmp(game, "soil-to-fertilizer map:") == 0)
		{
			//printf("soil-to-fertilizer map:\n");
			i++;
			k = 0;
			while(strcmp(file.file[i], "") != 0)
			{
				strcpy(game, file.file[i]);
				tokens = str_split(game, ' ', false);
				j = 0;
				while(NULL != *(tokens + j))
				{
					strcpy(tmp, *(tokens + j));
					data.soil_fertilizer[3 * k + j] = strtoint(tmp);
					j++;
				}
				i++;
				k++;
			}
		}
		if(strcmp(game, "fertilizer-to-water map:") == 0)
		{
			//printf("fertilizer-to-water map\n");
			i++;
			k = 0;
			while(strcmp(file.file[i], "") != 0)
			{
				strcpy(game, file.file[i]);
				tokens = str_split(game, ' ', false);
				j = 0;
				while(NULL != *(tokens + j))
				{
					strcpy(tmp, *(tokens + j));
					data.fertilizer_water[3 * k + j] = strtoint(tmp);
					j++;
				}
				i++;
				k++;
			}
		}
		if(strcmp(game, "water-to-light map:") == 0)
		{
			//printf("water-to-light map:\n");
			i++;
			k = 0;
			while(strcmp(file.file[i], "") != 0)
			{
				strcpy(game, file.file[i]);
				tokens = str_split(game, ' ', false);
				j = 0;
				while(NULL != *(tokens + j))
				{
					strcpy(tmp, *(tokens + j));
					data.water_light[3 * k + j] = strtoint(tmp);
					j++;
				}
				i++;
				k++;
			}
		}
		if(strcmp(game, "light-to-temperature map:") == 0)
		{
			//printf("light-to-temperature map:\n");
			i++;
			k = 0;
			while(strcmp(file.file[i], "") != 0)
			{
				strcpy(game, file.file[i]);
				tokens = str_split(game, ' ', false);
				j = 0;
				while(NULL != *(tokens + j))
				{
					strcpy(tmp, *(tokens + j));
					data.light_temperature[3 * k + j] = strtoint(tmp);
					j++;
				}
				i++;
				k++;
			}
		}
		if(strcmp(game, "temperature-to-humidity map:") == 0)
		{
			//printf("temperature-to-humidity map:\n");
			i++;
			k = 0;
			while(strcmp(file.file[i], "") != 0)
			{
				strcpy(game, file.file[i]);
				tokens = str_split(game, ' ', false);
				j = 0;
				while(NULL != *(tokens + j))
				{
					strcpy(tmp, *(tokens + j));
					data.temperature_humidity[3 * k + j] = strtoint(tmp);
					j++;
				}
				i++;
				k++;
			}
		}
		if(strcmp(game, "humidity-to-location map:") == 0)
		{
			//printf("humidity-to-location map:\n");
			i++;
			k = 0;
			while(i < file.lengthfile && strcmp(file.file[i], ""))
			{
				strcpy(game, file.file[i]);
				tokens = str_split(game, ' ', false);
				j = 0;
				while(NULL != *(tokens + j))
				{
					strcpy(tmp, *(tokens + j));
					data.humidity_location[3 * k + j] = strtoint(tmp);
					j++;
				}
				i++;
				k++;
			}
		}
	}
}

size_t find_location(size_t seed)
{
	size_t i = 0, tmp = seed;
	/*while(data.seed_soil[i] != 0)
	{
		if(tmp >= data.seed_soil[i + 1] && tmp < data.seed_soil[i + 1] + data.seed_soil[i + 2])
		{
			tmp = data.seed_soil[i] + tmp - data.seed_soil[i + 1];
			break;
		}
		i = i + 3;
	}*/
	tmp = change_destination(data.seed_soil, tmp);
	tmp = change_destination(data.soil_fertilizer, tmp);
	tmp = change_destination(data.fertilizer_water, tmp);
	tmp = change_destination(data.water_light, tmp);
	tmp = change_destination(data.light_temperature, tmp);
	tmp = change_destination(data.temperature_humidity, tmp);
	tmp = change_destination(data.humidity_location, tmp);

	return tmp;
}

size_t change_destination(size_t *vstring, size_t tmp)
{
	size_t i = 0;
	while(i < arraysize)
	{
		if(tmp >= vstring[i + 1] && tmp < vstring[i + 1] + vstring[i + 2])
		{
			tmp = vstring[i] + tmp - vstring[i + 1];
			break;
		}
		i = i + 3;
	}

	return tmp;
}
