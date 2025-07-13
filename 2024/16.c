#include "../libraries/adventofcode.c" //V1.0

struct Data
{
	char **map;
	char **map2;
	long long **crossway_map;
	long long amount_crossways;
	int Sy, Sx, Ey, Ex;
};
struct Data data;

struct Flood
{
	long long **flood;
	size_t length;
	long long shortest_path;
};
struct Flood flood;

void fill_data();
long long flood_water();
long long amount_seeds();

int main(int argc, char **argv)
{
	clock_t begin = clock();
	fix_file(argv, "T1");

	run_parts(begin);
	make_debug_file(argv, data.map, "16");
}

void part1()
{
	long long answer = 0;
	fill_data();
	answer = flood_water();

	free(data.map);
	printf("Part 1: %lld", answer);
}

void part2()
{
	long long answer = 0;
	fill_data();
	// answer = amount_seeds();

	printf("Part 2: %lld", answer);
}

void fill_data()
{
	int count;
	data.map = calloc(file.amountlines + 1, sizeof(char*));
	data.crossway_map = calloc(file.amountlines + 1, sizeof(long long*));
	for(size_t y = 0; y < file.amountlines; y++)
	{
		*(data.map + y) = calloc(file.lengthlines[y] + 1, sizeof(char));
		*(data.crossway_map + y) = calloc(file.lengthlines[y], sizeof(long long));
		for(size_t x = 0; x < file.lengthlines[y]; x++)
		{
			switch(file.file[y][x])
			{
				case '.':
					count = 0;
					for(size_t k = 0; k < 4; k++)
						if(file.file[y + directions[0][k]][x + directions[1][k]] == '.')
							count++;
					if(count > 2)
					{
						data.map[y][x] = 'X';
						data.crossway_map[y][x] = data.amount_crossways;
						data.amount_crossways++;
					}
					else if(count == 2 && file.file[y - 1][x] != file.file[y + 1][x])
					{
						data.map[y][x] = 'A';
						data.crossway_map[y][x] = -4;
					}
					else
					{
						data.map[y][x] = file.file[y][x];
						data.crossway_map[y][x] = -5;
					}
					break;
				case 'S':
					data.Sy = y;
					data.Sx = x;
					data.crossway_map[y][x] = -2;
					data.map[y][x] = file.file[y][x];
					break;
				case 'E':
					data.Ey = y;
					data.Ex = x;
					data.crossway_map[y][x] = -3;
					data.map[y][x] = file.file[y][x];
					break;
				case '#':
					data.map[y][x] = file.file[y][x];
					data.crossway_map[y][x] = -1;
					break;
			}
		}
	}
}

long long flood_water()
{
	size_t i, k, l, m;
	long long count = 0;
	flood.length = 1024;
	flood.flood = calloc(flood.length, sizeof(long long*));
	for(i = 0; i < flood.length; i++)
	{
		flood.flood[i] = calloc(3, sizeof(long long));
		flood.flood[i][2] = -1;
	}
	if(data.map[data.Sy - 1][data.Sx] == '.')
	{
		flood.flood[0][0] = data.Sy - 1;
		flood.flood[0][1] = data.Sx;
		flood.flood[0][2] = 1001;
	}
	if(data.map[data.Sy][data.Sx + 1] == '.')
	{
	flood.flood[1][0] = data.Sy;
	flood.flood[1][1] = data.Sx + 1;
	flood.flood[1][2] = 1;
	}
	while(data.map[data.Ey][data.Ex] == 'E' && data.map[data.Ey + 1][data.Ex] == '.' && data.map[data.Ey][data.Ex - 1] == '.')
	{
		count++;
		for(i = 0; i < flood.length; i++)
		{
			if(flood.flood[i][2] == 0)
			{
				data.map[flood.flood[i][0]][flood.flood[i][1]] = 'W';
				for(k = 0; k < 4; k++)
					switch(data.map[flood.flood[i][0] + directions[0][k]][flood.flood[i][1] + directions[1][k]])
					{
						case '.':
							for(l = 0; l < flood.length; l++)
								if(flood.flood[l][2] < 0)
								{
									flood.flood[l][0] = flood.flood[i][0] + directions[0][k];
									flood.flood[l][1] = flood.flood[i][1] + directions[1][k];
									flood.flood[l][2] = 1;
									data.map[flood.flood[l][0]][flood.flood[l][1]] = 'W';
									break;
								}
								else if(l == flood.length - 2)
									printf("run out of queue");
							break;
						case 'A':
							for(l = 0; l < flood.length; l++)
								if(flood.flood[l][2] < 0)
								{
									flood.flood[l][0] = flood.flood[i][0] + directions[0][k];
									flood.flood[l][1] = flood.flood[i][1] + directions[1][k];
									flood.flood[l][2] = 1001;
									data.map[flood.flood[l][0]][flood.flood[l][1]] = 'W';
									break;
								}
							break;
						case 'X':
							data.map[flood.flood[i][0] + directions[0][k]][flood.flood[i][1] + directions[1][k]] = 'W';
							for(m = 0; m < 4; m++)
							{
								if(data.map[flood.flood[i][0] + directions[0][k] + directions[0][m]][flood.flood[i][1] + directions[1][k] + directions[1][m]] == '.')
									for(l = 0; l < flood.length; l++)
										if(flood.flood[l][2] < 0 && directions[0][k] == directions[0][m])
										{
											flood.flood[l][0] = flood.flood[i][0] + directions[0][k] + directions[0][m];
											flood.flood[l][1] = flood.flood[i][1] + directions[1][k] + directions[1][m];
											flood.flood[l][2] = 2;
											data.map[flood.flood[l][0]][flood.flood[l][1]] = 'W';
											break;
										}
										else if(flood.flood[l][2] < 0)
										{
											flood.flood[l][0] = flood.flood[i][0] + directions[0][k] + directions[0][m];
											flood.flood[l][1] = flood.flood[i][1] + directions[1][k] + directions[1][m];
											flood.flood[l][2] = 1002;
											data.map[flood.flood[l][0]][flood.flood[l][1]] = 'W';
											break;
										}
							}
						case 'E':
							if(flood.flood[l][2] < 0)
								{
									flood.flood[l][0] = flood.flood[i][0] + directions[0][k];
									flood.flood[l][1] = flood.flood[i][1] + directions[1][k];
									flood.flood[l][2] = 1;
									data.map[flood.flood[l][0]][flood.flood[l][1]] = 'W';
									break;
								}

					}
			}
			
		}
		for(i = 0; i < flood.length; i++)
			if(flood.flood[i][2] > -1)
					flood.flood[i][2]--;
		// if(count > 1004)
		// 	return count;
	}
	flood.shortest_path = count + 1;
	return flood.shortest_path;
}

long long amount_seeds(long long amount_steps, long long y, long long x, size_t dif_y, size_t dif_x)
{
	if(amount_steps > flood.shortest_path + 1)
		return 0;
	if(y == data.Ey && x == data.Ex)
		return 1;
}