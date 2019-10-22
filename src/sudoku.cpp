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


sudoku::sudoku(const std::string &file) : sudoku(getBoard(file)) {}


sudoku::sudoku(std::vector<int> &board) : inputBoard(board), dim((int)std::sqrt(board.size())){}


void sudoku::printBoard(const std::vector<int> &board, std::ostream &stream) const {
	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j) {
			stream << board[i * dim + j] << " ";
		}
		stream << std::endl;
	}
}


void sudoku::printSolution(std::ostream &stream) const {
	printBoard(solution, stream);
}


void sudoku::printInputBoard(std::ostream &stream) const {
	printBoard(inputBoard, stream);
}


bool sudoku::solve() {
	ExactCoverBoard cb = convertToExactCover(inputBoard);
	exactCover ec(cb.values);

	if (ec.solve()) {
		solution.clear();
		solution.resize(dim * dim);

		for (const int i : ec.getSolution()) {
			Cell c = cb.solutionMap[i];
			solution[c.row * dim + c.col] = c.val;
		}

		return verifySolution();
	}

	return false;
}


std::vector<int> sudoku::getSolution() const {
	return solution;
}


bool sudoku::verifySolution(const std::vector<int>& solution) {
	if (solution.size() == 0) return false;

	int dim = (int)std::sqrt(solution.size());
	int rd = (int)std::sqrt(dim);

	// verify rows and columns
	for (int i = 0; i < dim; ++i) {
		std::map<int, int> row;
		std::map<int, int> col;
		std::map<int, int> region;
		int regionIndex = (int)(i / rd) * dim * rd + (i % rd) * rd;

		for (int j = 0; j < dim; ++j) {
			if (++row[solution[i * dim + j]] > 1 ||
				++col[solution[i + dim * j]] > 1 ||
				++region[solution[regionIndex + (j / rd) * dim + (j % rd)]] > 1)
				return false;
		}
	}

	return true;
}


std::vector<int> sudoku::getBoard(const std::string &fileName){
	// read file
	std::ifstream inFile;
	inFile.open(fileName);
	std::vector<int> board;

	if (!inFile){
		std::cout << "Cannot open file" << std::endl;
		std::cerr << strerror(errno) << std::endl;
		return board;
	}

	int n;
	for (int i = 0; inFile >> n; ++i) {
		board.push_back(n);
	}

	inFile.close();

	return board;
}


sudoku::ExactCoverBoard sudoku::convertToExactCover(const std::vector<int> &board){
	ExactCoverBoard cb;
	int regionDim = (int)(std::sqrt(dim));
	int rowCount = 0;

	for (int i = 0; i < dim; ++i){
		for (int j = 0; j < dim; ++j){
			int cellVal = board[(i * dim) + j];
			int region = (int)(i / regionDim) * regionDim + (j / regionDim);
			int cSize = dim * dim;

			// cell is not set, set all possibilities
			if (cellVal == 0){
				for (int k = 0; k < dim; ++k){
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

					cb.solutionMap.insert(std::pair<int, Cell>(rowCount++, cell));
					cb.values.push_back(row);
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

				cb.solutionMap.insert(std::pair<int, Cell>(rowCount++, cell));
				cb.values.push_back(row);
			}
		}
	}

	return cb;
}


bool sudoku::verifySolution() {
	return verifySolution(solution);
}


