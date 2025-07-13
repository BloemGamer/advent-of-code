#include "../libraries/adventofcode.c"

long long search_starts();
long long search_trailheads();

int main(int argc, char *argv[])
{
	clock_t begin = clock();
	fix_file(argv, "M");
	run_parts(begin);
}

void part1()
{
	printf("Part 1: %lld", search_starts(1));
}

void part2()
{
	printf("Part 2: %lld", search_starts(2));
}

long long search_starts(int part)
{
	long long answer = 0;
	for(long long y = 0; y < file.amountlines; y++)
		for(long long x = 0; x < file.lengthlines[y]; x++)
			if(file.file[y][x] == '0')
			{
				answer += search_trailheads(y, x, part);
				if(part == 1)
					for(long long y1 = 0; y1 < file.amountlines; y1++)
						for(long long x1 = 0; x1 < file.lengthlines[y1]; x1++)
							if(file.file[y1][x1] == 'X')
								file.file[y1][x1] = '9';
			}
	return answer;
}

long long search_trailheads(long long y, long long x, int part)
{
	long long answer = 0;
	if(file.file[y][x] == '9')
	{
		if(part == 1)
			file.file[y][x] = 'X';
		return 1;
	}
	else
	{
		for(size_t i = 0; i < 4; i++)
			if(x + directions[1][i] >= 0 && x + directions[1][i] < file.lengthlines[0] && y + directions[0][i] >= 0 && y + directions[0][i] < file.amountlines)
				if(file.file[y + directions[0][i]][x + directions[1][i]] == file.file[y][x] + 1)
					answer += search_trailheads(y + directions[0][i], x + directions[1][i], part);
	}
	return answer;
}