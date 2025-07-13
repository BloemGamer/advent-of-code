#include "../libraries/adventofcode.c" //V1.0

// too high 922886

char **map;
long long perimeter;

long long check_cost(char what, size_t y, size_t x, int part);
long long check_area(char what, size_t y, size_t x);
long long check_perimeter(char what, size_t y, size_t x);

int main(int argc, char **argv)
{
	clock_t begin = clock();
	fix_file(argv, "M");

	map = calloc((file.amountlines + 3), sizeof(char*));
	assert(!(map == NULL));
	for(size_t i = 0; i < file.amountlines + 2; i++)
	{
		*(map + i) = calloc(file.lengthlines[0] + 3, sizeof(char));
		assert(!(*(map + i) == NULL));
	}


	run_parts(begin);
}

void part1()
{
	long long answer = 0;

	for(size_t y = 0; y < file.amountlines; y++)
		for(size_t x = 0; x < file.lengthlines[y]; x++)
			if(file.file[y][x] >= 'A' && file.file[y][x] <= 'Z')
				answer += check_cost(file.file[y][x], y, x, 1);

				
	for(size_t y = 0; y < file.amountlines; y++)
		for(size_t x = 0; x < file.lengthlines[y]; x++)
			file.file[y][x] = file.file[y][x] - ('a' - 'A');

	printf("Part 1: %lld", answer);
}

void part2()
{
	long long answer = 0;

	for(size_t y = 0; y < file.amountlines; y++)
		for(size_t x = 0; x < file.lengthlines[y]; x++)
			if(file.file[y][x] >= 'A' && file.file[y][x] <= 'Z')
				answer += check_cost(file.file[y][x], y, x, 2);

	printf("Part 2: %lld", answer);
}

long long check_cost(char what, size_t y, size_t x, int part)
{
	perimeter = 0;
	long long tmp1;
	long long tmp2;
	if(part == 1)
		return check_area(what, y, x) * perimeter;
	else
	{
		for(size_t i = 0; i < file.amountlines + 2; i++)
			memset(*(map + i), '.', file.lengthlines[0] + 2);
		tmp1 = check_area(what, y, x);
		tmp2 = check_perimeter(what, y, x);
		return  tmp1 * tmp2;
	}
}

long long check_area(char what, size_t y, size_t x)
{
	long long area = 0;
	
	if(file.file[y][x] == what)
	{
		area++;
		file.file[y][x] = what + ('a' - 'A');
		map[y +  1][x + 1] = '#';
		for(size_t i = 0; i < 4; i++)
		{
			if(x + directions[1][i] >= 0 && x + directions[1][i] < file.lengthlines[0] && y + directions[0][i] >= 0 && y + directions[0][i] < file.amountlines)
			{
				area += check_area(what, y + directions[0][i], x + directions[1][i]);
			}
			else
			{
				perimeter++;
				
			}
		}
	}
	else if(file.file[y][x] != what + ('a' - 'A'))
	{
		perimeter++;
	}

	
	return area;
}

long long check_perimeter(char what, size_t y, size_t x)
{
	long long perimeter = 0;
	int amount_sides[8];
	int place_in_string[4] = {2, 3, 1, 0};
	int place_string_cross[4] = {0, 3, 2, 1};
	int where_sides;
	for(size_t y1 = 1; y1 < file.amountlines + 1; y1++)
		for(size_t x1 = 1; x1 < file.lengthlines[0] + 1; x1++)
		{
			if(map[y1][x1] == '#')
			{
				for(size_t i = 0; i < 8; i++)
					amount_sides[i] = 0;

				for(size_t i = 0; i < 8; i++)
					if(map[y1 + directions[0][i]][x1 + directions[1][i]] == '#')
						amount_sides[i] = 1;
//	DURL
//	RLUD
//  DR DL UR UL
				for(size_t i = 0; i < 4; i++)
				{
					if(amount_sides[i] == 0 && amount_sides[place_in_string[i]] == 0)
						where_sides++;
					if(amount_sides[i] != 0 && amount_sides[place_in_string[i]] != 0)
						if(amount_sides[4 + (place_string_cross[i])] == 0)
							where_sides++;

				}
				



				// if(amount_sidesLR == 2 && amount_sidesUD == 2)
				// 	perimeter += 4;
				// else if(amount_sidesLR > 0 && amount_sidesUD > 0)
				// 	perimeter += amount_sidesLR + amount_sidesUD - 1;
			}
			// if(map[y1][x1] != '#')
			// {
			// 	amount_sidesLR = 0;
			// 	amount_sidesUD = 0;
			// 	for(size_t i = 0; i < 2; i++)
			// 		if(map[y1 + directions[0][i]][x1 + directions[1][i]] == '#')
			// 			amount_sidesUD++;
			// 	for(size_t i = 2; i < 4; i++)
			// 		if(map[y1 + directions[0][i]][x1 + directions[1][i]] == '#')
			// 			amount_sidesLR++;
			// 	if(amount_sidesLR == 2 && amount_sidesUD == 2)
			// 		perimeter += 4;
			// 	else if(amount_sidesLR > 0 && amount_sidesUD > 0)
			// 	{
			// 		perimeter += amount_sidesLR + amount_sidesUD - 1;
			// 	}
			// }
		}
	perimeter = where_sides;
	return perimeter;
}