#include "../libraries/adventofcode.c" //V1.0

// #define TEST

#ifdef TEST
	#define FILE_TYPE "T1"
	#define MAP_SIZE_X 11
	#define MAP_SIZE_Y 7
#else
	#define FILE_TYPE "M"
	#define MAP_SIZE_X 101
	#define MAP_SIZE_Y 103
#endif

struct Data
{
	long long **position;
	long long **velocity;
	long long RU, RD, LU, LD;
	char **map;
	bool possible;
	bool more_than_1_part_2;
};
struct Data data;

void part();
void fill_data();
long long run();

int main(int argc, char **argv)
{
	clock_t begin = clock();
	fix_file(argv, FILE_TYPE);
	data.map = calloc(MAP_SIZE_Y + 1, sizeof(char*));
	for(size_t i = 0; i < MAP_SIZE_Y; i++)
	{
		*(data.map + i) = calloc(MAP_SIZE_X + 1, sizeof(char));
		memset(*(data.map + i), '.', MAP_SIZE_X);
	}
	fill_data();
	part(argv);
	clock_t end = clock();
	printf("\n%lfms", (double)end-begin/CLOCKS_PER_SEC*1000);
}

void part(char **argv)
{
	long long answer1, answer2;

	char filename[99];
	data.possible = false;
	data.more_than_1_part_2 = false;
	
	for(size_t i = 0; i < MAP_SIZE_X * MAP_SIZE_Y; i++)
	{
		answer1 = run(i);
		if(i == 100)
			printf("Part 1: %lld", answer1);
		if(data.possible)
		{
			answer2 = i;
			if(data.more_than_1_part_2)
				printf("\nCheck the debug files for the right answer\n");
			data.more_than_1_part_2 = true;
			sprintf(filename, "14_%llu", i);
			make_debug_file(argv, data.map, filename);
		}
		for(size_t j = 0; j < MAP_SIZE_Y; j++)
			memset(*(data.map + j), '.', MAP_SIZE_X);
		data.possible = false;
	}

	printf("Part 2: %lld", answer2);
}

void fill_data()
{
	char **tokens;
	char **tokensg;
	data.position = calloc(file.amountlines, sizeof(long long*));
	data.velocity = calloc(file.amountlines, sizeof(long long*));
	for(size_t i = 0; i < file.amountlines; i++)
	{
		*(data.position + i) = calloc(2, sizeof(long long));
		*(data.velocity + i) = calloc(2, sizeof(long long));
		tokens = str_split(file.file[i], '=', false);
		tokensg = str_split(*(tokens + 1), ',', false);
		*(*(data.position + i) + 1) = str_ll(*(tokensg));
		*(*(data.position + i)) = str_ll(*(tokensg + 1));
		tokensg = str_split(*(tokens + 2), ',', false);
		*(*(data.velocity + i) + 1) = str_ll(*(tokensg));
		*(*(data.velocity + i)) = str_ll(*(tokensg + 1));
	}
}

long long run(long long seconds)
{
	data.LD = 0;
	data.LU = 0;
	data.RD = 0;
	data.RU = 0;
	long long x, y;
	long long count = 0;
	for(size_t i = 0; i < file.amountlines; i++)
	{
		y = data.position[i][0] + seconds * data.velocity[i][0];
		x = data.position[i][1] + seconds * data.velocity[i][1];

		y = y % MAP_SIZE_Y;
		x = x % MAP_SIZE_X;

		while(y < 0)
			y += MAP_SIZE_Y;
		while(x < 0)
			x += MAP_SIZE_X;

		if(seconds == 100)
			if(y > MAP_SIZE_Y / 2 && x > MAP_SIZE_X / 2)
				data.RD++;
			if(y < MAP_SIZE_Y / 2 && x > MAP_SIZE_X / 2)
				data.RU++;
			if(y > MAP_SIZE_Y / 2 && x < MAP_SIZE_X / 2)
				data.LD++;
			if(y < MAP_SIZE_Y / 2 && x < MAP_SIZE_X / 2)
				data.LU++;

		data.map[y][x] = '#';
	}
	for(size_t y1 = 1; y1 < MAP_SIZE_Y - 1; y1++)
		for(size_t x1 = 1; x1 < MAP_SIZE_X - 1; x1++)
		{
			if(data.map[y1][x1] == '#')
			{
				for(size_t j = 0; j < 8; j++)
					if(data.map[y1 + directions[0][j]][x1 + directions[1][j]] == '#')
					{
						count++;
					}
			}
			if(count > 500)
				data.possible = true;
		}
	return data.LD * data.LU * data.RD * data.RU;;
}

void part1(){}
void part2(){}