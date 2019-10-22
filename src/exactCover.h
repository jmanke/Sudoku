#ifndef SUDOKU_EXACTCOVER_H
#define SUDOKU_EXACTCOVER_H

class exactCover{
public:
	struct Node {
		Node *up;
		Node *down;
		Node *left;
		Node *right;
		Node *column;
		int nodeCount;
		int colId;
		int rowId;
	};

	std::vector<int> solution;

	exactCover();
	explicit exactCover(const std::vector<std::vector<bool>> &matrix);
	void setMatrix(const std::vector<std::vector<bool>> &matrix);
	bool solve();
	std::vector<int> getSolution();

private:
	std::vector<std::vector<bool>> problemMatrix;
	std::vector<std::vector<exactCover::Node>> nodeMatrix;
	Node *header;
	int rowCount;
	int colCount;
	int leftIndex(int);
	int rightIndex(int);
	int upIndex(int);
	int downIndex(int);
	bool validCell(int, int);
	void cover(const Node&);
	void uncover(const Node&);
	bool solveMatrix();
	Node* getMinCol();
};

#endif //SUDOKU_EXACTCOVER_H
