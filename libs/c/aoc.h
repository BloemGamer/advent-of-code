#include <stddef.h>


typedef struct FileContent
{
	char** file;
	size_t* file_len;
	size_t len;
}FileContent;

typedef enum FileType
{
	FILE_TYPE_MAIN,
	FILE_TYPE_TEST1,
	FILE_TYPE_TEST2,
}FileType;


FileContent read_file(int year, int day, FileType file_type);
