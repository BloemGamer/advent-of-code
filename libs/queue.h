//V4.2

#ifndef QUEUE_BLOEM_H
#define QUEUE_BLOEM_H

#include <stdbool.h>


typedef struct Char_Queue
{
    long long unsigned front, back;
    char **queue;
    size_t length, height;
    bool setup;
}Char_Queue_t;
//struct Char_Queue char_queue;

typedef struct long_long_unsigned_Queue
{
    long long unsigned front, back;
    long long unsigned *queue;
    size_t length;
    bool setup;
}long_long_unsigned_Queue_t;
//struct long_long_unsigned_Queue long_long_unsigned_queue;

typedef struct long_long_Queue
{
    long long unsigned front, back;
    size_t length;
    long long *queue;
    bool setup;
}long_long_Queue_t;
//struct long_long_Queue long_long_queue;

void init_c(struct Char_Queue *char_queue, size_t length, size_t height);
void push_front_c(char vstr[], struct Char_Queue *char_queue);
void push_back_c(char vstr[], struct Char_Queue *char_queue);
char *pop_front_c(struct Char_Queue *char_queue);
char *pop_back_c(struct Char_Queue *char_queue);
char *front_c(struct Char_Queue *char_queue);
char *back_c(struct Char_Queue *char_queue);


void init_llu(struct long_long_unsigned_Queue *long_long_unsigned_queue, size_t length);
void push_front_llu(long long unsigned number, struct long_long_unsigned_Queue *long_long_unsigned_queue);
void push_back_llu(long long unsigned number, struct long_long_unsigned_Queue *long_long_unsigned_queue);
long long unsigned pop_front_llu(struct long_long_unsigned_Queue *long_long_unsigned_queue);
long long unsigned pop_back_llu(struct long_long_unsigned_Queue *long_long_unsigned_queue);
long long unsigned front_llu(struct long_long_unsigned_Queue *long_long_unsigned_queue);
long long unsigned back_llu(struct long_long_unsigned_Queue *long_long_unsigned_queue);

void init_ll(struct long_long_Queue *long_long_queue, size_t length);
void push_front_ll(long long unsigned number, struct long_long_Queue *long_long_queue);
void push_back_ll(long long unsigned number, struct long_long_Queue *long_long_queue);
long long pop_front_ll(struct long_long_Queue *long_long_queue);
long long pop_back_ll(struct long_long_Queue *long_long_queue);
long long front_ll(struct long_long_Queue *long_long_queue);
long long back_ll(struct long_long_Queue *long_long_queue);

#endif /* QUEUE_BLOEM_H */
