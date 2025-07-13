#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int read();
int part1();
int part2();
char *verv();
char *searchAndReplace();

int main (void)
{
    clock_t begin = clock();
    part1();
    clock_t end1 = clock();
    printf("\n%lfms\n\n", (double)(end1 - begin));
    part2();
    clock_t end2 = clock();
    printf("\n%lfms", (double)(end2 - end1));
}

int part1(void)
{
    int r1 = 0;
    int z = 0;

    // Defining file pointer and opening the file.
    FILE *file_ptr;
    char str[500];

    // a+ Mode Means that the file is opened in the
    // append + reading mode
    file_ptr = fopen("txt/1.txt", "r"); //put here the file name you need to open

    if (NULL == file_ptr) {
        printf("File can't be opened \n");
    }

      // Reading stinrg using fgets
    printf("part1\n");
    while (fgets(str, 500, file_ptr) != NULL) {
        r1 = r1 + read(str);
        //printf("%d\n", z);
        //printf("%s", str);
        z++;
        //printf("%d %d %d %s", z, r2, read(str), str;
    }

    fclose(file_ptr);
    printf ("%d\n", r1);
}

int part2(void)
{
    int r2 = 0;
    int z = 0;
    // Defining file pointer and opening the file.
    FILE *file_ptr;
    char str[500], str2[500];

    // a+ Mode Means that the file is opened in the
    // append + reading mode
    file_ptr = fopen("txt/1.txt", "r"); //put here the file name you need to open

    rewind(file_ptr);
    if (NULL == file_ptr) {
        printf("File can't be opened \n");
    }

      // Reading stinrg using fgets
    printf("part2\n");
    while (fgets(str, 500, file_ptr) != NULL) {
        r2 = r2 + read(verv(str));
        //z++;
        //printf("%d %d %d %s", z, r2, read(verv(str)), verv(str));
    }

    fclose(file_ptr);
    printf ("%d", r2);
}

int read(const char *vstring)
{
    int a = -1, b, i, t;
    for(i = 0; i < strlen(vstring); i++)
    { 
        t = (int)vstring[i] - '0';       
        if(t >= 0 && t <= 9)
        {
            //printf("%d %c ", t, vstring[i]);
            if(a == -1) //if a is already read, just override b, else override both
            {
                a = t;
                b = t;
            }
            else
            {
                b = t;
            }
        }
    }
    if(a == -1)
    {
        return 0;
    }
    else
    {
        return (10*a + b); //return a als the first number and b as the second
    }
}

char *verv(const char *vstring)
{
    char output[500];
    char *output_p = output;

    strcpy(output, vstring);

    //replace the written numbers with the number
    output_p = searchAndReplace(output_p, "one", "o1e");
    output_p = searchAndReplace(output_p, "two", "t2o");
    output_p = searchAndReplace(output_p, "three", "t3e");
    output_p = searchAndReplace(output_p, "four", "f4r");
    output_p = searchAndReplace(output_p, "five", "f5e");
    output_p = searchAndReplace(output_p, "six", "s6x");
    output_p = searchAndReplace(output_p, "seven", "s7n");
    output_p = searchAndReplace(output_p, "eight", "e8t");
    output_p = searchAndReplace(output_p, "nine", "n9e");

    return output_p;
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

      text = ptr + 1;

   }
   strcat(buffer, text);

   modText = malloc(strlen(buffer) + 1);
   strcpy(modText, buffer);
   return modText;
}