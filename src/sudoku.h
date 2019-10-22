//
// Created by jeffm on 4/25/2019.
//

#ifndef SUDOKU_SUDOKU_H
#define SUDOKU_SUDOKU_H
class sudoku {
private:
	struct Cell{
		int val;
		int row;
		int col;
	};
	struct ExactCoverBoard{
		std::vector<std::vector<bool>> values;
		std::map<int, Cell> solutionMap;
	};

	std::vector<int> getBoard(const std::string&);
	ExactCoverBoard convertToExactCover(const std::vector<int> &board);
	void printBoard(const std::vector<int> &board, std::ostream &stream) const;
	bool verifySolution();

	int dim;
	std::vector<int> inputBoard;
	std::vector<int> solution;
public:
	sudoku(const std::string &file);
	sudoku(std::vector<int> &board);
	static bool verifySolution(const std::vector<int> &solution);
	bool solve();
	std::vector<int> getSolution() const;
	void printSolution(std::ostream &stream) const;
	void printInputBoard(std::ostream &stream) const;
};


#endif //SUDOKU_SUDOKU_H
