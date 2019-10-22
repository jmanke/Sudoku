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


const std::string SUDOKU_DIR = "sudokus/";


std::vector<std::string> read_directory(const std::string& name)
{
	std::vector<std::string> files;
	std::string pattern(name);
	pattern.append("\\*");
	WIN32_FIND_DATA data;
	HANDLE hFind;
	if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
		do {
			files.emplace_back(data.cFileName);
		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}

	return files;
}


std::vector<std::string> getFilesWithExtension(const std::string &dir, const std::string &ext) {
	auto files = read_directory(dir);
	std::string pattern = "(.*)([.]" + ext + ")";

	files.erase(std::remove_if(
		files.begin(),
		files.end(),
		[&pattern](std::string file) { return !std::regex_match(file, std::regex(pattern)); }),
		files.end());

	return files;
}


std::vector<std::string> getSudokuFiles(int argc, char **args) {
	std::vector<std::string> sudokuFiles;
	if (argc == 1) {
		sudokuFiles = getFilesWithExtension(SUDOKU_DIR, "txt");
		std::transform(sudokuFiles.begin(), sudokuFiles.end(), sudokuFiles.begin(), [](std::string file) { return SUDOKU_DIR + file; });
	}
	else if (argc == 2 || argc == 3) {
		sudokuFiles = { args[1] };
	} 
	else {
		std::cout << "Invalid arguments" << std::endl;
	}

	return sudokuFiles;
}


void printSolution(const sudoku &sudokuSolver, const std::string &file, int argc, char **args) {
	if (argc == 1 || argc == 2) {
		std::cout << "Solution for " << file << std::endl;
		sudokuSolver.printSolution(std::cout);
		std::cout << std::endl;
	}
	else {
		std::string outFile = args[2];
		std::ofstream solFile;
		solFile.open(outFile);

		if (!solFile) {
			std::cerr << "Cannot open output file: " << outFile << std::endl;
			std::cerr << strerror(errno) << std::endl;
			return;
		}

		sudokuSolver.printSolution(solFile);
		solFile.close();
	}
}


int main(int argc, char **args) {
	auto sudokuFiles = getSudokuFiles(argc, args);

	for (const auto& sudokuFile : sudokuFiles) {
		sudoku sudokuSolver(sudokuFile);
		sudokuSolver.solve();
		printSolution(sudokuSolver, sudokuFile, argc, args);
	}

	return 0;
}