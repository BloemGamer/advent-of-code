#include "../libs/old/bloem.h"
#include <iostream>
#include <chrono>
#include <climits>
#include <thread>
#include <unordered_map>

filecontent file;

unsigned long long prune_number = 0b1000000000000000000000000 - 0b1;
long long *file_numbers;

std::unordered_map<int, int> price_change;
std::unordered_map<int, int> diff_hash[1892];

void part1();
void part2();

long long secret_number(long long number, size_t line);
long long max_bananas();
long long run(long long number_win);
int make_win_number(long long a, long long b, long long c, long long d);


int main(int argc, char **argv)
{
	std::chrono::time_point<std::chrono::system_clock>start, end0, end1, end2;
	start = std::chrono::system_clock::now();
	file.give_argv(argv);
	file.fix_file("M");
	file_numbers = (long long*)calloc(file.amount_lines + 1, sizeof(long long));
	for(size_t i = 0; i < file.amount_lines; i++)
		*(file_numbers + i) = atoll(file.file[i]);
	price_change.reserve(5000);

	std::cout << "Reading file: ";
	end0 = std::chrono::system_clock::now();
	std::chrono::duration<double> seconds = end0 - start;

	std::cout << seconds.count() * 1000 << "ms\n";
	part1();

	end1 = std::chrono::system_clock::now();
	seconds = end1 - end0;

	std::cout << seconds.count() * 1000 << "ms\n";
	part2();

	end2 = std::chrono::system_clock::now();
	seconds = end2 - end1;

	std::cout << seconds.count() * 1000 << "ms\n";
	seconds = end2 - start;
	std::cout << "Total: "<< seconds.count() * 1000 << "ms\n";
}

void part1()
{
	long long answer = 0;

	for(size_t i = 0; i < file.amount_lines; i++)
		answer += secret_number(file_numbers[i], i);

	std::cout << "Part 1: " << answer << "\n";
}

void part2()
{
	long long answer = max_bananas();
	std::cout << "Part 2: " << answer << "\n";
}

long long secret_number(long long number, size_t line)
{
	long long tmp0 = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0, tmp4 = LLONG_MAX;
	int tmp;
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
		
		if(i > 3)
		{
			tmp = make_win_number(tmp0, tmp1, tmp2, tmp3);
			price_change[tmp] = tmp;
			if(diff_hash[line].find(tmp) == diff_hash[line].end())
				diff_hash[line][tmp] = number % 10;
		}
	}
	return number;
}

long long max_bananas()
{
    long long answer = 0;
    int prices;

    for(int d0 = -9; d0 <= 9; d0++)
    {
        for(int d1 = -9; d1 <= 9; d1++)
            for(int d2 = -9; d2 <= 9; d2++)
                for(int d3 = -9; d3 <= 9; d3++)	
                {
					prices = make_win_number(d0, d1, d2, d3);
					if(price_change.find(prices) == price_change.end())
						continue;
						
					answer = __max(answer, run(prices));
				}
	}
	return answer;
}

long long run(long long number_win)
{
	long long answer = 0;
	for(size_t j = 0; j < file.amount_lines; j++)
	{
		if(diff_hash[j].find(number_win) != diff_hash[j].end())
			answer += diff_hash[j][number_win];
	}
	return answer;
}


int make_win_number(long long a, long long b, long long c, long long d)
{
	int tmp = 0;
	tmp += a > (-1 * a) ? (a) : ((-1 * a) + 10);
	tmp += 19 * (b > (-1 * b) ? (b) : ((-1 * b) + 10));
	tmp += 19*19 *(c > (-1 * c) ? (c) : ((-1 * c) + 10));
	tmp += 19*19*19 *(d > (-1 * d) ? (d) : ((-1 * d) + 10));
	return tmp;
}
