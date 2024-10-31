#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define arraysize 128

struct filecontent
{
    char **file;
    size_t lengthfile, maxlengthfile;
};
struct mapdata
{
    size_t seeds[arraysize];
    size_t seed_[arraysize];
    size_t _soil[arraysize];
    size_t soil_[arraysize];
    size_t _fertilizer[arraysize];
    size_t fertilizer_[arraysize];
    size_t _water[arraysize];
    size_t water_[arraysize];
    size_t _light[arraysize];
    size_t light_[arraysize];
    size_t _temperature[arraysize];
    size_t temperature_[arraysize];
    size_t _humidity[arraysize];
    size_t humidity_[arraysize];
    size_t _location[arraysize];
};
struct number
{
    size_t number, size, place;
    char *str;
    size_t *numberstring;
};

void part1();
void part2();

size_t strtoarray();
struct number strtoint();

char *replace(const char *vstring);
char** str_split(char* a_str, const char a_delim);
char *searchAndReplace(char *text, char *search, char *replace);
struct filecontent read(const char *files);
size_t max();

int main(void)
{
    const char *filename = "txt/5.test1.txt";
    struct filecontent main = read(filename);
    part1(main);
    part2(main);
}

void part1(struct filecontent part1)
{

}

void part2(struct filecontent part2)
{
    
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
    while(i < arraysize)
    {
        strtoarray.numberstring[i] = 0;
        i++;
    }
    return *strtoarray.numberstring;
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