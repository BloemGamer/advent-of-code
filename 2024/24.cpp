#include "../libs/old/bloem.h"
#include <cstring>
#include <bitset>
// too high 0000000000000000001111101001111011111111111111
// too low 262651903

#define TEST2

#ifdef TEST
	#define FILE_TYPE "T1"
	#define AMOUNT_Z 13
#elif defined TEST2
	#define FILE_TYPE "T2"
	#define AMOUNT_Z 46
#else
	#define FILE_TYPE "M"
	#define AMOUNT_Z 46
#endif

struct Data
{
	unsigned long long z_array = 0, y_array = 0, x_array = 0;
	char **instructions;
	unsigned long long *answers;
    size_t end_start_data;
	size_t amount_instructions;
};
struct Data data;

void part1(void);
void part2(void);

void print_answer();
void fill_data();
void run();
int search(char *input);
char *search_and_replace(char *text, char *search, char *replace);
void fix_xy(void);

filecontent file;

int main(int argc, char **argv)
{
	// clock_t begin = clock();
    file.give_argv(argv);
	file.fix_file(FILE_TYPE);

    part1();
    part2();
}

void part1(void)
{
	// long long answer = 0;
	fill_data();
	run();
	printf("Part 1: ");
	print_answer();
}

void part2(void)
{
	// long long answer = 0;
	std::cout << "\nPart 2: " << std::bitset<AMOUNT_Z>(data.x_array + data.y_array) << "\n";
	std::cout << "\nPart 2: " << std::bitset<AMOUNT_Z>(data.y_array) << "\n";
	std::cout << "\nPart 2: " << std::bitset<AMOUNT_Z>(data.x_array) << "\n";
	// printf("Part 2: %lld", answer);

	for(size_t i = 0; i < data.amount_instructions; i++)
		if(!(strncmp("gnt", (*(data.instructions + i) + 9), 3)))
			std::cout << data.answers[i];
}

void print_answer(void)
{
	for(size_t i = 0; i < data.amount_instructions; i++)
	{
		// printf("%d", data.answers[i]);
		if(*(*(data.instructions + i) + 9) == 'z')
			data.z_array = (data.z_array | (data.answers[i] << atoll((*(data.instructions + i) + 10))));
	}

	// long long size = AMOUNT_Z - 1;
	// while(size >= 0)
	// 	printf("%llu", (data.z_array & (long long unsigned)1 << size) >> size--);
	// printf("\n%llu", data.z_array);
	std::cout << std::bitset<AMOUNT_Z>(data.z_array);

}

void fill_data(void)
{
    
	size_t i = 0;
	while(file.length_lines[i] != 0)
	{
		i++;
	}
	data.end_start_data = i;
	i++;
	data.amount_instructions = file.amount_lines - i;
	data.instructions = file.file + i;
	for(size_t j = 0; j < data.amount_instructions; j++)
	{
		*(data.instructions + j) = search_and_replace(*(data.instructions + j), (char*)"AND", (char*)"&");
		*(data.instructions + j) = search_and_replace(*(data.instructions + j), (char*)"XOR", (char*)"^");
		*(data.instructions + j) = search_and_replace(*(data.instructions + j), (char*)"OR", (char*)"|");
		*(data.instructions + j) = search_and_replace(*(data.instructions + j), (char*)" ", (char*)"");
	}
	data.answers = (unsigned long long*)calloc(data.amount_instructions + 1, sizeof(unsigned long long));
    fix_xy();
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
    unsigned long long *tmp;
    if(*input == 'x')
        tmp = &data.x_array;
    else if(*input == 'y')
        tmp = &data.y_array;

    else for(size_t i = 0; i < data.amount_instructions; i++)
		if(!(strncmp(input, (*(data.instructions + i) + 9), 3)))
			return data.answers[i];
    
    return ((*tmp >> atoll(input + 1)) & 0b01);
}


char *search_and_replace(char *text, char *search, char *replace)
{
	char buffer[4096];
	char *ptr;
	char *modText = NULL;

	buffer[0] ='\0';
	while((ptr = strstr(text, search)))
	{
		strncat(buffer, text, ptr - text);
		strcat(buffer, replace);

		text = ptr + strlen(search);
	}
	strcat(buffer, text);

	modText = (char*)malloc(strlen(buffer) + 1);
	strcpy(modText, buffer);
	return modText;
}

void fix_xy(void)
{
    unsigned long long *tmp;
    data.y_array = 0;
    data.z_array = 0;
    for(size_t i = 0; i < data.end_start_data; i++)
    {
        if(**(file.file + i) == 'x')
            tmp = &data.x_array;
        if(**(file.file + i) == 'y')
            tmp = &data.y_array;

        *tmp |= atoll((*(file.file + i) + 5)) << atoll((*(file.file + i) + 1));
    }
    //std::cout << "x_arr\n" << std::bitset<64>(data.x_array);
    //std::cout << "\n";

}
