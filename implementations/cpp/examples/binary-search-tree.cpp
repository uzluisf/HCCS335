#include "../lib/BinarySearchTree.h"
#include <iostream>
#include <vector>

int main() {
    BinarySearchTree<int> bst{};

    if (bst.isEmpty()) {
        std::cout << "1. BST is empty.\n";
    }

    std::vector<int> items = {6, 2, 8, 1, 4, 3};
    for (auto item : items) {
        bst.insert(item);
    }

    if (!bst.isEmpty()) {
        std::cout << "2. BST isn't empty after inserting some items.\n";
    }

    if (bst.findMin() == 1) {
        std::cout << "3. Found the smallest value in the BST.\n";
    }

    if (bst.findMax() == 8) {
        std::cout << "4. Found the largest value in the BST.\n";
    }

    if (bst.contains(4)) {
        std::cout << "5. BST contains item that was added.\n";
    }

    if (!bst.contains(10)) {
        std::cout << "6. BST doesn't contain item that was never added.\n";
    }

    bst.remove(4);
    if (!bst.contains(4)) {
        std::cout << "7. BST doesn't contain item that was removed added.\n";
    }

    for (auto item : items) {
        if (bst.contains(item))
            bst.remove(item);
    }

    if (bst.isEmpty()) {
        std::cout << "8. BST is empty after removing all its items.\n";
    }

    return 0;
}
