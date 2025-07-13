#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>

#define numlines 200

struct filecontent
{
    char **file;
    size_t lengthfile, maxlengthfile;
};
struct numberinfo
{
    size_t winningnumbersarr[numlines];
    size_t mynumbersarr[numlines];
    size_t amountwinningnumberscard[numlines];
    size_t amountcard[numlines];
    char **card;
    char winningnumbers[4096];
    char mynumbers[4096];
};
struct number
{
    size_t number, size, place;
    char str[4096];
    size_t numberstring[numlines];
};

void part1();
void part2();

size_t strtoarray();
struct number strtoint();
size_t amountnumberswin();

char *replace(const char *vstring);
char** str_split(char* a_str, const char a_delim);
char *searchAndReplace(char *text, char *search, char *replace);
struct filecontent read(const char *files);
size_t max();

int main(void)
{
    clock_t begin = clock();
    const char *filename = "txt/4.txt";
    struct filecontent main = read(filename);
    clock_t begin1 = clock();
    printf("\nFile readtime: %lfms\n\n", (double)(begin1 - begin));
    part1(main);
    clock_t end1 = clock();
    printf("\n%lfms\n\n", (double)(end1 - begin1));
    part2(main);
    clock_t end2 = clock();
    printf("\n%lfms", (double)(end2 - end1));
}

void part1(struct filecontent part1)
{

    char game[4095] = "0";
    size_t result = 0;
    size_t amountwin;
    size_t answer = 0;
    struct numberinfo number1;
    char **tokens;
    char **tokensg;
    for(size_t i = 0; i < part1.lengthfile; i++)
    {
        strcpy(game, part1.file[i]);
        strcpy(game, replace(game));
        tokens = str_split(game, ':');
        tokensg = str_split(*(tokens + 1), '|');
        strcpy(number1.winningnumbers, *tokensg);
        strcpy(number1.mynumbers, *(tokensg + 1));
        memcpy((void *)number1.winningnumbersarr, (void *)strtoarray(number1.winningnumbers), numlines * sizeof(size_t));
        memcpy((void *)number1.mynumbersarr, (void *)strtoarray(number1.mynumbers), numlines * sizeof(size_t));
        result = 0;
        amountwin = amountnumberswin(number1);
        if(amountwin != 0)
        {
            result = 1;
            for(size_t j = 1; j < amountwin; j++)
            {
                result = result * 2;
            }
        }
        answer = answer + result;        
    }
    printf("part 1: %llu", answer);
}

void part2(struct filecontent part2)
{
    char game[4095] = "0";  
    size_t amountwin;
    size_t answer = 0;
    size_t j = 0, k = 0;
    struct numberinfo number2;
    char **tokens;
    char **tokensg;
    for(size_t i = 0; i < part2.lengthfile; i++)
    {
        strcpy(game, part2.file[i]);
        strcpy(game, replace(game));
        tokens = str_split(game, ':');
        tokensg = str_split(*(tokens + 1), '|');
        strcpy(number2.winningnumbers, *tokensg);
        strcpy(number2.mynumbers, *(tokensg + 1));
        memcpy((void *)number2.winningnumbersarr, (void *)strtoarray(number2.winningnumbers), numlines * sizeof(size_t));
        memcpy((void *)number2.mynumbersarr, (void *)strtoarray(number2.mynumbers), numlines * sizeof(size_t));
        number2.amountwinningnumberscard[i] = amountnumberswin(number2);
        number2.amountcard[i] = 1;
    }
    while(j < part2.lengthfile)
    {
        k = j + 1;
        while(k <= j + number2.amountwinningnumberscard[j])
        {
            number2.amountcard[k] = number2.amountcard[k] + number2.amountcard[j];
            k++;
        }
        answer = answer + number2.amountcard[j];
        j++;
    }
    printf("part 2: %llu", answer);
}

size_t strtoarray(char *vstring)
{ 
    size_t i = 0;
    struct number strtoarray;

    strcpy(strtoarray.str, vstring);
    strtoarray.place = 1;
    while(strtoarray.place < strlen(strtoarray.str))
    {
        strtoarray = strtoint(strtoarray);
        strtoarray.numberstring[i] = strtoarray.number;
        strtoarray.place = strtoarray.place + strtoarray.size + 1;
        i++;
    }
    while(i < numlines)
    {
        strtoarray.numberstring[i] = 0;
        i++;
    }
    return strtoarray.numberstring;
}

struct number strtoint(struct number strint)
{
    size_t number = 0;
    size_t i = strint.place;
    char numberstr[4096];
    strint.size = 0;
    strcpy(numberstr, strint.str);
    while(true)
    {
        if(numberstr[i] >= '0' && numberstr[i] <= '9')
            {
                number = 10 * number + (numberstr[i] - '0');
            }
            else
            {
                strint.number = number;
                return strint;
            }
        strint.size++;    
        i++;
    }
    return strint;
}

size_t amountnumberswin(struct numberinfo amountwin)
{
    size_t i = 0;
    size_t win = 0;
    while(amountwin.mynumbersarr[i] != 0 && i < numlines)
    {
        for(size_t j = 0; j < numlines; j++)
        {
            if(amountwin.mynumbersarr[i] == amountwin.winningnumbersarr[j])
            {
                win++;
                j = numlines;
            }
            else if(amountwin.winningnumbersarr[j] == 0)
            {
                j = numlines;
            }
        }
        i++;
    }
    return win;
}

char *replace(const char *vstring)
{
    char output[4095];
    char *output_p = output;

    strcpy(output, vstring);

    output_p = searchAndReplace(output_p, "  ", " ");

    return output_p;
}

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    count += last_comma < (a_str + strlen(a_str) - 1);

    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

char *searchAndReplace(char *text, char *search, char *replace)
{
   char buffer[1000];
   char *ptr;
   char *modText = NULL;

   buffer[0] ='\0';
   while ( ptr = strstr(text, search) )
   {
      strncat(buffer, text, ptr-text);
      strcat(buffer, replace);

      text = ptr + strlen(search);
   }
   strcat(buffer, text);

   modText = malloc(strlen(buffer) + 1);
   strcpy(modText, buffer);
   return modText;
}

struct filecontent read(const char *files)
{
    FILE *file_ptr;
    char str[4096] = "0";
    size_t i = 0;
    char ch;
    struct filecontent read;
    read.lengthfile = 0;
    read.maxlengthfile = 1;

    file_ptr = fopen(files, "r");

    while((ch = fgets(str, 4095, file_ptr) != NULL))
    {
        read.lengthfile++;
        read.maxlengthfile = max(read.maxlengthfile, strlen(str));
    }
    rewind(file_ptr);
    const size_t size = read.lengthfile*sizeof(char*);
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
        i = 0;
    }
    while (fgets(str, 4095, file_ptr) != NULL) {
        strcpy(output[i], str);
        i++;
    }
    fclose(file_ptr);
    read.file = output;
    return read;
}

size_t max(size_t a, size_t b)
{
    if(a > b)
        return a;
    else
        return b;
}