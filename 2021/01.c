#include <stdio.h>
#include <stdlib.h>

#include "../libs/old/bloem.h"


void part1(void);
void part2(void);

int main (int argc, char **argv)
{
    fix_file(argv, "M");
    part1();
    part2();
}

void part1()
{
    int64_t anwer = 0;
    for(size_t i = 1; i < file.amountlines; i++)
        if(atoi(file.file[i]) > atoi(file.file[i-1]))
           anwer++;
    printf("part 1: %ld\n", anwer);
}


void part2()
{

}
