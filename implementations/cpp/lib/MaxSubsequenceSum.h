#ifndef MAXSUBSEQUENCESUM_H
#define MAXSUBSEQUENCESUM_H

#include <iostream>
#include <vector>

namespace MaxSubsequenceSum {
    int v1( const std::vector<int> &a ) {
        int maxSum = 0;
        for (size_t i = 0; i < a.size(); i++) {
            for (size_t j = i; j < a.size(); j++) {
                int currentSum = 0;
                for (size_t k = i; k <= j; k++) {
                    currentSum += a[k]; 
                }
                if (currentSum > maxSum) {
                   maxSum = currentSum; 
                }
            }
        }

        return maxSum;
    }
    
    int v2( const std::vector<int> &a ) {
        int maxSum = 0;
        for (size_t i = 0; i < a.size(); ++i) {
            int currentSum = 0;
            for (size_t j = i; j < a.size(); ++j) {
                currentSum += a[j];
                if (currentSum > maxSum) {
                    maxSum = currentSum;
                }
            }
        }

        return maxSum;
    }
}

#endif
