#include <iostream>
#include <fstream>
#include "main.h"

const int BOARD_SIZE = 81;
const int BLOCK_SIZE = 3;
const int ROW_SIZE = 9;

struct block{
    cell *cells[ROW_SIZE];
};

struct column{
    cell *cells[ROW_SIZE];
};

struct row{
    cell *cells[ROW_SIZE];
};

struct cell{
    int val = 0;
    block block;
    column col;
    row row;
};

struct board{
    cell cells[BOARD_SIZE];
    block blocks[BLOCK_SIZE];
    row rows[ROW_SIZE];
    column cols[ROW_SIZE];
};

board generateBoard(std::string fileName){
    // read file
    std::ifstream inFile;
    inFile.open(fileName);
    board board;

    if (!inFile){
        std::cout << "Cannot open file" << std::endl;
        exit(1);
    }

    int n;
    for (int i = 0; inFile >> n; i++){
        board.cells[i].val = n;
    }

    // Fill , columns and blocks
    for (int i = 0, row = 0; row < ROW_SIZE; row++){
        for (int j = 0; j < ROW_SIZE; j++, i++){
            int blockRow = row / BLOCK_SIZE;
            int blockIndex = j / BLOCK_SIZE + blockRow * BLOCK_SIZE;
            int cellIndex = (row % BLOCK_SIZE) * BLOCK_SIZE + j % BLOCK_SIZE;
            board.blocks[blockIndex].cells[cellIndex] = &board.cells[i];
            board.rows[row].cells[j] = &board.cells[i];
            board.cells[i].row = board.rows[row];
            board.cols[j].cells[row] = &board.cells[i];
            board.cells[i].block = board.blocks[blockIndex];
            board.cells[i].col = board.cols[j];
        }
    }

    return board;
}

int main() {
    std::string sudokuPath = "sudokus/s01a.txt";
    board board = generateBoard(sudokuPath);

    for (int i = 0, n = 0; i < BOARD_SIZE; i++, n++){
        if (n > 8){
            std::cout << std::endl;
            n = 0;
        }

        std::cout << board.cells[i].val << "  ";
    }

    // Populate board

    return 0;
}