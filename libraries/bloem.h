//6.0

#ifndef BLOEM_H
#define BLOEM_H

#ifndef __GNUC__
	#warning "I haven't tested for this compiler"
#endif /* __GNUC__ */

#include <stdbool.h>

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


struct filecontent
{
	char **file;
	size_t amountlines;
	size_t *lengthlines;
};

struct filecontent file;


extern struct filecontent readfile(const char *filename);
extern void fix_file(char *argv[], const char *whichfile);
extern char *make_file_name(char *argv[]);
extern void make_file(char *argv[], char filename[]);
extern void make_debug_file(char *argv[], char **string, char *filename);
extern void make_directory(const char *name);
extern char *fix_path_until_now(char *argv[]);




#endif //BLOEM_H