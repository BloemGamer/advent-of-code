//6.0

/*
This file has no copyright assigned
But please give credit to the original creater (at the moment the official copy
of this library is located at "https://github.com/BloemGamer/advent-of-code/blob/main/libraries/bloem.h")
Feel free to use this library and change some things for personal use
*/

#ifndef BLOEM_H
#define BLOEM_H

#ifndef __GNUC__
	#warning "I haven't tested for this compiler"
#endif /* __GNUC__ */

#if defined(WIN32) || defined(_WIN32) 
	#define PATH_SEPARATOR '\\' 
#else 
	#define PATH_SEPARATOR '/'
	#define __max(a,b) (((a) > (b)) ? (a) : (b))
	#define __min(a,b) (((a) < (b)) ? (a) : (b))
#endif 

#ifndef FILE_READ_AMOUNT
	#define FILE_READ_AMOUNT 4096
#endif

#ifndef __cplusplus

#include <stdbool.h>
#include <stdlib.h>




struct filecontent
{
	char **file;
	size_t amountlines;
	size_t *lengthlines;
};

extern struct filecontent file;


extern struct filecontent readfile(const char *filename);
extern void fix_file(char *argv[], const char *whichfile);


extern void make_debug_file(char *argv[], char **string, char *filename);
extern void make_directory(const char *name);


#else //__cplusplus

#include <iostream>

class filecontent
{
private:
	bool has_file = false;
	char const* filename;

	char* make_file_name(char *argv[]);
	void make_file(char *argv[], char* filename_);
	char* fix_path_until_now(char *argv[]);
public:
	char** file;
	size_t amount_lines = 0;
	size_t* length_lines;

	void readfile(const char* filename_);
	size_t amountlines();
	size_t lengthlines(size_t line = 0);
	void fix_file(char *argv[], const char *whichfile = "M");
	void make_directory(const char *name);
	void make_debug_file(char *argv[], char** string, char *filename_);

};


#endif // __cplusplus


#endif //BLOEM_H