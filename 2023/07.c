#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "..\libraries\adventofcode.c" //V5.0

#define amountdecks 1000
#define decksize 5

#define Five_of_a_kind 7 * pow(100, 6)
#define Four_of_a_kind 6 * pow(100, 6)
#define Full_house 5 * pow(100, 6)
#define Three_of_a_kind 4 * pow(100, 6)
#define Two_pair 3 * pow(100, 6)
#define One_pair 2 * pow(100, 6)
#define High_card 1 * pow(100, 6)

struct Data
{
	char deck[amountdecks][decksize];
	size_t decktype[amountdecks];
	size_t rank[amountdecks];
	size_t rank2[amountdecks];
};

struct Data data;


void part1();
void part2();

void fill_data();
void type_deck();
void type_deck2();
void highest_in_string();
size_t win();

void swap();
void bubblesort_custom();
void bubblesort_custom2();


int main(void)
{
	clock_t begin = clock();
	const char *filename = "txt\\7.txt";
	file = readfile(filename);
	assert(amountdecks + 1> file.lengthfile); //if the file is longer than the string you want to put the file into

	fill_data();

	//printf("%llu", ULONG_LONG_MAX);


	clock_t begin1 = clock();
	printf("\nThings for 1 and 2: %lfms\n\n", (double)(begin1 - begin));
	part1();
	clock_t end1 = clock();
	printf("\n%lfms\n\n", (double)(end1 - begin1));
	part2();
	clock_t end2 = clock();
	printf("\n%lfms", (double)(end2 - end1));

}

void part1()
{
	size_t answer = 0, j = 1;

	type_deck();
	bubblesort_custom2(data.decktype, data.rank, amountdecks);

	for(size_t i = 0; i < amountdecks; i++)
	{
		if(data.rank[i] != 0)
		{
			answer = answer + j * data.rank[i];

			j++;
		}
	}

	printf("Part 1: %llu", answer);
}

void part2()
{
	size_t answer = 0, j = 1;

	memset(data.decktype, 0, sizeof(data.decktype));

	type_deck2();
	bubblesort_custom2(data.decktype, data.rank2, amountdecks);

	for(size_t i = 0; i < amountdecks; i++)
	{
		if(data.rank2[i] != 0)
		{
			answer = answer + j * data.rank2[i];

			j++;
		}
	}

	printf("Part 2: %llu", answer);
}

void fill_data()
{
	size_t j = 0;
	char game[4096];
	char tmp[4096];
	char **tokens;

	for(j = 0; j < file.lengthfile; j++)
	{
		strcpy(game, file.file[j]);
		tokens = str_split(game, ' ', false);
		strncpy(data.deck[j], *tokens, decksize);
		data.rank[j] = str_to_llu(*(tokens+1));
		data.rank2[j] = str_to_llu(*(tokens+1));
	}
}

void type_deck()
{
	size_t deckdata[13] = { 0 };
	size_t str[decksize] = { 0 };
	for(size_t i = 0; i < file.lengthfile; i ++)
	{
		memset(deckdata, 0, sizeof(deckdata));
		for(size_t j = 0; j < decksize; j++)
		{
			switch(data.deck[i][j])
			{
				case 'A':
					deckdata[0]++; break;
				case 'K':
					deckdata[1]++; break;
				case 'Q':
					deckdata[2]++; break;
				case 'J':
					deckdata[3]++; break;
				case 'T':
					deckdata[4]++; break;
				case '9':
					deckdata[5]++; break;
				case '8':
					deckdata[6]++; break;
				case '7':
					deckdata[7]++; break;
				case '6':
					deckdata[8]++; break;
				case '5':
					deckdata[9]++; break;
				case '4':
					deckdata[10]++; break;
				case '3':
					deckdata[11]++; break;
				case '2':
					deckdata[12]++; break;
				default:
					break;
			}
		}
		bubblesort_custom(deckdata, 13);
		if(deckdata[0])
		switch (deckdata[0])
		{
			case 5:
				data.decktype[i] = Five_of_a_kind; break;
			case 4:
				data.decktype[i] = Four_of_a_kind; break;
			case 3:
				if(deckdata[1] == 2)
					data.decktype[i] = Full_house;
				else
					data.decktype[i] = Three_of_a_kind;
				break;
			case 2:
				if(deckdata[1] == 2)
					data.decktype[i] = Two_pair;
				else
					data.decktype[i] = One_pair;
				break;
			default:
				data.decktype[i] = High_card; break;	
		}
		
		for(size_t k = 5; k > 0; k--)
		{
			int tmp = 5 - k;
			switch(data.deck[i][tmp])
			{
				case 'A':
					data.decktype[i] = data.decktype[i] + 13 * pow(100, k) ; break;
				case 'K':
					data.decktype[i] = data.decktype[i] + 12 * pow(100, k); break;
				case 'Q':
					data.decktype[i] = data.decktype[i] + 11 * pow(100, k); break;
				case 'J':
					data.decktype[i] = data.decktype[i] + 10 * pow(100, k); break;
				case 'T':
					data.decktype[i] = data.decktype[i] + 9 * pow(100, k); break;
				case '9':
					data.decktype[i] = data.decktype[i] + 8 * pow(100, k); break;
				case '8':
					data.decktype[i] = data.decktype[i] + 7 * pow(100, k); break;
				case '7':
					data.decktype[i] = data.decktype[i] + 6 * pow(100, k); break;
				case '6':
					data.decktype[i] = data.decktype[i] + 5 * pow(100, k); break;
				case '5':
					data.decktype[i] = data.decktype[i] + 4 * pow(100, k); break;
				case '4':
					data.decktype[i] = data.decktype[i] + 3 * pow(100, k); break;
				case '3':
					data.decktype[i] = data.decktype[i] + 2 * pow(100, k); break;
				case '2':
					data.decktype[i] = data.decktype[i] + 1 * pow(100, k); break;
				default:
					break;
			}
		}
	}
}

