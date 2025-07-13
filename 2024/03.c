#include "../libraries/adventofcode.c" //V5.1


long long all_multiply();
bool enabled = true;

int main(int argc, char *argv[])
{
	clock_t begin = clock();
	fix_file(argv, "M");
	run_parts(begin);
}

void part1() 
{
	long long answer = 0;
	for(size_t i = 0; i < file.amountlines; i++)
		answer += all_multiply(file.file[i], 1);
	printf("Part 1: %lld", answer);
}

void part2()
{
	long long answer = 0;
	for(size_t i = 0; i < file.amountlines; i++)
		answer += all_multiply(file.file[i], 2);
	printf("Part 2: %lld", answer);
}

long long all_multiply(char *vstring, int part)
{
	char *game;
	char *current;
	long long first, second, answer = 0;
	game = searchAndReplace(vstring, "*", "_");
	game = searchAndReplace(game, "mul", "*");
	if(part == 2)
	{
		game = searchAndReplace(game, "X", "_");
		game = searchAndReplace(game, "Y", "_");
		game = searchAndReplace(game, "do()", "Y");
		game = searchAndReplace(game, "don't()", "X");
	}
	current = game;
	while(current <= game + strlen(game))
	{
		if(enabled)
		{
			if(*current == 'X')
			{
				enabled = false;
				current++;
			}
			else if(*current == '*')
			{
				current++;
				if(*(current++) == '(')
				{
					first = str_ll(current);
					while(*(current) >= '0' && *(current) <= '9')
						current++;
					if(*(current++) == ',')
					{
						second = str_ll(current);
						while(*(current) >= '0' && *(current) <= '9')
						current++;
						if(*(current++) == ')')
						{
							answer += first * second;
						}
					}
					
				}
			}
			else
				current++;
		}
		else
		{
			if(*current == 'Y')
				enabled = true;
			current++;
		}
	}
	return answer;
}