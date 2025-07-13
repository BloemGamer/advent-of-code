#include "../libraries/adventofcode.c" //V5.1

char **antinodes;

void fill_antinodes(char frequency, int part);
long long count_antinodes();

int main(int argc, char *argv[])
{
	clock_t begin = clock();
	fix_file(argv, "M");
	antinodes = calloc(file.amountlines + 1, sizeof(char*));
	for(size_t i = 0; i < file.amountlines; i++)
		*(antinodes + i) = calloc(file.lengthlines[i] + 1, sizeof(char));
	run_parts(begin);
}

void part1() 
{
	long long answer = 0;

	for(char i = 0; i < 127; i++)
		if(i != '.')
			fill_antinodes(i, 1);
	answer = count_antinodes();
	printf("Part 1: %lld", answer);
}

void part2()
{
	long long answer = 0;

	for(char i = 0; i < 127; i++)
		if(i != '.')
			fill_antinodes(i, 2);
	answer = count_antinodes();
	printf("Part 2: %lld", answer);
}

void fill_antinodes(char frequency, int part)
{
	long long yt, xt;
	long long count;
	for(long long y1 = 0; y1 < file.amountlines; y1++)
		for(long long x1 = 0; x1 < file.lengthlines[y1]; x1++)
			if(file.file[y1][x1] == frequency)
				for(long long y2 = 0; y2 < file.amountlines; y2++)
					for(long long x2 = 0; x2 < file.lengthlines[y2]; x2++)
						if(!(x1 == x2 && y1 == y2) && file.file[y2][x2] == frequency)
						{
							yt = y1 - y2;
							xt = x1 - x2;
							count = 0;
							if(part == 1)
								if(x1 + xt >= 0 && x1 + xt < file.lengthlines[0] && y1 + yt >= 0 && y1 + yt < file.amountlines)
									antinodes[y1 + yt][x1 + xt] = '#';
							if(part == 2)
								while(x1 + xt * count >= 0 && x1 + xt * count < file.lengthlines[0] && y1 + yt * count >= 0 && y1 + yt * count < file.amountlines)
								{
									antinodes[y1 + yt * count][x1 + xt * count] = '#';
									count++;
								}
						}
}

long long count_antinodes()
{
	long long answer = 0;
	for(size_t y = 0; y < file.amountlines; y++)
		for(size_t x = 0; x < file.lengthlines[y]; x++)
			if(antinodes[y][x] == '#')
				answer++;
	return answer;
}