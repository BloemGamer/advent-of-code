#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>

// 45282386 too low
// 45057825 too low

struct filecontent
{
    char **file;
    size_t lengthfile;
};
struct numberinfo
{
    size_t placex, placey, size, number, number2;
    bool possible;
};

void part1();
void part2();
struct numberinfo readfirstnumber();
struct numberinfo strtoint();
struct numberinfo check();
int min();
struct numberinfo checkstar();
struct numberinfo readstar();
struct numberinfo checkstartnumber();

struct filecontent read(const char *files);

int main(void)
{
    clock_t begin = clock();
    const char *filename = "txt/3.txt";
    struct filecontent main = read(filename);
    clock_t begin1 = clock();
    printf("File readtime: %lfms\n\n", (double)(begin1 - begin));
    part1(main);
    clock_t end1 = clock();
    printf("\n%lfms\n\n", (double)(end1 - begin1));
    part2(main);
    clock_t end2 = clock();
    printf("\n%lfms", (double)(end2 - end1));
}

void part1(struct filecontent part1)
{
    int result = 0;
    char game[4095];
    struct numberinfo numberinfo1;
    
    for(int i = 0; i < part1.lengthfile; i++)
    {
        numberinfo1.placey = i;
        numberinfo1.placex = 0;
        strcpy(game, part1.file[i]);
        //printf("%s", game);
        do{
            numberinfo1.number = 0;
            numberinfo1 = readfirstnumber(numberinfo1, part1);
            if(numberinfo1.possible == true)
            {
                numberinfo1 = strtoint(numberinfo1, part1);
                numberinfo1 = check(numberinfo1, part1);
                numberinfo1.placex = numberinfo1.placex + numberinfo1.size;
            }
            result = result + numberinfo1.number;
        }while(numberinfo1.possible == true);
    }
    printf("part 1: %d", result);
}

void part2(struct filecontent part2)
{
    int result = 0;
    char game[4095];
    struct numberinfo numberinfo2;
    for(int i = 0; i < part2.lengthfile; i++)
    {
        //printf("%llu\n", i + 1);
        numberinfo2.placey = i;
        numberinfo2.placex = 0;
        strcpy(game, part2.file[i]);
        //printf("%s", game);
        do{
            numberinfo2.number = 0;
            numberinfo2 = readstar(numberinfo2, part2);
            if(numberinfo2.possible == true)
            {
                numberinfo2 = checkstar(numberinfo2, part2);
            }
            result = result + numberinfo2.number;
        }while(numberinfo2.possible == true);
    }
    printf("part 2: %d", result);
}

struct numberinfo readfirstnumber(struct numberinfo readnumnum, struct filecontent readnumfile)
{
    size_t j = readnumnum.placey;
    size_t i = readnumnum.placex;
    char game[4095];
    readnumnum.possible = false;
    strcpy(game, readnumfile.file[j]);
    while(i < strlen(game))
    {
        if(game[i] >= '0' && game[i] <= '9')
        {
            readnumnum.possible = true;
            readnumnum.placex = i;
            return readnumnum;
        }
        i++;
    }
    return readnumnum;
}

struct numberinfo strtoint(struct numberinfo strint, struct filecontent strintfile)
{
    strint.size = 0;
    strint.number = 0;
    size_t j = strint.placey;
    size_t i = strint.placex;
    char number[4095];
    strcpy(number, strintfile.file[j]);
    while((strint.size) < 4095)
    {
        if(number[i] >= '0' && number[i] <= '9')
        {
            strint.number = 10 * strint.number + (number[i] - '0');
        }
        else
        {
            return strint;
        }
        strint.size++;
        i++;
    }
    return strint;
}

struct numberinfo check(struct numberinfo checknumber, struct filecontent checkfile)
{
    bool possible = false;
    size_t i, j, k;
    if(checknumber.placex >= 1)
        i = checknumber.placex - 1;
    else 
        i = checknumber.placex;
    if(checknumber.placey >= 1)
        j = checknumber.placey - 1;
    else 
        j = checknumber.placey;
    char game[4095];
    strcpy(game, checkfile.file[j]);    
    while(j <= checknumber.placey + 1)
    {
        k = min(j, checkfile.lengthfile - 1);
        strcpy(game, checkfile.file[k]);
        while(i <= (checknumber.placex + checknumber.size))
        {
            if(game[i] >= '!' && game[i] <= '/' && game[i] != '.' || game[i] >= ':' && game[i] <= '@' || game[i] >= '[' && game[i] <= '`' || game[i] >= '{' && game[i] <= '~')
            {
                possible = true;
            }
            i++;
        }
        if(checknumber.placex >= 1)
            i = checknumber.placex - 1;
        else 
            i = checknumber.placex;
        j++;
    }

    if(possible == false)
    {
        checknumber.number = 0;
    }
    return checknumber;
}

