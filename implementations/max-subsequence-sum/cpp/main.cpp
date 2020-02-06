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


int main() {
    std::vector<int> nums = {-2, 11, -4, 13, -5 -2};
    int sum;

    sum = MaxSubsequenceSum::v1(nums);
    std::cout << "Sum (v1): " << sum << "\n";

    sum = MaxSubsequenceSum::v2(nums);
    std::cout << "Sum (v2): " << sum << "\n";

    return 0;
}
