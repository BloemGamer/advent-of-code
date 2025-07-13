#include "../libraries/adventofcode.c" //V5.1

long long check();
long long check_xmas();
long long check2();
long long check_mas();

int main(int argc, char *argv[])
{
	clock_t begin = clock();
	fix_file(argv, "M");
	run_parts(begin);
}

void part1() 
{
	long long answer;
	answer = check();
	printf("Part 1: %lld", answer);
}

void part2()
{
	long long answer;
	answer = check2();
	printf("Part 2: %lld", answer);
}

long long check()
{
	long long answer = 0; 
	for(size_t y = 0; y < file.amountlines; y++)
		for(size_t x = 0; x < file.lengthlines[0]; x++)
			if(file.file[y][x] == 'X')
				for(size_t i = 0; i < 8; i++)
					answer += check_xmas(y, x, directions[0][i], directions[1][i]);
	return answer;
}

long long check_xmas(size_t y, size_t x, int dif_y, int dif_x)
{
	if((x + dif_x < file.lengthlines[0]) && (y + dif_y < file.amountlines) && (file.file[y + dif_y][x + dif_x] == 'M'))
		if((x + 2 * dif_x < file.lengthlines[0]) && (y + 2 *dif_y < file.amountlines) &&  (file.file[y + 2 * dif_y][x + 2 * dif_x] == 'A'))
			if((x + 3 * dif_x < file.lengthlines[0]) && (y + 3 * dif_y < file.amountlines) && (file.file[y + 3 * dif_y][x + 3 * dif_x] == 'S'))
				return 1;
	return 0;
}

long long check2()
{
	long long answer = 0;
	long long possible;
	for(size_t y = 0; y < file.amountlines; y++)
		for(size_t x = 0; x < file.lengthlines[0]; x++)
		{
			possible = 0;
			if(file.file[y][x] == 'A')
			{
				for(size_t i = 4; i < 8; i++)
					possible += check_mas(y, x, directions[0][i], directions[1][i]);
				if(possible == 2)
					answer++;
			}
		}
	return answer;
}

long long check_mas(size_t y, size_t x, int dif_y, int dif_x)
{
	if((x + -dif_x < file.lengthlines[0]) && (y + -dif_y < file.amountlines) && (file.file[y - dif_y][x - dif_x] == 'M'))
		if((x + dif_x < file.lengthlines[0]) && (y + dif_y < file.amountlines) &&  (file.file[y + dif_y][x + dif_x] == 'S'))
			return 1;
	return 0;
}