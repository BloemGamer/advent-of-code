# Advent of code library

A library containing some basic functions I use to solve the advent of code problems. This library should work on both windows and linux.

## Global variables

-   `directons`; This is a struct to be able to easy cycle through 4 or 8 sides of a place.

## Functions

### General functions

-   `run_parts()`; This function runs `part1()` and `part2()`, and times them, u need to give this function a clock_t of the start of the code.
-   `str_to_llu()`; This function reads a string, and returns a `long long unsigned`.
-   `str_ll()`; This function reads a string, and returns a `long long`.
-   `str_split()`; This function splits the string you give it, it splits the string on a character you chose, and returns a 2D string back.
> [!WARNING]
> This function changes the original string, so if you still need the original, copy the string first

-   `searchAndReplace()`; This function replaces part of a string with another string.
> [!WARNING]
> Be aware, make sure the string you return this function to is big enough for the new string

-   `part1()` & `part2()`; These definitions are here so I don't have to define these functions every time.