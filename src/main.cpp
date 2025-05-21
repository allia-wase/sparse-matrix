#include "SparseMatrix.h"
#include <iostream>

int main() {
    try {
        std::cout << "Enter path for first matrix: ";
        std::string path1;
        std::cin >> path1;

        std::cout << "Enter path for second matrix: ";
        std::string path2;
        std::cin >> path2;

        SparseMatrix A(path1.c_str());
        SparseMatrix B(path2.c_str());

        std::cout << "Choose operation:\n";
        std::cout << "1. Add\n2. Subtract\n3. Multiply\n";
        int choice;
        std::cin >> choice;

        SparseMatrix result(1, 1); // Placeholder

        switch (choice) {
            case 1:
                result = A.add(B);
                break;
            case 2:
                result = A.subtract(B);
                break;
            case 3:
                result = A.multiply(B);
                break;
            default:
                std::cerr << "Invalid choice\n";
                return 1;
        }

        result.saveToFile("output/result.txt");
        std::cout << "Result saved to output/result.txt\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
