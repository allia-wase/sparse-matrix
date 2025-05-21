#include "SparseMatrix.h"
#include <fstream>
#include <cstring>
#include <cctype>
#include <stdexcept>

Node::Node(int r, int c, int v) : row(r), col(c), value(v), next(nullptr) {}

RowList::RowList() : head(nullptr) {}

SparseMatrix::SparseMatrix(int rows, int cols) : numRows(rows), numCols(cols) {
    rows = std::max(0, rows);
    this->rows = new RowList[rows];
}

SparseMatrix::SparseMatrix(int rows, int cols, const char* filePath) : numRows(rows), numCols(cols) {
    this->rows = new RowList[rows];
    // Load from file logic would go here if needed
}

SparseMatrix::SparseMatrix(const char* filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::invalid_argument("File not found");
    }

    char line[256];

    // Read rows
    if (!file.getline(line, sizeof(line))) throw std::invalid_argument("Invalid file format");
    if (strncmp(line, "rows=", 5) != 0) throw std::invalid_argument("Invalid file format");
    numRows = SparseMatrix::parseInt(line + 5);

    // Read cols
    if (!file.getline(line, sizeof(line))) throw std::invalid_argument("Invalid file format");
    if (strncmp(line, "cols=", 5) != 0) throw std::invalid_argument("Invalid file format");
    numCols = SparseMatrix::parseInt(line + 5);

    rows = new RowList[numRows];

    while (file.getline(line, sizeof(line))) {
        int r, c, v;
        if (parseTuple(line, r, c, v)) {
            setElement(r, c, v);
        }
    }
}

int SparseMatrix::getRows() const {
    return numRows;
}

int SparseMatrix::getCols() const {
    return numCols;
}

int SparseMatrix::getElement(int row, int col) const {
    if (row < 0 || row >= numRows || col < 0 || col >= numCols) {
        throw std::out_of_range("Index out of bounds");
    }

    Node* current = rows[row].head;
    while (current) {
        if (current->col == col) {
            return current->value;
        }
        current = current->next;
    }
    return 0; // Default zero
}

void SparseMatrix::setElement(int row, int col, int value) {
    if (row < 0 || row >= numRows || col < 0 || col >= numCols) {
        throw std::out_of_range("Index out of bounds");
    }

    if (value == 0) {
        // Remove existing node if it exists
        Node* prev = nullptr;
        Node* current = rows[row].head;
        while (current) {
            if (current->col == col) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    rows[row].head = current->next;
                }
                delete current;
                return;
            }
            prev = current;
            current = current->next;
        }
        return;
    }

    // Insert or update
    Node* prev = nullptr;
    Node* current = rows[row].head;
    while (current && current->col < col) {
        prev = current;
        current = current->next;
    }

    if (current && current->col == col) {
        current->value = value; // Update
    } else {
        Node* newNode = new Node(row, col, value);
        if (prev) {
            prev->next = newNode;
            newNode->next = current;
        } else {
            rows[row].head = newNode;
            newNode->next = current;
        }
    }
}

SparseMatrix SparseMatrix::add(const SparseMatrix& other) const {
    if (numRows != other.numRows || numCols != other.numCols) {
        throw std::invalid_argument("Matrices dimensions must match");
    }

    SparseMatrix result(numRows, numCols);

    for (int r = 0; r < numRows; ++r) {
        Node* curr1 = rows[r].head;
        Node* curr2 = other.rows[r].head;

        while (curr1) {
            result.setElement(r, curr1->col, curr1->value + other.getElement(r, curr1->col));
            curr1 = curr1->next;
        }

        while (curr2) {
            int val1 = getElement(r, curr2->col);
            result.setElement(r, curr2->col, val1 + curr2->value);
            curr2 = curr2->next;
        }
    }

    return result;
}

SparseMatrix SparseMatrix::subtract(const SparseMatrix& other) const {
    if (numRows != other.numRows || numCols != other.numCols) {
        throw std::invalid_argument("Matrices dimensions must match");
    }

    SparseMatrix result(numRows, numCols);

    for (int r = 0; r < numRows; ++r) {
        Node* curr1 = rows[r].head;
        Node* curr2 = other.rows[r].head;

        while (curr1) {
            result.setElement(r, curr1->col, curr1->value - other.getElement(r, curr1->col));
            curr1 = curr1->next;
        }

        while (curr2) {
            int val1 = getElement(r, curr2->col);
            result.setElement(r, curr2->col, val1 - curr2->value);
            curr2 = curr2->next;
        }
    }

    return result;
}

SparseMatrix SparseMatrix::multiply(const SparseMatrix& other) const {
    if (numCols != other.numRows) {
        throw std::invalid_argument("Incompatible dimensions for multiplication");
    }

    SparseMatrix result(numRows, other.numCols);

    for (int r = 0; r < numRows; ++r) {
        Node* currA = rows[r].head;
        while (currA) {
            int colA = currA->col;
            Node* currB = other.rows[colA].head;
            while (currB) {
                int resVal = result.getElement(r, currB->col) + currA->value * currB->value;
                result.setElement(r, currB->col, resVal);
                currB = currB->next;
            }
            currA = currA->next;
        }
    }

    return result;
}

void SparseMatrix::saveToFile(const char* filePath) const {
    std::ofstream file(filePath);
    file << "rows=" << numRows << "\n";
    file << "cols=" << numCols << "\n";

    for (int r = 0; r < numRows; ++r) {
        Node* curr = rows[r].head;
        while (curr) {
            file << "(" << r << ", " << curr->col << ", " << curr->value << ")\n";
            curr = curr->next;
        }
    }
}

// Static helper methods

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

int SparseMatrix::parseInt(const char*& ptr) {
    int sign = 1;
    if (*ptr == '-') {
        sign = -1;
        ++ptr;
    }

    int value = 0;
    while (isDigit(*ptr)) {
        value = value * 10 + (*ptr - '0');
        ++ptr;
    }
    return sign * value;
}

void SparseMatrix::skipWhitespace(const char*& ptr) {
    while (*ptr == ' ') ++ptr;
}

bool parseTuple(const char* line, int& row, int& col, int& value) {
    if (line[0] != '(') return false;
    const char* ptr = line + 1;

    // Parse row
    int sign = 1;
    if (*ptr == '-') { sign = -1; ++ptr; }
    int r = 0;
    while (isDigit(*ptr)) { r = r * 10 + (*ptr++ - '0'); }
    row = sign * r;
    if (*ptr++ != ',') return false;

    // Skip space
    while (*ptr == ' ') ++ptr;

    // Parse col
    sign = 1;
    if (*ptr == '-') { sign = -1; ++ptr; }
    int c = 0;
    while (isDigit(*ptr)) { c = c * 10 + (*ptr++ - '0'); }
    col = sign * c;
    if (*ptr++ != ',') return false;

    // Skip space
    while (*ptr == ' ') ++ptr;

    // Parse value
    sign = 1;
    if (*ptr == '-') { sign = -1; ++ptr; }
    int v = 0;
    while (isDigit(*ptr)) { v = v * 10 + (*ptr++ - '0'); }
    value = sign * v;

    while (*ptr == ' ') ++ptr;
    if (*ptr != ')') return false;
    ++ptr;

    while (*ptr == ' ') ++ptr;
    return *ptr == '\0';
}
