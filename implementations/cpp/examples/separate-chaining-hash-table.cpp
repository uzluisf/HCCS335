#include "../lib/HashTable.h"
#include <iostream>

int main() {
    HashTable<int> h1{};
    HashTable<int> h2{};

    const int NUMS = 400000;
    const int GAP  =   37;
    int i;

    std::cout << "Checking... (no more output means success)" << std::endl;

    for (i = GAP; i != 0; i = ( i + GAP ) % NUMS)
        h1.insert(i);
    
    h2 = h1;
    
    for (i = 1; i < NUMS; i += 2)
        h2.remove(i);

    for (i = 2; i < NUMS; i += 2)
        if (!h2.contains(i))
            std::cout << "Contains fails " << i << std::endl;

    for (i = 1; i < NUMS; i += 2) {
        if (h2.contains(i))
            std::cout << "OOPS!!! " <<  i << std::endl;
    }

    return 0;
}

