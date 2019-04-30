#ifndef SUDOKU_EXACTCOVER_H
#define SUDOKU_EXACTCOVER_H

class exactCover{
public:
    struct Node {
    public:
        Node *up;
        Node *down;
        Node *left;
        Node *right;
        Node *column;
        int nodeCount;
        int colId;
        int rowId;
    };
private:
    std::vector<std::vector<bool>> problemMatrix;
    std::vector<std::vector<exactCover::Node>> nodeMatrix;
    Node *header;
    int rowCount;
    int colCount;

public:
    std::vector<int> solution;

private:
    int leftIndex(int);
    int rightIndex(int);
    int upIndex(int);
    int downIndex(int);
    bool validCell(int, int);
    void cover(Node&);
    void uncover(Node&);
    bool solveMatrix();
    Node* getMinCol();
public:
    exactCover();
    explicit exactCover(std::vector<std::vector<bool>> matrix);
    void setMatrix(std::vector<std::vector<bool>> matrix);
    bool solve();
    std::vector<int> getSolution();
};

#endif //SUDOKU_EXACTCOVER_H
