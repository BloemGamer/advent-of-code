# Bloem

A library containing some basic file functions. This library was specifically made for advent of code, but can be used for other purposes. This library should work on both windows and linux.

## Propeties/Defines

-   `PATH_SEPARATOR`; The line seperator, should allow the code to work on both linux and mac.
For linux there are two defines more, a `__max()` and a `__min()`, these are in the `stdlib.h` of windows, but not on linux, and they're neaded for some functions.
-   `FIX_FILE_STR_LENGTH`; If argv is bigger than 256 this should be changed to a bigger number, so the functions will still work.
-    `FILE_READ_AMOUNT`; The readfile function reads this amount in one time, if you want to read really big files with long lines, this one should be made bigger so the function is faster..

## Structs

-   `filecontent`; Used by the `readfile` function. It contains a 2D string to contain al the info from the file. The struct also contains a `size_t` variable to determine the amount of lines in the file (`amountlines`). And also a `size_t` struct to determine the length of line in the file (`lengthlines`).

## Global variables

-   `filecontent.file`; Global because there are some functions that fill this specific struct. In advent of code you only get one file you would use at once while running, so 1 global struct should be enough. In the event that ain't enought you can always just define another `filecontent` struct.

## Functions

### File functions

-   `readfile()`; This function reads the file you give with the function. It fills the struct with the content of the file, and also fills the `amountlines` and `lengthlines`.
-   `fix_file()`; This function runs the `make_file_names`. After this the functions runs the `make_file` function on the two test filenames and the main testfilename, these file should come in the "txt" folder. After making the filenames it runs the `readfile` on the global struct `filecontent.file`, depending if you give `T1`, `T2` or `M` with the function, the filename it uses is different. This function was specificly made for advent of code but can be modified to be used for anything.
-   `make_file_name()`; This function read the filename from the `argv`, and returns the filename.
-   `make_file()`; This function checkes if a file exists, if not it creates the file.
-   `make_debug_file()`; This function makes a debug file in a folder "debug". This function prints makes a .txt file from a 2D array.
-   `make_directory()`; This function makes a directory, if it succeded it prints the directory it created.



<!-- Thanks you "JTnadrooi". For your help with reviewing and improving this file -->