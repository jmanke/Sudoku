#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cmath>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "sudoku.h"
#include "exactCover.h"

bool sudoku::verifySolution(int board[], int dim){
    int rd = (int)std::sqrt(dim);
    // verify rows and columns
    for (int i = 0; i < dim; i++){
        std::map<int, int> row;
        std::map<int, int> col;
        std::map<int, int> region;
        int regionIndex = (int)(i / rd) * dim * rd + (i % rd) * rd;

        for (int j = 0; j < dim; j++){
            if (++row[board[i * dim + j]] > 1 ||
            ++col[board[i + dim * j]] > 1 ||
            ++region[board[regionIndex + (j / rd) * dim + (j % rd)]] > 1)
                return false;
        }
    }

    return true;
}

std::vector<int> sudoku::getBoard(const std::string *fileName){
    // read file
    std::ifstream inFile;
    inFile.open(*fileName);
    std::vector<int> board;

    if (!inFile){
        std::cout << "Cannot open file" << std::endl;
        std::cerr << strerror(errno) << std::endl;
        exit(1);
    }

    int n;
    for (int i = 0; inFile >> n; i++) {
        board.push_back(n);
    }

    return board;
}

sudoku::ConvertedBoard sudoku::convertToExactCover(std::vector<int> board){
    ConvertedBoard cb;
    int dim = (int)std::sqrt(board.size());
    int regionDim = (int)(std::sqrt(dim));
    int rowCount = 0;
    std::vector<std::vector<bool>> ecBoard;

    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            int cellVal = board[(i * dim) + j];
            int region = (int)(i / regionDim) * regionDim + (j / regionDim);
            int cSize = dim * dim;

            // cell is not set, set all possibilities
            if (cellVal == 0){
                for (int k = 0; k < dim; k++){
                    std::vector<bool> row(4 * dim * dim);
                    Cell cell;
                    cell.col = j;
                    cell.row = i;
                    cell.val = k + 1;

                    //1st constraint, 1 value per cell
                    row[i * dim + j] = true;

                    //2nd constraint, 1 value per row
                    row[cSize + i * dim + k] = true;

                    //3rd constraint, 1 value per column
                    row[cSize * 2 + j * dim + k] = true;

                    //4th constraint, 1 value per region
                    row[cSize * 3 + region * dim + k] = true;

                    cb.solutionMap.insert(std::pair<int, Cell>(rowCount, cell));
                    ecBoard.push_back(row);
                    rowCount++;
                }
            }
            // cell value is known, only add 1 row (only 1 possibility)
            else {
                //std::cout << "Called" << std::endl;
                Cell cell;
                cell.col = j;
                cell.row = i;
                cell.val = cellVal;

                std::vector<bool> row(4 * dim * dim);

                //1st constraint, 1 value per cell
                row[i * dim + j] = true;

                //2nd constraint, 1 value per row
                row[cSize + i * dim + cellVal - 1] = true;

                //3rd constraint, 1 value per column
                row[cSize * 2 + j * dim + cellVal - 1] = true;

                //4th constraint, 1 value per region
                row[cSize * 3 + region * dim + cellVal - 1] = true;

                cb.solutionMap.insert(std::pair<int, Cell>(rowCount, cell));
                ecBoard.push_back(row);
                rowCount++;
            }
        }
    }

    cb.ecBoard = ecBoard;
    return cb;
}

void sudoku::solve(const std::string *fileName) {
    std::vector<int> board = getBoard(fileName);
    ConvertedBoard cb = convertToExactCover(board);
    exactCover ec(cb.ecBoard);

    if (ec.solve()){
        int dim = (int)std::sqrt(board.size());
        int solution[dim * dim];

        for (int i : ec.getSolution()) {
            Cell c = cb.solutionMap[i];
            solution[c.row * dim + c.col] = c.val;
        }

        if (verifySolution(solution, dim)){
            std::cout << "Valid solution!" << std::endl;
        } else {
            std::cout << "Invalid solution" << std::endl;
        }
    }
    else {
        std::cout << "No solution found" << std::endl;
    }
}
