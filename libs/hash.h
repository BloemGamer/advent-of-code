//V0.1

#ifndef HASH_BLOEM_H
#define HASH_BLOEM_H

#ifndef HASH_SIZE
	#define HASH_SIZE 100
#endif

struct Data_Item
{
	long long data;
	long long key;
};

struct Data_Item *hash_array[HASH_SIZE];
struct Data_Item *dummy_item;
struct Data_Item *item;

long long hash_code(long long key);
struct Data_Item *hash_search(long long key);
void hash_insert(long long key, long long data);
struct Data_Item *hash_delete(struct Data_Item *item);
void hash_display();
void hash_fix_dummy_item();

#endif