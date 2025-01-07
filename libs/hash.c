#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "hash.h"

long long hash_code(long long key)
{
	return key % HASH_SIZE;
}

struct Data_Item *hash_search(long long key)
{
	long long hash_index = hash_code(key);

	while(hash_array[hash_index] != NULL)
	{
		if(hash_array[hash_index]->key == key)
			return hash_array[hash_index];

		hash_index++;

		hash_index %= HASH_SIZE;
	}

	return NULL;
}

void hash_insert(long long key, long long data)
{
	struct Data_Item *item = (struct Data_Item*) malloc(sizeof(struct Data_Item));
	item->data = data;
	item->key = key;

	long long hash_index = hash_code(key);

	while(hash_array[hash_index] != NULL && hash_array[hash_index]->key != -1)
	{
		hash_index++;
		hash_index %= HASH_SIZE;
	}

	hash_array[hash_index] = item;
}

struct Data_Item *hash_delete(struct Data_Item *item)
{
	long long key = item->key;

	long long hash_index = hash_code(key);

	while(hash_array[hash_index] != NULL)
	{
		if(hash_array[hash_index]->key == key)
		{
			struct Data_Item *tmp = hash_array[hash_index];
			hash_array[hash_index] = dummy_item;
			return tmp;
		}
		hash_index++;

		hash_index %= HASH_SIZE;
	}
	return NULL;
}

void hash_display()
{
	long long i ;

	for(i = 0; i < HASH_SIZE; i++)
	{
		if(hash_array[i] != NULL)
			printf(" (%d, %d)", hash_array[i]->key, hash_array[i]->data);
		else
			printf(" ~~ ");
	}
	printf("\n");
}

void hash_fix_dummy_item()
{
	dummy_item = (struct Data_Item*) malloc(sizeof(struct Data_Item));
	dummy_item->data = -1;
	dummy_item->key -1;
}