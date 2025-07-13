#include "../libraries/adventofcode.c" //V5.1

struct Data
{
	long long *answers;
	long long **input;
	size_t *amount_input;
};
struct Data data;

void fill_data();
long long possible();

int main(int argc, char *argv[])
{
	clock_t begin = clock();
	fix_file(argv, "M");
	fill_data();
	run_parts(begin);
}

void part1() 
{
	long long answer = 0;
	for(size_t i = 0; i < file.amountlines; i++)
		answer += possible(i, 0, 0, 1);
	printf("Part 1: %lld", answer);
}

void part2()
{
	long long answer = 0;
	for(size_t i = 0; i < file.amountlines; i++)
		answer += possible(i, 0, 0, 2);
	printf("Part 2: %lld", answer);
}

void fill_data()
{
	char **tokens;
	char game[4096];
	size_t j, k;
	
	data.answers = calloc(file.amountlines, sizeof(long long));
	data.input = calloc(file.amountlines, sizeof(long long*));
	data.amount_input = calloc(file.amountlines, sizeof(size_t));

	for(size_t i = 0; i < file.amountlines; i++)
	{
		strcpy(game, file.file[i]);
		tokens = str_split(game, ':', false);
		*(data.answers + i) = str_ll(*tokens);

		tokens = str_split(*(tokens + 1), ' ', false);
		j = 0;
		k = 0;
		while(*(tokens + j) != NULL)
			j++;
		*(data.amount_input + i) = j;
		*(data.input + i) = calloc(j, sizeof(long long));
		while(*(tokens + k) != NULL)
		{
			*(*(data.input + i) + k) = str_ll(*(tokens + k));
			k++;
		}
	}
}

long long possible(size_t place, size_t where, size_t current, int part)
{
	long long mul, add, cons, fact;
	if(where >= *(data.amount_input + place))
	{
		if(current == *(data.answers + place))
			return *(data.answers + place);
		else
			return 0;
	}
	fact = (long long)pow(10, (long long)(log10(*(*(data.input + place) + where)) + 1));
	add = current + *(*(data.input + place) + where);
	cons = current * fact + *(*(data.input + place) + where);
	if(current == 0 && where == 0)
		current = 1;
	mul = current * *(*(data.input + place) + where);
	
		
	if(possible(place, where + 1, mul, part) == *(data.answers + place) || possible(place, where + 1, add, part) == *(data.answers + place))
		return *(data.answers + place);
	if(part == 2)
		if(possible(place, where + 1, cons, part) == *(data.answers + place))
			return *(data.answers + place);
	
	return 0;
}