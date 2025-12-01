#include "aoc.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef linux
	#error this code does not work on windows
#endif // Linux

typedef struct
{
	char* file;
	size_t length;
} DynArr;

static DynArr read_file_to_str(const char* filename);


FileContent read_file(int year, int day, FileType file_type)
{
	FileContent file_content = {};
	char* filename = 0;
	switch (file_type) {
		case FILE_TYPE_MAIN:
			asprintf(&filename, "./%d/txt/%02d.txt", year, day);
			break;
		case FILE_TYPE_TEST1:
			asprintf(&filename, "./%d/txt/%02d.test1.txt", year, day);
			break;
		case FILE_TYPE_TEST2:
			asprintf(&filename, "./%d/txt/%02d.test2.txt", year, day);
			break;
	}

	DynArr file = read_file_to_str(filename);

	size_t amount_enters = 0;

	for (size_t i = 0; i < file.length; i++)
	{
		if (file.file[i] == '\n')
		{
			amount_enters++;
		}
	}

	file_content.file = calloc(amount_enters + 1, sizeof(*file_content.file));
	assert(file_content.file != nullptr);
	file_content.file[0] = file.file;
	file_content.len = amount_enters;

	size_t j = 1;

	for (size_t i = 0; i < file.length; i++)
	{
		if (file.file[i] == '\n')
		{
			file.file[i] = '\0';
			file_content.file[j++] = file.file + i + 1;
		}
	}

	free((void*)filename);
	return file_content;
}


static DynArr read_file_to_str(const char* filename)
{
	DynArr ret = {};
#ifdef _WIN32
	FILE* file = fopen(filename, "rb");
#else // LINUX
	FILE* file = fopen(filename, "rbe");
#endif
	if (file == NULL)
	{
		return ret;
	}

	(void)fseek(file, 0, SEEK_END);
	long ftell_ret = ftell(file);
	unsigned long length = (unsigned long)ftell_ret;
	(void)fseek(file, 0, SEEK_SET);

	char* buffer = malloc((length + 1) * sizeof(char));
	if (buffer == NULL)
	{
		(void)fclose(file);
		return ret;
	}

	unsigned long read = fread(buffer, 1, length, file);
	if (read != length)
	{
		if (ferror(file))
		{
			perror("Error reading file");
		}
		else if (feof(file))
		{
			(void)fprintf(stderr, "Unexpected end of file\n");
		}
	}
	buffer[length] = '\0';

	(void)fclose(file);

	return (DynArr){.file = buffer, .length = length};
}
