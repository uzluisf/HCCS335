#include "../lib/BinaryHeap.h"

#include <iostream>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>

// Test program
int main() {
    int minItem = 10000;  // same number of digits
    int maxItem = 99999;

    BinaryHeap<std::string> h;
    std::string str = "hello";
    int i = 37;
    std::string x;

    std::cout << "Begin test... " << std::endl;

    for (i = 37; i != 0; i = ( i + 37 ) % maxItem) {
        std::ostringstream sout;
        sout << "hello" << i;
        if (i >= minItem) {
            std::string s = sout.str( );
            h.insert( s );
        }
    }
    for (i = minItem; i < maxItem; ++i) {
        std::ostringstream sout;
        sout << "hello" << i;
        
        h.deleteMin( x );

        if (x != sout.str())
            std::cout << "Oops! " << i << std::endl;
    }

    std::cout << "End test... no other output is good" << std::endl;
    return 0;
}

