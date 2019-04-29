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

int main() {
    std::vector<std::string> files;
    std::string dir = "sudokus/";
    read_directory(dir, files);

    for (const std::string &file : files){
        if (std::regex_match (file, std::regex("(.*)([.]txt)"))){
            std::cout << std::endl << file << ": ";
            std::string path = dir + file;
            std::vector<int> sol;
            std::string solStr;

            if (sudoku::solve(path, sol)){
                std::cout << "Solution found: " << std::endl << std::endl;
                int dim = (int)std::sqrt(sol.size());

                for (int i = 0; i < dim; i++){
                    for (int j = 0; j < dim; j++){
                        solStr.append(std::to_string(sol[i * dim + j])).append(" ");
                    }

                    solStr.append("\n");
                }

                std::cout << solStr << std::endl;
            }
        }
    }

    return 0;
}