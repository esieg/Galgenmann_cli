# Galgenmann / Hangman
A little exercise in C++ (after too long a period of abstinence) for a loved one who is currently ill but loves to play hangman.

## Compile
I use simple g++ for compiling
> g++ -std=c++17 -o Galgenmann Galgenmann.cpp

## Start the game
Use this in the folder containing the Galgenmann-Binary (tests on a Mac only)
> ./Galgenmann
**words.txt has to be next to the binary!**

## Gallows ASCII-Art
Selfdrawn, has reduced that gradually and one lined in the ``draw_gallows``-Method
```
 ||========
 || /   |
 ||/    O
 ||    /|\
 ||    / \
 ||
============
```