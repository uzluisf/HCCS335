
## Usage

```
#include "lib/BinarySearch.h"
#include <iostream>

int main() {
    std::vector<int> vec = {1, 2, 3, 5, 6};
    int needle = 3;

    int res = binarySearch(vec, needle);

    if (res != -1) {
        std::cout << "Position: " << res << "\n";
    }
    else {
        std::cout << "Value " << needle << " not in array\n";
    }

    return 0;
}
```

To compile the program above (e.g., `main.cpp`) in the same directory as
`lib` directory:

```
$ g++ -o prog lib/BinarySearch.h main.cpp
```

This creates the executable `prog`.