void type_deck2()
{
	size_t deckdata[13] = { 0 };
	size_t str[decksize] = { 0 };
	for(size_t i = 0; i < file.lengthfile; i ++)
	{
		memset(deckdata, 0, sizeof(deckdata));
		for(size_t j = 0; j < decksize; j++)
		{
			switch(data.deck[i][j])
			{
				case 'A':
					deckdata[0]++; break;
				case 'K':
					deckdata[1]++; break;
				case 'Q':
					deckdata[2]++; break;
				case 'J':
					deckdata[3]++; break;
				case 'T':
					deckdata[4]++; break;
				case '9':
					deckdata[5]++; break;
				case '8':
					deckdata[6]++; break;
				case '7':
					deckdata[7]++; break;
				case '6':
					deckdata[8]++; break;
				case '5':
					deckdata[9]++; break;
				case '4':
					deckdata[10]++; break;
				case '3':
					deckdata[11]++; break;
				case '2':
					deckdata[12]++; break;
				default:
					break;
			}
		}
		size_t tmp2 = deckdata[3];
		deckdata[3] = 0;
		bubblesort_custom(deckdata, 13);
		deckdata[0] = deckdata[0] + tmp2;
		if(deckdata[0])
		switch (deckdata[0])
		{
			case 5:
				data.decktype[i] = Five_of_a_kind; break;
			case 4:
				data.decktype[i] = Four_of_a_kind; break;
			case 3:
				if(deckdata[1] == 2)
					data.decktype[i] = Full_house;
				else
					data.decktype[i] = Three_of_a_kind;
				break;
			case 2:
				if(deckdata[1] == 2)
					data.decktype[i] = Two_pair;
				else
					data.decktype[i] = One_pair;
				break;
			default:
				data.decktype[i] = High_card; break;	
		}
		
		for(size_t k = 5; k > 0; k--)
		{
			int tmp = 5 - k;
			switch(data.deck[i][tmp])
			{
				case 'A':
					data.decktype[i] = data.decktype[i] + 13 * pow(100, k) ; break;
				case 'K':
					data.decktype[i] = data.decktype[i] + 12 * pow(100, k); break;
				case 'Q':
					data.decktype[i] = data.decktype[i] + 11 * pow(100, k); break;
				case 'J':
					data.decktype[i] = data.decktype[i] + 0 * pow(100, k); break;
				case 'T':
					data.decktype[i] = data.decktype[i] + 9 * pow(100, k); break;
				case '9':
					data.decktype[i] = data.decktype[i] + 8 * pow(100, k); break;
				case '8':
					data.decktype[i] = data.decktype[i] + 7 * pow(100, k); break;
				case '7':
					data.decktype[i] = data.decktype[i] + 6 * pow(100, k); break;
				case '6':
					data.decktype[i] = data.decktype[i] + 5 * pow(100, k); break;
				case '5':
					data.decktype[i] = data.decktype[i] + 4 * pow(100, k); break;
				case '4':
					data.decktype[i] = data.decktype[i] + 3 * pow(100, k); break;
				case '3':
					data.decktype[i] = data.decktype[i] + 2 * pow(100, k); break;
				case '2':
					data.decktype[i] = data.decktype[i] + 1 * pow(100, k); break;
				default:
					break;
			}
		}
	}
}




void swap(size_t *xp, size_t *yp)
{
    size_t temp = *xp;
    *xp = *yp;
    *yp = temp;
}


void bubblesort_custom(size_t arr[], int n)
{
    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++)
	{
        swapped = false;
        for (j = 0; j < n - i - 1; j++)
		{
            if (arr[j] < arr[j + 1])
			{
                swap(&arr[j], &arr[j + 1]);
                swapped = true;
            }
        }

        if (swapped == false)
            break;
    }
}

void bubblesort_custom2(size_t arr[], size_t arr2[], int n)
{
    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++)
	{
        swapped = false;
        for (j = 0; j < n - i - 1; j++)
		{
            if (arr[j] > arr[j + 1])
			{
                swap(&arr[j], &arr[j + 1]);
				swap(&arr2[j], &arr2[j + 1]);
                swapped = true;
            }
        }

        if (swapped == false)
            break;
    }
}
