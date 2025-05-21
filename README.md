# Sparse Matrix Operations

This project reads two sparse matrices from text files, performs addition, subtraction, and multiplication, and saves the resulting matrix to an output file.

## Features
- Custom sparse matrix representation without using STL containers
- Supports matrix addition, subtraction, and multiplication
- Validates input format
- Efficient memory usage for large sparse matrices

## How to Run

1. Clone the repository
2. Navigate to `src/`
3. Compile: `g++ main.cpp SparseMatrix.cpp -o matrix`
4. Run: `./matrix`
5. Follow on-screen prompts to select operation and input files

## Input Format Example
rows=2
cols=2
(0, 0, 5)
(1, 1, 10)

## Output

The result is saved in `output/result.txt`.