int min(int a, int b)
{
    if(a < b)
        return a;
    else
        return b;
}

struct numberinfo checkstar(struct numberinfo checkstarnumber, struct filecontent checkstarfile)
{
    struct numberinfo checkstarnumber2;
    int possible = 0;
    size_t i, j, k, tmp, x, y;
    x = checkstarnumber.placex;
    y = checkstarnumber.placey;
    checkstarnumber.number = 0;
    checkstarnumber.number2 = 0;
    if(checkstarnumber.placex >= 1)
        i = checkstarnumber.placex - 1;
    else 
        i = checkstarnumber.placex;
    if(checkstarnumber.placey >= 1)
        j = checkstarnumber.placey - 1;
    else 
        j = checkstarnumber.placey;
    char game[4095];
    strcpy(game, checkstarfile.file[j]);    
    while(j <= y + 1)
    {
        if(j == min(j, checkstarfile.lengthfile - 1))
            {
            k = min(j, checkstarfile.lengthfile - 1);
            strcpy(game, checkstarfile.file[k]);
            while(i <= (x + 1))
            {
                if(game[i] >= '0' && game[i] <= '9')
                {
                    possible++;
                    checkstarnumber.placex = i;
                    checkstarnumber.placey = j;
                    checkstarnumber2 = checkstartnumber(checkstarnumber, checkstarfile);
                    checkstarnumber2 = strtoint(checkstarnumber2, checkstarfile);
                    checkstarnumber2.placex = checkstarnumber2.placex + checkstarnumber2.size;
                    i = checkstarnumber2.placex;
                    if(checkstarnumber.number == 0)
                    {
                        checkstarnumber.number = checkstarnumber2.number;
                    }
                    else if(checkstarnumber.number2 == 0)
                    {
                        checkstarnumber.number2 = checkstarnumber2.number;
                    }
                    else
                    {
                        printf("%llu", possible);
                        checkstarnumber.number = 0;
                        checkstarnumber.number2 = 0;
                        checkstarnumber.placey = tmp;
                        return checkstarnumber;
                    }
                    
                }
                i++;
            }
        }
        if(x >= 1)
            i = x - 1;
        else 
            i = x;
        j++;
    }

    if(possible != 2)
    {
        checkstarnumber.number = 0;
        checkstarnumber.number2 = 0;
    }
    checkstarnumber.number = checkstarnumber.number * checkstarnumber.number2;
    checkstarnumber.placex = x + 1;
    checkstarnumber.placey = y;
    return checkstarnumber;
}

struct numberinfo readstar(struct numberinfo readstarnum, struct filecontent readstarfile)
{
    size_t j = readstarnum.placey;
    size_t i = readstarnum.placex;
    char game[4095];
    readstarnum.possible = false;
    strcpy(game, readstarfile.file[j]);
    while(i < strlen(game))
    {
        if(game[i] == '*')
        {
            readstarnum.possible = true;
            readstarnum.placex = i;
            return readstarnum;
        }
        i++;
    }
    return readstarnum;
}

struct numberinfo checkstartnumber(struct numberinfo checkstartnumber, struct filecontent checkstartfile)
{
    checkstartnumber.size = 0;
    size_t j = checkstartnumber.placey;
    size_t i = checkstartnumber.placex;
    char number[4095];
    strcpy(number, checkstartfile.file[j]);
    while((checkstartnumber.size) < 4095)
    {
        if(number[i-1] >= '0' && number[i-1] <= '9')
        {
            i--;
        }
        else
        {
            checkstartnumber.placex = i;
            return checkstartnumber;
        }
    }
    checkstartnumber.placex = i;
    return checkstartnumber;
}




struct filecontent read(const char *files)
{
    FILE *file_ptr;
    char str[4096] = "0";
    size_t maxstrlength = 1;
    size_t i = 0;
    char ch;
    struct filecontent read;
    read.lengthfile = 0;

    file_ptr = fopen(files, "r");

    while((ch = fgets(str, 4095, file_ptr) != NULL))
    {
        read.lengthfile++;
    }
    rewind(file_ptr);
    const size_t size = read.lengthfile*sizeof(char*);
    //printf("Size of the array is %llu x %d\n", numlines, maxstrlength);
    char **output = malloc(size);
    for (i = 0; i < read.lengthfile; i++)
    {
        output[i] = (char*)malloc(4096 * sizeof(char));
        *output[i] = 0;
    }
    if (NULL == file_ptr) {
        printf("File can't be opened \n");
    }
    else
    {
        //printf("File open worked\n");
        i = 0;
    }
    while (fgets(str, 4095, file_ptr) != NULL)
    {
        strcpy(output[i], str);
        //check op "/n" als einde, haal de /n weg
        i++;
    }
    fclose(file_ptr);
    read.file = output;
    return read;
}
