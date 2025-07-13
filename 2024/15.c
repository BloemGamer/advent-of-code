#include "../libraries/adventofcode.c" //V1.0

struct Data
{
	char **map;
	char **map2;
	int *direction;
	long long y, x;
	size_t amount_instructions;
	size_t map_y, map_x;
};
struct Data data;


bool test_boxes();
void run();
long long sum_coordinates();
void fill_data();
void fill_data2();

int main(int argc, char *argv[])
{
	clock_t begin = clock();
	fix_file(argv, "M");

	
	run_parts(begin);
}

void part1()
{
	long long answer = 0;

	fill_data();
	run(1);
	answer = sum_coordinates();
	free(data.map);

	printf("Part 1: %lld", answer);
}

void part2()
{
	long long answer = 0;

	fill_data2();
	run(2);
	answer = sum_coordinates();
	free(data.map);
	free(data.map2);

	printf("Part 2: %lld", answer);
}

void run(int part)
{
	int tmp;
	size_t j;
	for(size_t i = 0; i < data.amount_instructions; i++)
	{
		tmp = data.direction[i];
		if(part == 2)
			for(j = 0; j < data.map_y; j++)
				strcpy(data.map2[j], data.map[j]);
		if(test_boxes(data.y, data.x, directions[0][tmp], directions[1][tmp]))
		{
			data.y += directions[0][tmp];
			data.x += directions[1][tmp];
		}
		else if(part == 2)
			for(j = 0; j < data.map_y; j++)
				strcpy(data.map[j], data.map2[j]);
	}
}

bool test_boxes(size_t y, size_t x, int dif_y, int dif_x)
{
	switch(data.map[y + dif_y][x + dif_x])
	{
		case 'O':
			if(test_boxes(y + dif_y, x + dif_x, dif_y, dif_x))
			{
				data.map[y + dif_y][x + dif_x] = data.map[y][x];
				data.map[y][x] = '.';
				return true;
			}
			else
				return false;
		case '.':
			data.map[y + dif_y][x + dif_x] = data.map[y][x];
			data.map[y][x] = '.';
			return true;
		case '#':
			return false;
		case '[':
			if(dif_y == 0)
			{
				if(test_boxes(y + dif_y, x + dif_x, dif_y, dif_x))
				{
					data.map[y + dif_y][x + dif_x] = data.map[y][x];
					data.map[y][x] = '.';
					return true;
				}
				else
					return false;
			}
			else
			{
				if(test_boxes(y + dif_y, x + dif_x, dif_y, dif_x) && test_boxes(y + dif_y, x + dif_x + 1, dif_y, dif_x))
				{
					data.map[y + dif_y][x + dif_x] = data.map[y][x];
					data.map[y][x] = '.';
					return true;
				}
				else
					return false;
			}
		case ']':
			if(dif_y == 0)
			{
				if(test_boxes(y + dif_y, x + dif_x, dif_y, dif_x))
				{
					data.map[y + dif_y][x + dif_x] = data.map[y][x];
					data.map[y][x] = '.';
					return true;
				}
				else
					return false;
			}
			else
			{
				if(test_boxes(y + dif_y, x + dif_x, dif_y, dif_x) && test_boxes(y + dif_y, x + dif_x - 1, dif_y, dif_x))
					{
						data.map[y + dif_y][x + dif_x] = data.map[y][x];
						data.map[y][x] = '.';
						return true;
					}
					else
						return false;
			}
	}
}

long long sum_coordinates()
{
	long long answer = 0;
	for(size_t y = 0; y < data.map_y; y++)
		for(size_t x = 0; x < data.map_x; x++)
			if(data.map[y][x] == 'O' || data.map[y][x] == '[')
				answer += 100 * y + x;

	return answer;
}

void fill_data()
{
	size_t y = 0, start_directions, tmp;
	while(file.lengthlines[y] != 0)
		y++;

	data.map = calloc(y + 1, sizeof(char*));
	data.map_y = y;
	data.map_x = file.lengthlines[0];
	y = 0;
	while(file.lengthlines[y] != 0)
	{
		*(data.map + y) = calloc(file.lengthlines[y] + 1, sizeof(char));
		for(size_t x = 0; x < file.lengthlines[y]; x++)
		{
			data.map[y][x] = file.file[y][x];
			if(data.map[y][x] == '@')
			{
				data.y = y;
				data.x = x;
			}
		}
		y++;
	}

	y++;
	start_directions = y;
	tmp = 0;
	while(y < file.amountlines)
	{
		tmp += strlen(file.file[y]);
		y++;
	}
	y = start_directions;
	data.direction = calloc(tmp, sizeof(tmp));
	while(y < file.amountlines)
	{
		for(size_t x = 0; x < file.lengthlines[y]; x++)
		{
			switch(file.file[y][x])
			{
				case 'v':
					data.direction[file.lengthlines[y] * (y - start_directions) + x] = 0;
					break;
				case '^':
					data.direction[file.lengthlines[y] * (y - start_directions) + x] = 1;
					break;
				case '>':
					data.direction[file.lengthlines[y] * (y - start_directions) + x] = 2;
					break;
				case '<':
					data.direction[file.lengthlines[y] * (y - start_directions) + x] = 3;
					break;

				default:
					printf("TF");
					break;
			}
		}
		y++;
	}
	data.amount_instructions = file.lengthlines[file.amountlines - 1] * (y - start_directions);
}

void fill_data2()
{
	size_t y = 0, start_directions, tmp;
	while(file.lengthlines[y] != 0)
		y++;

	data.map = calloc(y + 1, sizeof(char*));
	data.map2 = calloc(y + 1, sizeof(char*));
	data.map_y = y;
	data.map_x = file.lengthlines[0] * 2;
	y = 0;
	while(file.lengthlines[y] != 0)
	{
		*(data.map + y) = calloc((file.lengthlines[y] * 2) + 1, sizeof(char));
		*(data.map2 + y) = calloc((file.lengthlines[y] * 2) + 1, sizeof(char));
		for(size_t x = 0; x < file.lengthlines[y]; x++)
		{
			switch(file.file[y][x])
			{
				case 'O':
					data.map[y][2 * x] = '[';
					data.map[y][(2 * x) + 1] = ']';
					break;
				case '.':
					data.map[y][2 * x] = '.';
					data.map[y][(2 * x) + 1] = '.';
					break;
				case '@':
					data.map[y][2 * x] = '@';
					data.map[y][(2 * x) + 1] = '.';
					data.y = y;
					data.x = x * 2;
					break;
				case '#':
					data.map[y][2 * x] = '#';
					data.map[y][(2 * x) + 1] = '#';
					break;
				
			}
		}
		y++;
	}

	y++;
	start_directions = y;
	tmp = 0;
	while(y < file.amountlines)
	{
		tmp += strlen(file.file[y]);
		y++;
	}
	data.amount_instructions = tmp;
	y = start_directions;
	data.direction = calloc(tmp, sizeof(tmp));
	while(y < file.amountlines)
	{
		for(size_t x = 0; x < file.lengthlines[y]; x++)
		{
			switch(file.file[y][x])
			{
				case 'v':
					data.direction[file.lengthlines[y] * (y - start_directions) + x] = 0;
					break;
				case '^':
					data.direction[file.lengthlines[y] * (y - start_directions) + x] = 1;
					break;
				case '>':
					data.direction[file.lengthlines[y] * (y - start_directions) + x] = 2;
					break;
				case '<':
					data.direction[file.lengthlines[y] * (y - start_directions) + x] = 3;
					break;

				default:
					printf("FUCK deze shit");
					break;
			}
		}
		y++;
	}	
}