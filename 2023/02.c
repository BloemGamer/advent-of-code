#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
//#include "..\libraries\adventofcode.h"

void part1();
void part2();
int possible();
int possible2();
char** str_split(char* a_str, const char a_delim);
int strtoint();
size_t custom_strlen(const char* str);
char **read(const char *files);
int lenfile(const char *files);
char *searchAndReplace(char *text, char *search, char *replace);
char *verv(const char *vstring);
int max();

int main(void)
{
    clock_t begin = clock();
    part1();
    clock_t end1 = clock();
    printf("\n%lfms\n\n", (double)(end1 - begin));
    part2();
    clock_t end2 = clock();
    printf("\n%lfms", (double)(end2 - end1));
}

void part1(void)
{
    char game[500];
    char game2[500];
    char** tokens;
    char** tokensg;
    int sum = 0;

    FILE *file_ptr;
    file_ptr = fopen("txt/2.txt", "r"); //put here the file name you need to open
    if (NULL == file_ptr) {
        printf("File can't be opened \n");
    }
    else
    {
        printf("\n");
        while (fgets(game, 500, file_ptr) != NULL)
        {
            //printf("%s\n", game);
            strcpy(game2, game);
        
            tokens = str_split(game2, ':');
            //printf("%s\n", *(tokens));
            //printf("%s\n", *(tokens + 1));

            tokensg = str_split(*tokens, ' ');
            //printf("%s\n", *(tokensg + 1));
            if(possible(*(tokens + 1)) == 1)
            {
                sum = sum + atoi(*(tokensg + 1));
            }

        }
    }
    fclose(file_ptr);
    printf("part 1: %d\n", sum);
}

void part2(void)
{
    int sum = 0;
    int j;
    char game[4095];
    char game2[4095];
    const char *filename = "txt/2.txt";
    char **tokens;
    char **tokensg;
    char **file = read(filename);
    /*for(j = 0; j < lenfile(filename); j++)
    {
        printf("%s", file[j]);
    }*/
    for(int i = 0; i < lenfile(filename); i++)
    {
        strcpy(game, file[i]);
        strcpy(game2, game);
        tokens = str_split(game2, ':');
        sum = sum + possible2(*(tokens + 1));
    }
    printf("part 2: %d", sum);
}

int possible(const char *vstring)
{
    int t = 0;
    for(size_t i = 0; i < strlen(vstring); i++)
    {
        switch(vstring[i])
        {
            case '1':
                switch(vstring[i+3])
                {
                    case 'r':
                        if(vstring[i+1] >= '3')
                            t++;
                        break;
                    case 'g':
                        if(vstring[i+1] >= '4')
                            t++;
                        break;
                    case 'b':
                        if(vstring[i+1] >= '5')
                            t++; 
                        break;
                    default:
                        break;     
                }
                break;
            default:
            {
                if(vstring[i] <= '2' && vstring[i] >= '9')
                {
                    switch(vstring[i+2])
                    {
                        case 'r':
                        case 'g':
                        case 'b':
                            break;
                        default:
                            t++; break;
                    }
                }
            }
        }
    }
    //printf("%d\n", t);
    if(t==0)
        return 1;
    else
        return 0;
}

int possible2(const char *vstring)
{
    int prod = 0;
    int r = 0, g = 0, b = 0, tmp;
    char game[4095];
    size_t i;

    strcpy(game, verv(vstring));
    //printf("%s", game);
    for(i = 0; i < strlen(game); i++)
    {
        tmp = strtoint(game, i);
        while(game[i] >= '0' && game[i] <= '9')
        {
            i++;
        }
        switch(game[i])
        {
            case 'r':
                r = max(tmp, r); break;
            case 'g':
                g = max(tmp, g); break;
            case 'b':
                b = max(tmp, b); break;
            default:
                break;
        }
    }
    prod = r * g * b;
    return prod;
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

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
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

int strtoint(char number[], size_t i)
{
    int check = 0;
    size_t len = strlen(number);

    while(i < len)
    {
        if(number[i] >= '0' && number[i] <= '9')
        {
            check = 10 * check + (number[i] - '0');
        }
        else
        {
            return check;
        }
        i++;
    }
    return check;
}

char **read(const char *files)
{
    FILE *file_ptr;
    char str[4096];
    size_t numlines = 0;
    int maxstrlength = 1;
    size_t i = 0;
    char ch;
    
    file_ptr = fopen(files, "r");

    while((ch = fgets(str, 4095, file_ptr) != NULL))
    {
        numlines++;
    }
    rewind(file_ptr);
    const size_t size = numlines*sizeof(char*);
    //printf("Size of the array is %llu x %d\n", numlines, maxstrlength);
    char **output = malloc(size);
    for (i = 0; i < numlines; i++)
    {
        output[i] = (char*)malloc(4096 * sizeof(char));
        *output[i] = 0;
    }
    if (NULL == file_ptr) {
        printf("File can't be opened \n");
    }
    else
    {
        printf("\n");
        i = 0;
    }
    while (fgets(str, 4095, file_ptr) != NULL) {
        strcpy(output[i], str);
        i++;
    }
    fclose(file_ptr);
    return output;
}

int lenfile(const char *files)
{
    FILE *file_ptr;
    int numlines = 0;
    char str[4096];
    char ch;
    
    file_ptr = fopen(files, "r");

    while((ch = fgets(str, 4095, file_ptr) != NULL))
    {
        numlines++;
    }
    fclose(file_ptr);
    return numlines;
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

char *verv(const char *vstring)
{
    char output[4095];
    char *output_p = output;

    strcpy(output, vstring);

    output_p = searchAndReplace(output_p, " ", "");
    output_p = searchAndReplace(output_p, ";", "");
    output_p = searchAndReplace(output_p, ",", "");
    output_p = searchAndReplace(output_p, "blue", "b");
    output_p = searchAndReplace(output_p, "green", "g");
    output_p = searchAndReplace(output_p, "red", "r");

    return output_p;
}

int max(int a, int b)
{
    int tmp;
    if(a > b)
        return a;
    else
        return b;
}