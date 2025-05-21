#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <stdexcept>

// Node representing one non-zero element
struct Node {
    int row;
    int col;
    int value;
    Node* next;

    Node(int r, int c, int v);
};

// RowList represents a linked list of nodes in one row
struct RowList {
    Node* head;
    RowList();
};

// SparseMatrix class definition
class SparseMatrix {
private:
    int numRows;
    int numCols;
    RowList* rows; // Array of RowList (we simulate dynamic array)

public:
    // Constructors
    SparseMatrix(int rows, int cols);
    SparseMatrix(const char* filePath);

    // Getters
    int getRows() const;
    int getCols() const;

    // Element access
    int getElement(int row, int col) const;
    void setElement(int row, int col, int value);

    // Operations
    SparseMatrix add(const SparseMatrix& other) const;
    SparseMatrix subtract(const SparseMatrix& other) const;
    SparseMatrix multiply(const SparseMatrix& other) const;

    // Save to file
    void saveToFile(const char* filePath) const;

private:
    // Helper functions
    static int parseInt(const char*& ptr);
    static void skipWhitespace(const char*& ptr);
};

#endif
