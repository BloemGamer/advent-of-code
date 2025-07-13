#include "../libraries/adventofcode.c" //V5.1

struct Data
{
	size_t start_y, start_x, y, x, current_direction;
	size_t horizontal[4];
	size_t vertical[4];
	char direction;
};
struct Data data;

void search_start();
bool walk();
long long check_amount_walked();

int main(int argc, char *argv[])
{
	clock_t begin = clock();
	
	fix_file(argv, "M");

	search_start();
	data.horizontal[0] = 1;
	data.horizontal[1] = 0;
	data.horizontal[2] = -1;
	data.horizontal[3] = 0;

	data.vertical[0] = 0;
	data.vertical[1] = 1;
	data.vertical[2] = 0;
	data.vertical[3] = -1;
	
	run_parts(begin);
}

void part1() 
{
	long long answer = 0;
	walk(file.file);
	answer = check_amount_walked(file.file);
	printf("Part 1: %lld", answer);
}

void part2()
{
	long long answer = 0;
	char *board_row;
	char *tmp;
	board_row = calloc(file.lengthlines[0], sizeof(char));
	for(size_t i = 0; i < file.amountlines; i++)
		for(size_t j = 0; j < file.lengthlines[i]; j++)
		{
			strcpy(board_row, file.file[i]);
			board_row[j] = '#';
			tmp = file.file[i];
			file.file[i] = board_row;

			if(walk(file.file) == true)
				answer++;

			file.file[i] = tmp;
		}
	printf("Part 2: %lld", answer);
}

void search_start()
{
	for(size_t y = 0; y < file.amountlines; y++)
		for(size_t x = 0; x < file.lengthlines[y]; x++)
		{
			if(file.file[y][x] == 'v' || file.file[y][x] == '<' || file.file[y][x] == '^' || file.file[y][x] == '>')
			{
				data.start_y = y;
				data.start_x = x;
				data.y = y;
				data.x = x;
				data.direction = file.file[y][x];
				if(file.file[y][x] == '>')
					data.current_direction = 0;
				if(file.file[y][x] == 'v')
					data.current_direction = 1;
				if(file.file[y][x] == '<')
					data.current_direction = 2;
				if(file.file[y][x] == '^')
					data.current_direction = 3;
				return;
			}
		}
}

bool walk(char **board)
{
	size_t amount_steps = 0;
	while(amount_steps < 100000)
	{
		while(board[data.y + data.vertical[data.current_direction]][data.x + data.horizontal[data.current_direction]] != '#')
		{
			board[data.y][data.x] = ',';
			data.y += data.vertical[data.current_direction];
			data.x += data.horizontal[data.current_direction];
			if(data.y + data.vertical[data.current_direction] < 0 || data.y + data.vertical[data.current_direction] >= file.amountlines)
			{
				board[data.y][data.x] = ',';
				data.y = data.start_y;
				data.x = data.start_x;
				if(data.direction == '>')
					data.current_direction = 0;
				if(data.direction == 'v')
					data.current_direction = 1;
				if(data.direction == '<')
					data.current_direction = 2;
				if(data.direction == '^')
					data.current_direction = 3;
				return false;
			}
			if(data.x + data.horizontal[data.current_direction] < 0 || data.x + data.horizontal[data.current_direction] >= file.lengthlines[data.y])
			{
				board[data.y][data.x] = ',';
				data.y = data.start_y;
				data.x = data.start_x;
				if(data.direction == '>')
					data.current_direction = 0;
				if(data.direction == 'v')
					data.current_direction = 1;
				if(data.direction == '<')
					data.current_direction = 2;
				if(data.direction == '^')
					data.current_direction = 3;
				return false;
			}
			amount_steps++;
		}
		data.current_direction++;
		data.current_direction = data.current_direction % 4;
		amount_steps++;
	}
	if(data.direction == '>')
		data.current_direction = 0;
	if(data.direction == 'v')
		data.current_direction = 1;
	if(data.direction == '<')
		data.current_direction = 2;
	if(data.direction == '^')
		data.current_direction = 3;
	data.y = data.start_y;
	data.x = data.start_x;
	return true;
}

long long check_amount_walked(char **board)
{
	long long answer = 0;
	for(size_t y = 0; y < file.amountlines; y++)	
		for(size_t x = 0; x < file.lengthlines[y]; x++)
			if(board[y][x] == ',')
				answer++;

	return answer;
}