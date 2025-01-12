#include "../libs/bloem.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <unordered_map>


long long answer2;
long long **diff;
long long **numbers;
unsigned long long prune_number = 0b1000000000000000000000000 - 0b1;
long long *file_numbers;

std::unordered_map<int, int> price_change;
std::unordered_map<int, int> diff_hash[1892];

void part1();
void part2();

long long secret_number(long long number, size_t line);
void max_bananas();
void run(long long number_win);
void run2(long long number_win);
int make_win_number(long long a, long long b, long long c, long long d);


int main(int argc, char **argv)
{
	std::chrono::time_point<std::chrono::system_clock>start, end0, end1, end2;
	start = std::chrono::system_clock::now();
	fix_file(argv, "M");
	file_numbers = (long long*)calloc(file.amountlines + 1, sizeof(long long));
	for(size_t i = 0; i < file.amountlines; i++)
		*(file_numbers + i) = atoll(file.file[i]);
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
	std::cout << seconds.count() << "s\n";
	seconds = end2 - start;
	std::cout << "Total: "<< seconds.count() << "s\n";
}

void part1()
{
	long long answer = 0;
	diff = (long long**)calloc(file.amountlines, sizeof(long long*));
	numbers = (long long**)calloc(file.amountlines, sizeof(long long*));

	for(size_t i = 0; i < file.amountlines; i++)
		answer += secret_number(file_numbers[i], i);

	std::cout << "Part 1: " << answer << "\n";
}

void part2()
{
	max_bananas();
	std::cout << "Part 2: " << answer2 << "\n";
}

long long secret_number(long long number, size_t line)
{
	*(diff + line) = (long long*)calloc(2000, sizeof(long long));
	*(numbers + line) = (long long*)calloc(2000, sizeof(long long));
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
		
		*(*(numbers + line) + i) = number;
		*(*(diff + line) + i) = 0;
		if(i > 3)
		{
			tmp = make_win_number(tmp0, tmp1, tmp2, tmp3);
			*(*(diff + line) + i) = tmp;
			price_change[tmp] = tmp;
			if(diff_hash[line].find(tmp) == diff_hash[line].end())
				diff_hash[line][tmp] = number % 10;
		}
	}
	return number;
}

void max_bananas()
{
	long long tmp = 0;
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
					run(prices);
					answer2 = __max(answer2, tmp);
					tmp = 0;
				}
	}
}

void run(long long number_win)
{
	long long answer = 0;
	for(size_t j = 0; j < file.amountlines; j++)
	{
		if(diff_hash[j].find(number_win) != diff_hash[j].end())
			answer += diff_hash[j][number_win];
	}
	answer2 = __max(answer2, answer);
}

void run2(long long number_win)
{
	long long answer = 0;
	for(size_t j = 0; j < file.amountlines; j++)
	{
		for(size_t i = 0; i < 2000; i++)
		{
			if(number_win == *(*(diff + j) + i))
			{
				answer += *(*(numbers + j) + i) % 10;
				break;
			}
		}
	}
	answer2 = __max(answer2, answer);
}

int make_win_number(long long a, long long b, long long c, long long d)
{
	int tmp;
	tmp += a > (-1 * a) ? (a) : ((-1 * a) + 10);
	tmp += 20 * (b > (-1 * b) ? (b) : ((-1 * b) + 10));
	tmp += 20*20 *(c > (-1 * c) ? (c) : ((-1 * c) + 10));
	tmp += 20*20*20 *(d > (-1 * d) ? (d) : ((-1 * d) + 10));
	return tmp;
}
