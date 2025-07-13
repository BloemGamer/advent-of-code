#include "../libraries/adventofcode.c" //V1.0

// #define TEST

#ifdef TEST
	#define FILE_TYPE "T1"
	#define MAP_SIZE (6 + 1)
    #define AMOUNT_BYTES 12
#else
	#define FILE_TYPE "M"
	#define MAP_SIZE (70 + 1)
    #define AMOUNT_BYTES 1024
#endif

struct Data
{
	char **map;
	char **map2;
	long long **crossway_map;
	long long amount_crossways;
	int Sy, Sx, Ey, Ex;
	long long **crossways;
};
struct Data data;

struct Flood
{
	long long **flood;
	size_t length;
	long long shortest_path;
};
struct Flood flood;

void fill_data();
long long flood_water();

int main(int argc, char **argv)
{
	clock_t begin = clock();
	fix_file(argv, FILE_TYPE);

	run_parts(begin);
}

void part1()
{
	long long answer = 0;
    long long amount_bytes = AMOUNT_BYTES;
	fill_data(amount_bytes);
	answer = flood_water(1);

	free(data.map);
	printf("Part 1: %lld", answer);
}

void part2()
{
	long long answer = 1;
	long long amount_bytes = 1024;
    while(answer != 0)
    {
        fill_data(amount_bytes);
        answer = flood_water(2);
        amount_bytes++;
        free(data.map);
    }

	printf("Part 2: %s", file.file[amount_bytes - 2]);
}

void fill_data(long long amount_bytes)
{
	data.map = calloc(MAP_SIZE + 1, sizeof(char*));
    char game[4096];
    char **tokens;
    int y;
    int x;
    for(size_t i = 0; i < MAP_SIZE; i++)
    {
        *(data.map + i) = calloc(MAP_SIZE + 1, sizeof(char));
        memset(*(data.map + i), '.', MAP_SIZE * sizeof(char));
    }
    for(size_t i = 0; i < amount_bytes; i++)
    {
        strcpy(game, file.file[i]);
        tokens = str_split(game, ',', false);
        y = str_ll(*(tokens + 1));
        x = str_ll(*tokens);
        data.map[y][x] = '#';
    }    
}

long long flood_water(int part)
{
	size_t i, k, l, m;
	long long count = 0;
	flood.length = 1024;
	flood.flood = calloc(flood.length, sizeof(long long*));
	for(i = 0; i < flood.length; i++)
	{
		flood.flood[i] = calloc(3, sizeof(long long));
		flood.flood[i][2] = -1;
	}
	if(data.map[0][0] == '.')
	{
		flood.flood[0][0] = 0;
		flood.flood[0][1] = 0;
		flood.flood[0][2] = 0;
	}
    data.map[0][0] = 'X';
	while(data.map[MAP_SIZE - 1][MAP_SIZE - 1] == '.')
	{
		count++;
		for(i = 0; i < flood.length; i++)
		{
			if(flood.flood[i][2] == 0)
			{
				data.map[flood.flood[i][0]][flood.flood[i][1]] = 'W';
				for(k = 0; k < 4; k++)
                if(flood.flood[i][1] + directions[1][k] >= 0 && flood.flood[i][1] + directions[1][k] < MAP_SIZE && flood.flood[i][0] + directions[0][k] >= 0 && flood.flood[i][0] + directions[0][k] < MAP_SIZE)
					switch(data.map[flood.flood[i][0] + directions[0][k]][flood.flood[i][1] + directions[1][k]])
					{
						case '.':
							for(l = 0; l < flood.length; l++)
								if(flood.flood[l][2] < 0)
								{
									flood.flood[l][0] = flood.flood[i][0] + directions[0][k];
									flood.flood[l][1] = flood.flood[i][1] + directions[1][k];
									flood.flood[l][2] = 1;
									data.map[flood.flood[l][0]][flood.flood[l][1]] = 'W';
									break;
								}
								else if(l == flood.length - 2)
									printf("run out of queue");
							break;
					}
			}
			
		}
		for(i = 0; i < flood.length; i++)
			if(flood.flood[i][2] > -1)
					flood.flood[i][2]--;
        if(part == 2)
		    if(count > MAP_SIZE * MAP_SIZE)
			    return 0;
	}
    if(part == 1)
	    flood.shortest_path = count;
	    return flood.shortest_path;
}
