#include <iostream>
#include <cassert>
#include <cstring>
#include <memory>
#include <filesystem>
#include "bloem.h"

void filecontent::read_file(const char* filename_)
{
	if(has_file)
		reset_filecontent();
	FILE* file_ptr;
	// struct filecontent read_file;
	filename = filename_;
	file_ptr = fopen(filename.string().c_str(), "r");
	if(file_ptr == NULL)
	{
		perror(filename.string().c_str());
		assert(false);
	}
	char str[FILE_READ_AMOUNT];
	char buffer[FILE_READ_AMOUNT];
	char* tmp;
	amount_lines = 1;
	size_t current_buffer_count = 0, max_buffer_count = 0;
	size_t current_line = 0;

	while(true)
	{
		size_t res = fread(buffer, 1, FILE_READ_AMOUNT, file_ptr);
		if(ferror(file_ptr))
		{
			perror(filename.string().c_str());
			assert(false);
		}
		for(size_t i = 0; i < res; i++)
			if(buffer[i] == '\n')
				amount_lines++;
		
		if(feof(file_ptr))
			break;
	}

	rewind(file_ptr);
	file = (char**)calloc(amount_lines + 1, sizeof(char*));
	length_lines = (size_t*)calloc(amount_lines, sizeof(size_t));
	tmp = (char*)calloc(FILE_READ_AMOUNT, sizeof(char));

	if((file == NULL) || (length_lines == NULL) || (tmp == NULL))
	{
		perror("");
		assert(false);
	}

	while(fgets(str, FILE_READ_AMOUNT, file_ptr) != NULL)
	{
		if(ferror(file_ptr))
		{
			perror(filename.string().c_str());
			assert(false);
		}
		if((str[strlen(str) - 1]) != '\n')
		{
			if(current_buffer_count > max_buffer_count)
			{
				tmp = (char*)realloc(tmp, FILE_READ_AMOUNT * (current_buffer_count + 1) * sizeof(char));
				if(tmp == NULL)
				{
					perror("");
					assert(false);
				}
			}
			current_buffer_count++;
			strcat(tmp, str);
		}
		else
		{
			str[strlen(str) - 1] = 0;
			max_buffer_count = __max(max_buffer_count, current_buffer_count);
			strcat(tmp, str);
			*(file + current_line) = (char*)malloc((strlen(tmp) + 1) * sizeof(char));
			if(*(file + current_line) == NULL)
			{
				perror("");
				assert(false);
			}
			length_lines[current_line] = strlen(tmp);
			strcpy(*(file + current_line), tmp);
			memset(tmp, 0, FILE_READ_AMOUNT * (max_buffer_count + 1) * sizeof(char));
			current_buffer_count = 0;
			current_line++;
		}	
	}
	*(file + current_line) = (char*)malloc((strlen(tmp) + 1) * sizeof(char));
	if(*(file + current_line) == NULL)
	{
		perror("");
		assert(false);
	}
	length_lines[current_line] = strlen(tmp);
	strcpy(*(file + current_line), tmp);
	has_file = true;
}

void filecontent::fix_file(const char* whichfile)
{	
	char filenametest1[FILENAME_MAX];
	char filenametest2[FILENAME_MAX];
	char filenamemain[FILENAME_MAX];
	char path_until_now[FILENAME_MAX];
	char filename_[FILENAME_MAX];
	char directory[FILENAME_MAX];

	strcpy(path_until_now, filename.parent_path().string().c_str());
	{
		char tmp[2] = { (char) PATH_SEPARATOR, '\0' };
		strcat(path_until_now, tmp);
	}
	#if defined(WIN32) || defined(_WIN32) 
		strncpy(filename_, filename.filename().string().c_str(), strlen(filename.filename().string().c_str()) - 4);
	#else
		strcpy(filename_, filename.filename().string().c_str());
	#endif
	
	sprintf(directory, "%stxt", path_until_now);
	make_directory(directory);

	if(strlen(path_until_now) + strlen(filename_) + strlen("txt.testx.txt") >= FILENAME_MAX)
	{
		fprintf(stderr, "%s", filename_);
		assert(false);
	}
	sprintf(filenametest1, "%stxt%c%s.test1.txt", path_until_now, PATH_SEPARATOR, filename_);
	sprintf(filenametest2, "%stxt%c%s.test2.txt", path_until_now, PATH_SEPARATOR, filename_);
	sprintf(filenamemain, "%stxt%c%s.txt", path_until_now, PATH_SEPARATOR, filename_);
	
	make_file(filenametest1);
	make_file(filenametest2);
	make_file(filenamemain);

	if(!strcmp(whichfile, "T1"))
	{
		printf("\nReading form \"%s\"\n", filenametest1);
		read_file(filenametest1);
	}
	else if(!strcmp(whichfile, "T2"))
	{
		printf("\nReading form \"%s\"\n", filenametest2);
		read_file(filenametest2);
	}
	else if(!strcmp(whichfile, "M"))
	{
		printf("\nReading form \"%s\"\n", filenamemain);
		read_file(filenamemain);
	}
	else
	{
		fprintf(stderr, "Not a valid input\n");
		assert(false);
	}
}


void filecontent::make_file(char* filename_)
{
	FILE* file_ptr;
	
	file_ptr = fopen(filename_, "r");
	if(file_ptr == NULL)
	{
		file_ptr = fopen(filename_, "w");
		if(file_ptr != NULL)
		{
			printf("Made file \"%s\"\n", filename_);
		}
		else
		{
			printf("Can not make file \"%s\"\n", filename_);
		}
	}
	fclose(file_ptr);
}

void filecontent::make_debug_file(char** string, char* filename_)
{
	FILE* file_ptr;
	unsigned long long i = 0;
	char filename_debug[FILENAME_MAX];
	char path_until_now[FILENAME_MAX];
	strcpy(path_until_now, filename.parent_path().string().c_str());
	strcat(path_until_now, "\\");
	char debug_dir[FILENAME_MAX];
	sprintf(debug_dir, "%sdebug", path_until_now);
	make_directory(debug_dir);
	sprintf(filename_debug, "%s%c%s.txt", debug_dir, PATH_SEPARATOR, filename_);
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

void filecontent::make_directory(const char* name)
{
	if(std::filesystem::create_directory(name))
	{
		printf("\nMade directory \"%s\"\n", name);
	}
}

filecontent::filecontent(void)
{
#if defined(WIN32) || defined(_WIN32) 
    filename.replace_filename(*__argv);
#endif
}

void filecontent::give_argv(char** _argv)
{
	filename.replace_filename(*_argv);
}

filecontent::~filecontent(void)
{
	reset_filecontent();
}

void filecontent::reset_filecontent(void)
{
	if(has_file)
	{
		free(length_lines);
		for(size_t i = 0; i < amount_lines; i++)
			free(file[i]);
		free(file);
		amount_lines = 0;
		has_file = false;
	}
}

