//V6.0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#if defined(WIN32) || defined(_WIN32) 
	#include <direct.h>
#else
	#include <sys/stat.h>
#endif

#include "bloem.h"


struct filecontent readfile(const char *filename)
{
	FILE *file_ptr;
	struct filecontent readfile;
	file_ptr = fopen(filename, "r");
	assert(!(file_ptr == NULL));
	char str[FILE_READ_AMOUNT];
	char buffer[FILE_READ_AMOUNT];
	char *tmp;
	readfile.amountlines = 1;
	size_t current_buffer_count = 0, max_buffer_count = 0;
	size_t current_line = 0;

	while(true)
	{
		size_t res = fread(buffer, 1, FILE_READ_AMOUNT, file_ptr);
		assert(!(ferror(file_ptr)));
		for(size_t i = 0; i < res; i++)
			if(buffer[i] == '\n')
				readfile.amountlines++;
		
		if(feof(file_ptr))
			break;
	}

	rewind(file_ptr);
	readfile.file = (char**)calloc(readfile.amountlines + 1, sizeof(char*));
	readfile.lengthlines = (size_t*)calloc(readfile.amountlines, sizeof(size_t));
	tmp = (char*)calloc(FILE_READ_AMOUNT, sizeof(char));

	assert(!(readfile.file == NULL));
	assert(!(readfile.lengthlines == NULL));
	assert(!(tmp == NULL));

	while(fgets(str, FILE_READ_AMOUNT, file_ptr) != NULL)
	{
		if((str[strlen(str) - 1]) != '\n')
		{
			if(current_buffer_count > max_buffer_count)
			{
				tmp = (char*)realloc(tmp, FILE_READ_AMOUNT * (current_buffer_count + 1) * sizeof(char));
				assert(!(tmp == NULL));
			}
			current_buffer_count++;
			strcat(tmp, str);
		}
		else
		{
			str[strlen(str) - 1] = 0;
			max_buffer_count = __max(max_buffer_count, current_buffer_count);
			strcat(tmp, str);
			*(readfile.file + current_line) = (char*)malloc(strlen(tmp) * sizeof(char));
			assert(!(*(readfile.file + current_line) == NULL));
			readfile.lengthlines[current_line] = strlen(tmp);
			strcpy(*(readfile.file + current_line), tmp);
			memset(tmp, 0, FILE_READ_AMOUNT * (max_buffer_count + 1) * sizeof(char));
			current_buffer_count = 0;
			current_line++;
		}	
	}
	*(readfile.file + current_line) = (char*)malloc(strlen(tmp) * sizeof(char));
	readfile.lengthlines[current_line] = strlen(tmp);
	assert(!(*(readfile.file + current_line) == NULL));
	strcpy(*(readfile.file + current_line), tmp);

	return readfile;
}

void fix_file(char *argv[], const char *whichfile)
{	
	char filenametest1[FIX_FILE_STR_LENGTH];
	char filenametest2[FIX_FILE_STR_LENGTH];
	char filenamemain[FIX_FILE_STR_LENGTH];
	char path_until_now[FIX_FILE_STR_LENGTH];

	strcpy(path_until_now, fix_path_until_now(argv));

	char *filename = make_file_name(argv);
	char directory[FIX_FILE_STR_LENGTH];
	sprintf(directory, "%stxt", path_until_now);
	make_directory(directory);

	sprintf(filenametest1, "%stxt%c%s.test1.txt", path_until_now, PATH_SEPARATOR, filename);
	sprintf(filenametest2, "%stxt%c%s.test2.txt", path_until_now, PATH_SEPARATOR, filename);
	sprintf(filenamemain, "%stxt%c%s.txt", path_until_now, PATH_SEPARATOR, filename);
	
	make_file(argv, filenametest1);
	make_file(argv, filenametest2);
	make_file(argv, filenamemain);

	if(whichfile == "T1")
	{
		printf("\nReading form \"%s\"\n", filenametest1);
		file = readfile(filenametest1);
	}
	else if(whichfile == "T2")
	{
		printf("\nReading form \"%s\"\n", filenametest2);
		file = readfile(filenametest2);
	}
	else if(whichfile == "M")
	{
		printf("\nReading form \"%s\"\n", filenamemain);
		file = readfile(filenamemain);
	}
	else
	{
		assert(false && "Not a valid input");
	}
}


