# Sudoku
C++ implementation of a Sudoku solver using exact cover. Solves 4x4, 9x9, 16x16, 25x25 sudokus.

# Overview
This program solves sudoku puzzles with deimensions which are perfect squares.  Instead of using a heuristic or rule based algorithm, I opted for a reduction from sudoku to the exact cover problem.  I am an amateur at sudoku, and this approach requires very little knowledge of the game to solve the problem.  

# How it Works
The program is mainly split into two files, sudoku.cpp and exactCover.cpp.  sudoku accepts a file or vector representing a sudoku board, converts it into an input which can be solved by exactCover, and converts the solution from exactCover back into a solved sudoku board (if the original sudoku was solveable).  

# How to Use
- Run Sudoku.exe. The following argsuments can be passed in:
  1. Sudoku.exe
    - No arguments, solves each sudoku .txt file in the sudokus directory.  Prints solutions to standard output.
  2. Sudoku.exe -inputFile 
    - Specifies a sudoku .txt file to solve.  Prints solution to standard output.
  3. Sudoku.exe -inputFile -outputFile
    - Specifies a sudoku .txt file to solve, and prints solution to specified output file.


