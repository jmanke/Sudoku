//
// Created by jeffm on 4/25/2019.
//

#ifndef SUDOKU_SUDOKU_H
#define SUDOKU_SUDOKU_H
class sudoku {
private:
    struct Cell{
    public:
        int val;
        int row;
        int col;
    };
    struct ConvertedBoard{
        std::vector<std::vector<bool>> ecBoard;
        std::map<int, Cell> solutionMap;
    };

    static std::vector<int> getBoard(const std::string&);
    static ConvertedBoard convertToExactCover(std::vector<int> board);
public:
    static void printBoard(const std::vector<int>&);
    static bool solve(const std::string&);
    static bool solve(const std::vector<int>&);
    static bool solve(const std::vector<int>&, std::vector<int>&);
    static bool solve(const std::string&, std::vector<int>&);
    static bool verifySolution(std::vector<int>&, int);
};


#endif //SUDOKU_SUDOKU_H
