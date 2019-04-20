#include <iostream>
#include <map>
#include <fstream>
#include "main.h"

const int DIM = 3;
const int BLOCK_COUNT = DIM * DIM;
const int CELL_COUNT = DIM * DIM * DIM * DIM;

struct block{
    cell *cells[BLOCK_COUNT];
};

struct column{
    cell *cells[BLOCK_COUNT];
};

struct row{
    cell *cells[BLOCK_COUNT];
};

struct cell{
    int val = 0;
    block block;
    column col;
    row row;
};

struct board{
    cell cells[CELL_COUNT];
    block blocks[BLOCK_COUNT];
    row rows[BLOCK_COUNT];
    column cols[BLOCK_COUNT];
};

// Verifies a block's solution
bool verifySequence(cell *cells){
    std::cout << std::endl << std::endl;
    std::map<int, int> numberMap;
    numberMap[0] = 1;
    int mapIndex;

    for (int i = 0; i < BLOCK_COUNT; i++){
        mapIndex = cells[i].val;
        numberMap[mapIndex]++;
        std::cout << mapIndex << std::endl;

        if (numberMap[mapIndex] > 1)
            return false;
    }

    return true;
}

bool verifySolution(board board){
    for (block blk : board.blocks){
        if (!verifySequence(*blk.cells))
            return false;
    }
//    for (row row : board.rows){
//        if (!verifySequence(*row.cells))
//            return false;
//    }
//    for (column col : board.cols){
//        if (!verifySequence(*col.cells))
//            return false;
//    }

    return true;
}

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
    for (int i = 0; inFile >> n; i++) {
        board.cells[i].val = n;
    }

    // Fill , columns and blocks
    for (int i = 0, row = 0; row < BLOCK_COUNT; row++){
        for (int j = 0; j < BLOCK_COUNT; j++, i++){
            int blockRow = row / DIM;
            int blockIndex = j / DIM + blockRow * DIM;
            int cellIndex = (row % DIM) * DIM + j % DIM;
            board.blocks[blockIndex].cells[cellIndex] = &board.cells[i];
            board.cells[i].block = board.blocks[blockIndex];
            board.rows[row].cells[j] = &board.cells[i];
            board.cells[i].row = board.rows[row];
            board.cols[j].cells[row] = &board.cells[i];
            board.cells[i].col = board.cols[j];
        }
    }

    return board;
}

int main() {

    std::string sudokuPath = "sudokus/solvedTest.txt";
    board board = generateBoard(sudokuPath);
    int runningTime = 0;

    for (row row : board.rows){
        std::cout << std::endl;
        for (int i = 0; i < BLOCK_COUNT; i++){
            std::cout << row.cells[i]->val << "  ";
        }
    }

    std::cout << "Board is solved: " << verifySolution(board) << std::endl;

//
//    while (!verifySolution(board)){
//
//        runningTime++;
//    }

    return 0;
}