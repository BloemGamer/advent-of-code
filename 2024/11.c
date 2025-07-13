#define HASH_SIZE 1000000

#include "../libraries/adventofcode.c" //V1.0

#define CACHE_SIZE (size_t)4096*32*32

struct Data
{
	long long *start_stones;
	size_t amount_stones;
};
struct Cache
{
	long long *stone;
	long long *result;
	size_t *amount_blinks;
	size_t place;
};

struct Data data;
struct Cache cache;

long long check_stones();
void fill_data();

int main(int argc, char *argv[])
{
	clock_t begin = clock();
	hash_fix_dummy_item();
	fix_file(argv, "M");
	fill_data();
	run_parts(begin);
}

void part1()
{
	long long answer = 0;
	for(size_t i = 0; i < data.amount_stones; i++)
		answer += check_stones(25, *(data.start_stones + i));
	printf("Part 1: %lld", answer);
}

void part2()
{
	long long answer = 0;
	for(size_t i = 0; i < data.amount_stones; i++)
		answer += check_stones(75, *(data.start_stones + i));
	printf("Part 2: %lld", answer);
}

long long check_stones(int amount_blinks, long long stone)
{
	long long stone2;
	long long result, result2, tmp;
	int fact = (int)log10((double)stone) + 1;

	if(amount_blinks == 0)
		return 1;
	if(100 * stone + amount_blinks < HASH_SIZE)
		if((item = hash_search((100 * stone) + amount_blinks)) != NULL)
			return item->data;
	
	if(stone == 0)
	{
		tmp = stone;
		stone = 1;
		result = (check_stones(amount_blinks - 1, stone));
		if(tmp < 100000)
			hash_insert((100 * tmp) + amount_blinks, result);
		return result;
	}
	else if(stone > 9 && fact % 2 == 0)
	{
		tmp = stone;

		stone2 = stone % (long long)pow(10.0, (double)(fact/2));
		stone = stone / (long long)pow(10.0, (double)(fact/2));
		result =  check_stones(amount_blinks - 1, stone);
		result2 = check_stones(amount_blinks - 1, stone2);

		if(tmp < 100000)
			hash_insert((100 * tmp) + amount_blinks, result + result2);

		cache.place++;
		return result + result2;
	}
	else
	{
		tmp = stone;
		stone *= 2024;
		result = (check_stones(amount_blinks - 1, stone));
		if(tmp < 100000)
			hash_insert((100 * tmp) + amount_blinks, result);
		return result;
	}
	
}

void fill_data()
{
	char *game;
	char **tokens;
	size_t i = 0;
	game = calloc(file.lengthlines[0], sizeof(char));
	strcpy(game, file.file[0]);

	tokens = str_split(game, ' ', false);
	while(*(tokens + i) != NULL)
		i++;
	data.start_stones = calloc(i, sizeof(long long));
	data.amount_stones = i;

	for(size_t j = 0; j < i; j++)
		*(data.start_stones + j) = str_ll(*(tokens + j));
	cache.result = calloc(CACHE_SIZE, sizeof(long long));
	cache.amount_blinks = calloc(CACHE_SIZE, sizeof(size_t));
	cache.stone = calloc(CACHE_SIZE, sizeof(long long));
	cache.place = 0;
}