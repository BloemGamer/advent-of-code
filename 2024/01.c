#include "..\libraries\adventofcode.c" //V5.1

long long left[1005];
long long right [1005];

void bubbleSort(long long arr[], size_t n);
void swap(long long* xp, long long* yp);

int main(int argc, char *argv[])
{
	clock_t begin = clock();
	fix_file(argv, "M");
	char **tokens;
	char game[99];
	for(size_t i = 0; i < file.amountlines; i++)
	{
		strcpy(game, file.file[i]);
		tokens = str_split(game, ' ', false);
		left[i] = str_ll(tokens[0]);
		right[i] = str_ll(tokens[1]);
	}
	bubbleSort(left, file.amountlines);
	bubbleSort(right, file.amountlines);
	run_parts(begin);
}

void part1() 
{
	long long answer = 0;
	long long difference;
	for(size_t i = 0; i < file.amountlines; i++)
	{
		difference = left[i] - right[i];
		if(difference < 0)
			difference = difference * -1;
		answer = answer + difference;
	}
	printf("Part 1: %lld", answer);
}

void part2()
{
	long long answer = 0;

	for(size_t i = 0; i < file.amountlines; i++)
		for(size_t j = 0; j < file.amountlines; j++)
			if(left[i] == right[j])
				answer = answer + left[i];
	printf("Part 2: %lld", answer);
}

void bubbleSort(long long arr[], size_t n)
{
    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++)
	{
        swapped = false;
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                swapped = true;
            }
        if (swapped == false)
            break;
    }
}


void swap(long long* xp, long long* yp)
{
    long long temp = *xp;
    *xp = *yp;
    *yp = temp;
}