#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include "exactCover.h"

int exactCover::leftIndex(int i) {return (i == 0) ? colCount - 1 : i - 1;}
int exactCover::rightIndex(int i) {return (i + 1) % colCount;}
int exactCover::upIndex(int i) {return (i == 0) ? rowCount : i - 1;}
int exactCover::downIndex(int i) {return (i + 1) % (rowCount + 1);}

bool exactCover::validCell(int x, int y){
    return (x == 0) || (problemMatrix[x - 1][y]);
}

exactCover::exactCover(){
    header = new Node();
}

exactCover::exactCover(std::vector<std::vector<bool>> matrix) {
    header = new Node();
    setMatrix(matrix);
}

void exactCover::setMatrix(std::vector<std::vector<bool>> matrix) {
    problemMatrix = matrix;
    rowCount = matrix.size();

    if (rowCount > 0)
        colCount = matrix[0].size();
    else
        colCount = 0;

    nodeMatrix.clear();

    // initialize node matrix.  First row is for columns
    for (int i = 0; i < rowCount + 1; i++){
        nodeMatrix.emplace_back(std::vector<Node>(colCount));
    }

    // establish column counts
    for (int i = 0; i < colCount; i++){
        int n = 0;

        for (int j = 0; j < rowCount; j++){
            if (problemMatrix[j][i])
                n++;
        }

        nodeMatrix[0][i].nodeCount = n;
    }

    // link nodes
    for (int i = 0; i < rowCount + 1; i++){
        for (int j = 0; j < colCount; j++){
            if (validCell(i, j)){
                nodeMatrix[i][j].rowId = i - 1;
                nodeMatrix[i][j].colId = j;
                nodeMatrix[i][j].column = &nodeMatrix[0][j];
                int y = j;

                // set right cell
                do{
                    y = rightIndex(y);
                } while(!validCell(i, y));

                if (i > 0){
                    //std::cout << "(" << i - 1 << ", " << j << ")" << " ->: " << i - 1 << ", " << y << std::endl;
                }

                nodeMatrix[i][j].right = &nodeMatrix[i][y];
                y = j;

                // set left cell
                do{
                    y = leftIndex(y);
                } while(!validCell(i, y));

                nodeMatrix[i][j].left = &nodeMatrix[i][y];
                int x = i;

                // set up cell
                do{
                    x = upIndex(x);
                } while(!validCell(x, j));

                nodeMatrix[i][j].up = &nodeMatrix[x][j];
                x = i;

                // set down cell
                do{
                    x = downIndex(x);
                } while(!validCell(x, j));

                nodeMatrix[i][j].down = &nodeMatrix[x][j];
            }
        }
    }

    header->right = &nodeMatrix[0][0];
    header->left = &nodeMatrix[0][colCount - 1];
    nodeMatrix[0][0].left = header;
    nodeMatrix[0][colCount - 1].right = header;
}

std::vector<int> exactCover::getSolution() {
    return solution;
}

void exactCover::cover(Node *node){
    Node *col = node->column;

    col->left->right = col->right;
    col->right->left = col->left;

    for (exactCover::Node *r = col->down; r != col; r = r->down){
        for (exactCover::Node *p = r->right; p != r; p = p->right){
            p->up->down = p->down;
            p->down->up = p->up;
            p->column->nodeCount--;
        }
    }
}

void exactCover::uncover(Node *node){
    Node *col = node->column;

    for (exactCover::Node *r = col->up; r != col; r = r->up){
        for (exactCover::Node *p = r->left; p != r; p = p->left){
            p->up->down = p;
            p->down->up = p;
            p->column->nodeCount++;
        }
    }

    col->left->right = col;
    col->right->left = col;
}

exactCover::Node* exactCover::getMinCol() {
    Node *minCol = header->right;

    for (Node *n = header->right->right; n != header; n = n->right){
        if (minCol->nodeCount > n->nodeCount)
            minCol = n;
    }

    return minCol;
}

bool exactCover::solveMatrix(){
    if (header->right == header){
        return true;
    } else {
        Node *minCol = getMinCol();

        if (minCol->nodeCount == 0)
            return false;

        for (Node *n = minCol->down; n != minCol; n = n->down){
            std::vector<Node*> removeCol = {n};
            exactCover::solution.push_back(n->rowId);

            for (Node *p = n->right; p != n; p = p->right){
                removeCol.push_back(p);
            }

            for (Node *p : removeCol){
                cover(p);
            }

            if (solveMatrix()){
                return true;
            } else {
                for (Node *p : removeCol){
                    uncover(p);
                }
                exactCover::solution.pop_back();
            }
        }

        return false;
    }
}

bool exactCover::solve(){
    if (problemMatrix.empty()){
        std::cout << "No matrix to solve!" << std::endl;
        return false;
    }

    solution.clear();
    return solveMatrix();
}


