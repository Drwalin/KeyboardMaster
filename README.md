# KeyboardMaster


# How to compile:
	To compile on Windows:
		g++ KeyboardMaster.cpp -std=c++11 -DWIN -o KM.exe

	To compile on Linux (unchecked) / MacOSX (unchecked):
		g++ KeyboardMaster.cpp -std=c++11 -lncurses -o KM
		Library NCurses is needed.

	To both systems recommended is -m64 compiler flag for
	bigger dictionaries


# How to run:
KM #file# #args#
#file# - file with containing some words, if file not exist
(or file not entered) program then will use default "dictionary.txt"
#args# possibilities:
	.f - fast load dictionary, do not checking words polite-level
	.c - enable random changing capital - lower case letters
	.nc - disable random changing capital - lower case letters
	.l - use time limit for typing
	.nl - do not use time limit for typing
	.elven - use ase dictionary file "elvenNames.txt"


# How to use:
	Rewrite given string without border quotes.
	If given string is equation then type only
	solution rounded to integer number.


