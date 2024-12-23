#include "../libraries/adventofcode.c" //V1.0

unsigned long long prune_number = 0b1000000000000000000000000 - 0b1;

long long secret_number();
long long max_bananas();
long long run();


int main(int argc, char **argv)
{
	clock_t begin = clock();
	fix_file(argv, "M");


	run_parts(begin);
}

void part1()
{
	long long answer = 0;

	for(size_t i = 0; i < file.amountlines; i++)
		answer += secret_number(str_ll(file.file[i]));

	printf("Part 1: %lld", answer);
}

void part2()
{
	long long answer = 0;

	answer = max_bananas();

	printf("Part 2: %lld", answer);
}

long long secret_number(long long number)
{
	long long answer = 0;
	long long tmp;
	for(size_t i = 0; i < 2000; i++)
	{
		number = ((number << 6) ^ number) & prune_number;
		number = ((number >> 5) ^ number) & prune_number;
		number = ((number << 11) ^ number) & prune_number;
	}
	return number;
}

long long max_bananas()
{
	long long answer = 0;
	long long tmp;
	
	for(int d0 = -9; d0 <= 9; d0++)
	{
		for(int d1 = -9; d1 <= 9; d1++)
			for(int d2 = -9; d2 <= 9; d2++)
				for(int d3 = -9; d3 <= 9; d3++)
				{
					if(10 + d0 + d1 + d2 + d3 < 0)
						continue;
					for(size_t i = 0; i < file.amountlines; i++)
						tmp += run(str_ll(file.file[i]), d0, d1, d2, d3);
					answer = __max(answer, tmp);
					tmp = 0;
				}
		printf("%d\n", d0);	
	}
	return answer;
}

long long run(long long number, int d0, int d1, int d2, int d3)
{
	long long answer = 0;
	long long tmp0 = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0, tmp4 = INT_MIN;
	for(size_t i = 0; i < 2000; i++)
	{
		number = ((number << 6) ^ number) & prune_number;
		number = ((number >> 5) ^ number) & prune_number;
		number = ((number << 11) ^ number) & prune_number;

		tmp0 = tmp1;
		tmp1 = tmp2;
		tmp2 = tmp3;
		tmp3 = (number % 10) - tmp4;
		tmp4 = number % 10;
		if(d0 == tmp0 && d1 == tmp1 && d2 == tmp2 && d3 == tmp3)
		{
			return number % 10;
		}
	}
	return 0;
}