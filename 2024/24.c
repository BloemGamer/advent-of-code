#include "../libraries/adventofcode.c" //V1.0

// part 2 doesn't work, but has some functions so you can change your input file, and see improvements

// 10 1000 1001 1111 0001 0111 1010 0111 1100 0000 1110 1000
// 10 1001 0001 1111 0000 1111 1010 0111 1011 1110 1110 1000

#define TEST

#ifdef TEST
	#define FILE_TYPE "T2"
	#define AMOUNT_Z 46
#else
	#define FILE_TYPE "M"
	#define AMOUNT_Z 46
#endif

struct Data
{
	unsigned long long z_array, z_array2, y_array, x_array;

	char **instructions;
	unsigned long long *answers;
	size_t end_start_data;
	size_t amount_instructions;
};
struct Data data;

void print_answer();
void fill_data();
void run();
int search();

void fill_arrays();

int main(int argc, char **argv)
{
	clock_t begin = clock();
	fix_file(argv, FILE_TYPE);

	run_parts(begin);
}

void part1(void)
{
	long long answer = 0;
	fill_data();
	run();
	printf("Part 1: ");
	print_answer();
}

void part2(void)
{
	long long answer = 0;

	fill_arrays();

	printf("Part 2: %lld", answer);
}

void print_answer(void)
{
	for(size_t i = 0; i < data.amount_instructions; i++)
	{
		// printf("%d", data.answers[i]);
		if(*(*(data.instructions + i) + 9) == 'z')
			data.z_array = (data.z_array | (data.answers[i] << str_ll((*(data.instructions + i) + 10))));
	}

	long long size = AMOUNT_Z - 1;
	while(size >= 0)
		printf("%u", (data.z_array & (long long unsigned)1 << size) >> size--);
	printf("\n%llu", data.z_array);

}

void fill_data(void)
{
	size_t i = 0;
	while(file.lengthlines[i] != 0)
	{
		i++;
	}
	data.end_start_data = i;
	i++;
	data.amount_instructions = file.amountlines - i;
	data.instructions = file.file + i;
	for(size_t j = 0; j < data.amount_instructions; j++)
	{
		*(data.instructions + j) = searchAndReplace(*(data.instructions + j), "AND", "&");
		*(data.instructions + j) = searchAndReplace(*(data.instructions + j), "XOR", "^");
		*(data.instructions + j) = searchAndReplace(*(data.instructions + j), "OR", "|");
		*(data.instructions + j) = searchAndReplace(*(data.instructions + j), " ", "");
	}
	data.answers = calloc(data.amount_instructions + 1, sizeof(unsigned long long));
}
// y06&x06->pqj
void run(void)
{
	for(size_t i = 0; i < data.amount_instructions; i++)
		for(size_t j = 0; j < data.amount_instructions; j++)
		{
			switch(*(*(data.instructions + j) + 3))
			{
				case '&':
					data.answers[j] = (search(*(data.instructions + j)) & search(*(data.instructions + j) + 4));
					break;
				case '^':
					data.answers[j] = (search(*(data.instructions + j)) ^ search(*(data.instructions + j) + 4));
					break;
				case '|':
					data.answers[j] = (search(*(data.instructions + j)) | search(*(data.instructions + j) + 4));
					break;
			}
		}
}

int search(char *input)
{
	if(*input == 'y' || *input == 'x')
		for(size_t i = 0; i < data.end_start_data; i++)
			if(!(strncmp(input, *(file.file + i), 3)))
				return (*(*(file.file + i) + 5) - '0');

	for(size_t i = 0; i < data.amount_instructions; i++)
		if(!(strncmp(input, (*(data.instructions + i) + 9), 3)))
			return data.answers[i];
}

void fill_arrays()
{
	for(size_t i = 0; i < data.end_start_data; i++)
	{
		if(*(*(file.file + i)) == 'y')
			data.y_array = (data.y_array | ((unsigned long long)(*(*(file.file + i) + 5) - '0') << str_ll((*(file.file + i) + 1))));
		if(*(*(file.file + i)) == 'x')
			data.x_array = (data.x_array | ((unsigned long long)(*(*(file.file + i) + 5) - '0') << str_ll((*(file.file + i) + 1))));
	}

	long long size = AMOUNT_Z - 2;
	// while(size >= 0)
	// 	printf("%u", (data.y_array & (long long unsigned)1 << size) >> size--);
	// printf("\n%llu\n", data.y_array);

	// size = AMOUNT_Z - 2;
	// while(size >= 0)
	// 	printf("%u", (data.x_array & (long long unsigned)1 << size) >> size--);
	// printf("\n%llu\n", data.x_array);

	data.z_array2 = data.y_array + data.x_array;
	size = AMOUNT_Z - 1;
	while(size >= 0)
	{	
		if(!((size + 1) % 4))
			printf(" ");
		printf("%u", (data.z_array2 & (long long unsigned)1 << size) >> size--);
	}

	printf("\n");
	size = AMOUNT_Z - 1;
	while(size >= 0)
	{	
		if(!((size + 1) % 4))
			printf(" ");
		printf("%u", (data.z_array & (long long unsigned)1 << size) >> size--);
	}

	printf("\n");
}