char *make_file_name(char *argv[])
{
	char argvfile[FIX_FILE_STR_LENGTH];
	char *filename_ptr;
	char *filename;
	size_t last_separator;

	assert(!(strlen(argv[0]) > FIX_FILE_STR_LENGTH));

	strcpy(argvfile, argv[0]);
	for(size_t i = 0; i < strlen(*argv); i++)
	{
		if(argv[0][i] == PATH_SEPARATOR)
			last_separator = i;
	}
	filename_ptr = argv[0] + last_separator;
	#if defined(WIN32) || defined(_WIN32) 
	filename = calloc((strlen(filename_ptr) - 3), sizeof(char));
	strncpy(filename, filename_ptr, strlen(filename_ptr) - 4);
	#else
	filename = calloc((strlen(filename_ptr) +1), sizeof(char));
	strncpy(filename, filename_ptr, strlen(filename_ptr));
	#endif



	return filename;
}



void make_file(char *argv[], char filename[])
{
	FILE *file_ptr;
	
	file_ptr = fopen(filename, "r");
	if(file_ptr == NULL)
	{
		file_ptr = fopen(filename, "w");
		if(file_ptr != NULL)
		{
			printf("Made file \"%s\"\n", filename);
		}
		else
		{
			printf("Can not make file \"%s\"\n", filename);
		}
	}
	fclose(file_ptr);
}

void make_debug_file(char *argv[], char **string, char *filename)
{
	FILE *file_ptr;
	unsigned long long i = 0;
	char filename_debug[FIX_FILE_STR_LENGTH];
	char path_until_now[FIX_FILE_STR_LENGTH];
	strcpy(path_until_now, fix_path_until_now(argv));
	char debug_dir[FIX_FILE_STR_LENGTH];
	sprintf(debug_dir, "%sdebug", path_until_now);
	make_directory(debug_dir);
	sprintf(filename_debug, "%s%c%s.txt", debug_dir, PATH_SEPARATOR, filename);
	file_ptr = fopen(filename_debug, "w");
	if(file_ptr != NULL)
	{
		while(*(string + i) != NULL)
		{
			fprintf(file_ptr, "%s\n", *(string + i));
			i++;
		}
		printf("\nMade debug file \"%s\"\n", filename_debug);
	}
	else
	{
		printf("failed to make a debugfile\n");
	}
}

void make_directory(const char *name)
{
	int number;
	#if defined(WIN32) || defined(_WIN32) 
    number = _mkdir(name);
	#else
    number = mkdir(name, 0777);
	#endif
	if(number == 0)
	{
		printf("\nMade directory \"%s\"\n", name);
	}
}

char *fix_path_until_now(char *argv[])
{
	char filename_with_executable[FIX_FILE_STR_LENGTH];
	char *filename = make_file_name(argv);
	char *path_until_now;
	path_until_now = (char*)calloc(FIX_FILE_STR_LENGTH, sizeof(char));

	#if defined(WIN32) || defined(_WIN32)
		sprintf(filename_with_executable, "%s%s", filename, ".exe");
	#else
		sprintf(filename_with_executable, "%s", filename);
	#endif

	size_t last_separator;
	for(size_t i = 0; i < strlen(*argv); i++)
	{
		if(argv[0][i] == PATH_SEPARATOR)
			last_separator = i;
	}
	strncpy(path_until_now, *argv, last_separator + 1);
	return path_until_now;
}


