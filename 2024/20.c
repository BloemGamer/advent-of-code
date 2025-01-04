#include "../libraries/adventofcode.c" //V1.0

// #define TEST

#ifdef TEST
	#define FILE_TYPE "T1"
	#define LENGTH_SHORTCUT 20
#else
	#define FILE_TYPE "M"
	#define LENGTH_SHORTCUT 100
#endif

struct Data 
{
	long long **map;
	size_t Sy, Sx, Ey, Ex;	
};
struct Data data;

void fill_data();
void fill_steps();
long long count_shortcuts();

int main(int argc, char **argv)
{
	clock_t begin = clock();
	fix_file(argv, FILE_TYPE);


	run_parts(begin);
}

void part1()
{
	long long answer = 0;
	fill_data();
	fill_steps();
	answer = count_shortcuts(2);

	printf("Part 1: %lld", answer);
}

void part2()
{
	long long answer = 0;
	fill_data();
	fill_steps();
	answer = count_shortcuts(20);

	printf("Part 2: %lld", answer);
}

void fill_data()
{
	data.map = (long long**)calloc(file.amountlines, sizeof(long long*));
	for(size_t y = 0; y < file.amountlines; y++)
	{
		*(data.map + y) = (long long*)calloc(file.lengthlines[y], sizeof(long long));
		for(size_t x = 0; x < file.lengthlines[y]; x++)
		{
			switch(file.file[y][x])
			{
				case '#':
					data.map[y][x] = -1;
					break;
				case 'S':
					data.Sy = y;
					data.Sx = x;
					data.map[y][x] = -2;
					break;
				case 'E':
					data.Ey = y;
					data.Ex = x;
					data.map[y][x] = -2;
					break;
				case '.':
					data.map[y][x] = -2;
					break;
				default:
					printf("TF");
					break;
			}
		}
	}
}

void fill_steps()
{
	size_t y = data.Sy, x = data.Sx;
	long long count = 0;
	while(y != data.Ey || x != data.Ex)
	{
		data.map[y][x] = count;
		for(size_t k = 0; k < 4; k++)
			if(data.map[y + directions[0][k]][x + directions[1][k]] == -2)
			{
				y += directions[0][k];
				x += directions[1][k];
				break;
			}
		count++;
	}
	data.map[y][x] = count;
}

long long count_shortcuts(int max_amount)
{
	int dif_y, dif_x;
	
	long long answer = 0;
	for(size_t y = 1; y < file.amountlines - 1; y++)
		for(size_t x = 1; x < file.lengthlines[y] - 1; x++)
			for(dif_y = -1 * max_amount; dif_y <= max_amount; dif_y++)
				for(dif_x = -1 * max_amount; dif_x <= max_amount; dif_x++)
				{
					if(_abs64(dif_y) + _abs64(dif_x) > max_amount)
						continue;
					// if(!(in_bounds(y, x, 0, 0, file.amountlines, file.lengthlines[y], dif_y, dif_x)))
					// 	continue;	
					if(!(in_bounds(y, 0, file.amountlines - 1, dif_y, x, 0, file.lengthlines[y], dif_x)))
						continue;
					if((data.map[y + dif_y][x +  dif_x] - data.map[y][x]) >= LENGTH_SHORTCUT + _abs64(dif_y) + _abs64(dif_x) && data.map[y][x] != -1 && data.map[y + dif_y][x + dif_x] != -1)
						answer++;
				}
	return answer;
}