#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H

#include <vector>

int binarySearch( const std::vector<int> &A, int &x ) {
    int low = 0;
    int high = A.size() - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if      (A[mid] < x) { low = mid + 1;  }
        else if (A[mid] > x) { high = mid - 1; }
        else                 { return mid;     }
    }
    return -1;
}

#endif
