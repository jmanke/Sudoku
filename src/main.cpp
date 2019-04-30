#include <iostream>
#include <map>
#include <fstream>
#include <cmath>
#include <vector>
#include <windows.h>
#include <vector>
#include <string>
#include <regex>
#include "sudoku.h"

void read_directory(const std::string& name, std::vector<std::string>& v)
{
    std::string pattern(name);
    pattern.append("\\*");
    WIN32_FIND_DATA data;
    HANDLE hFind;
    if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
        do {
            v.emplace_back(data.cFileName);
        } while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
}

void writeSolution(const std::string &fileName, const std::string &sol){

}

int main(int argc, char **args) {
    if (argc == 1){
        std::vector<std::string> files;
        std::string dir = "sudokus/";
        read_directory(dir, files);

        for (const std::string &file : files){
            if (std::regex_match (file, std::regex("(.*)([.]txt)"))){
                std::cout << std::endl << file << ": " << std::endl;
                std::string path = dir + file;

                sudoku::solve(path);
            }
        }
    } else if (argc == 2){
        sudoku::solve(args[1]);
    } else if (argc == 3) {
        std::string inFile = args[1];
        std::string outFile = args[2];
        std::vector<int> sol;

        if (sudoku::solve(inFile, sol)){
            std::string solStr;
            int dim = (int)std::sqrt(sol.size());

            for (int i = 0; i < dim; i++){
                for (int j = 0; j < dim; j++){
                    solStr.append(std::to_string(sol[i * dim + j])).append(" ");
                }

                solStr.append("\n");
            }

            std::ofstream solFile;
            solFile.open(outFile);

            if (!solFile){
                std::cerr << "Cannot open output file: " << outFile << std::endl;
                std::cerr << strerror(errno) << std::endl;
                exit(1);
            }

            solFile << solStr;
            solFile.close();
        } else {

        }
    } else {
        std::cerr << "Invalid arguments" << std::endl;
    }

    return 0;
}