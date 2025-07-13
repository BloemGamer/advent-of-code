#include "../libraries/adventofcode.c" //V1.0

//to low 99048934849633


struct Data 
{
	long long Ay, Ax, By, Bx, Py, Px;
	size_t current_line;
};
struct Data data;

void fill_data();
long long amount_tokens();


int main(int argc, char *argv[])
{
	clock_t begin = clock();
	fix_file(argv, "M");
	run_parts(begin);
}

void part1()
{
	long long answer = 0;
	data.current_line = 0;
	while(data.current_line < file.amountlines)
	{
		fill_data(1);
		answer += amount_tokens();
	}
	printf("Part 1: %lld", answer);
}

void part2()
{
	long long answer = 0;
	data.current_line = 0;

	while(data.current_line < file.amountlines)
	{
		fill_data(2);
		answer += amount_tokens();
	}

	printf("Part 2: %lld", answer);
}

void fill_data(int part)
{
	char game[4096];
	char **tokens;
	while(file.lengthlines[data.current_line] == 0)
		data.current_line++;
	if(data.current_line >= file.amountlines)
		return;
	strcpy(game, file.file[data.current_line]);
	tokens = str_split(game, '+', false);
	data.Ax = str_ll(*(tokens + 1));
	data.Ay = str_ll(*(tokens + 2));
	data.current_line++;

	strcpy(game, file.file[data.current_line]);
	tokens = str_split(game, '+', false);
	data.Bx = str_ll(*(tokens + 1));
	data.By = str_ll(*(tokens + 2));
	data.current_line++;

	if(part == 1)
	{
		strcpy(game, file.file[data.current_line]);
		tokens = str_split(game, '=', false);
		data.Px = str_ll(*(tokens + 1));
		data.Py = str_ll(*(tokens + 2));
		data.current_line++;
	}
	else
	{
		strcpy(game, file.file[data.current_line]);
		tokens = str_split(game, '=', false);
		data.Px = (str_ll(*(tokens + 1))) + 10000000000000;
		data.Py = (str_ll(*(tokens + 2))) + 10000000000000;
		data.current_line++;
	}
}

long long amount_tokens()
{
	long long determinant = data.Ax * data.By - data.Ay * data.Bx;
	long long stepA = data.By * data.Px - data.Bx * data.Py;
	long long stepB = -data.Ay * data.Px + data.Ax * data.Py;
	if(stepA % determinant == 0 && stepB % determinant == 0)
		return stepA / determinant * 3 + stepB / determinant;
	else
		return 0;
}