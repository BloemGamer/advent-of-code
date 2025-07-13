#include "../libraries/adventofcode.c" //V1.0

#define OUTPUT_LENGTH 10

struct IntCode
{
	long long *reg;
	size_t current;
	size_t length;
	long long A, A2, B, C;
	long long *output;
	size_t place_output;
};
struct IntCode IC;
struct IntCode ICbackup;


void part1();
void part2();

void setup();
void reset();
long long run();
bool test_output();

long long operand();
void adv();
void bxl();
void bst();
void jnz();
void bxc();
int out();
void bdv();
void cdv();
void print_output();



int main(int argc, char *argv[])
{
	clock_t begin = clock();
	
	fix_file(argv, "M");

	setup(&IC);
	setup(&ICbackup);
	IC.A2 = IC.A;

	run_parts(begin);
}

void part1() 
{
	long long answer = 0;

	run();

	printf("Part 1: ");
	print_output();
}

void part2()
{
	long long answer = 0;
	long long rep_A = 0;
	long long tmp;
	bool possible;
	long_long_Queue_t queue;
	init_ll(&queue, INT_MAX);
	for(size_t i = 0; i < 8; i++)
	{
		push_back_ll(i, &queue);
	}
	
	while(true)
	{
		reset();
		rep_A = pop_front_ll(&queue);
		IC.A = rep_A;
		run();
		if(test_output())
			for(size_t i = 0; i < 8; i++)
				push_back_ll(i + rep_A * 8, &queue);

		if(!memcmp(IC.reg, IC.output, sizeof(long long) * IC.length) != 0)
		{
			answer = rep_A;
			break;
		}
	}

	printf("Part 2: %lld", answer);
}


void setup(struct IntCode *IntC)
{
	char game[4096];
	char **tokens;
	char **tokensg;
	size_t i = 0;
	IntC->output = calloc(OUTPUT_LENGTH, sizeof(long long));
	strcpy(game, file.file[4]);
	IntC->current = 0;
	IntC->place_output = 0;

	tokens = str_split(game, ' ', false);
	tokensg = tokens;
	tokens = str_split(*(tokens + 1), ',', false);
	while(*(tokens + i) != NULL)
	{
		i++;
	}
	IntC->reg = calloc(i, sizeof(long long));
	IntC->length = i;
	for(size_t j = 0; j < i; j++)
	{
		*(IntC->reg + j) = str_ll(*(tokens + j));
	}

	strcpy(game, file.file[0]);
	tokens = str_split(game, ':', false);
	IntC->A = str_ll(*(tokens + 1) + 1);

	strcpy(game, file.file[1]);
	tokens = str_split(game, ':', false);
	IntC->B = str_ll(*(tokens + 1) + 1);

	strcpy(game, file.file[2]);
	tokens = str_split(game, ':', false);
	IntC->C = str_ll(*(tokens + 1) + 1);

	return;
}

void reset()
{
	memcpy(IC.reg, ICbackup.reg, IC.length);
	memcpy(IC.output, ICbackup.output, OUTPUT_LENGTH);
	IC.current = 0;
	IC.place_output = 0;
}

long long run()
{
	while(IC.current < IC.length)
	{
		switch(*(IC.reg + IC.current))
		{
			case 0:
				adv();
				break;
			case 1:
				bxl();
				break;
			case 2:
				bst();
				break;
			case 3:
				jnz();
				break;
			case 4:
				bxc();
				break;
			case 5:
				out();
				break;
			case 6:
				bdv();
				break;
			case 7:
				cdv();
				break;
			default:
				return 0;
		}
	}
	return 1;
}

bool test_output()
{
	size_t j = 0;
	for(size_t i = IC.length - IC.place_output; i < IC.length; i++)
	{
		if(IC.output[j] != ICbackup.reg[i])
			return false;
		j++;
	}
	return true;
}

long long operand(long long number)
{
	switch(number)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			return number;
		case 4:
			return IC.A;
		case 5:
			return IC.B;
		case 6:
			return IC.C;
		case 7:
			printf("ERROR CODE 7");
			exit(7);
	}
}

void adv()
{
	IC.A = IC.A >> operand(*(IC.reg + IC.current + 1));
	IC.current += 2;
}

void bxl()
{
	IC.B = *(IC.reg + IC.current + 1) ^ IC.B;
	IC.current += 2;
}

void bst()
{
	IC.B = IC.B = (operand(*(IC.reg + IC.current + 1)) & 0b111);
	IC.current += 2;
}

void jnz()
{
	if(IC.A != 0)
	{
		IC.current = *(IC.reg + IC.current + 1);
	}
	else
		IC.current += 2;
}

void bxc()
{
	IC.B = IC.C ^ IC.B;
	IC.current += 2;
}

int out()
{
	IC.output[IC.place_output] = (operand(*(IC.reg + IC.current + 1)) & 0b111);
	IC.place_output++;
	IC.current += 2;
	return 0;
}
void bdv()
{
	IC.B = IC.A >> operand(*(IC.reg + IC.current + 1));
	IC.current += 2;
}

void cdv()
{
	IC.C = IC.A >> operand(*(IC.reg + IC.current + 1));
	IC.current += 2;
}

void print_output()
{
	for(size_t i = 0; i < IC.place_output; i++)
	{
		printf("%lld,", IC.output[i]);
	}
}