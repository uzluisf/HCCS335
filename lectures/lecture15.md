# Lecture 15: Sorting (cont.)


## Shellsort

* Named after the its inventor, Donald Shell.
* Among the first algorithm to break the quadratic time barrier.
* It works by comparing elements that are distant from each other; the distance
  between comparisons decreases as the algorithm runs until the last phase,
  in which adjacent elements are compared.

Shellsort uses a sequence, $h_1, h_2, \ldots, h_t$, called the **increment sequence**.
Any increment sequence will do as long as $h_1 = 1$, but some choices are better
than others. After a phase, using some increment $h_k$, for every $i$,
we have $a[i] \leq a[i + h_k ]$ (where this makes sense); all elements spaced
$h_k$ apart are sorted. The file is then said to be $h_k$-sorted.

The idea behind Shellsort is to arrange the list of elements so that, starting
anywhere, considering every $h^{th}$ element gives a sorted list.
In general, for each position, $i$, in $h_k, h_{k} + 1, \ldots, N-1$,
place the element in the correct spot among $i, i-h_{k}, i-2h_{k}$, and so
on. This action performs an insertion sort on $h_k$ independent of subarrays;
after all, Shellsort is a generalization of inserion sort.

A popular (but poor choice) for increment sequence is to use the sequence
suggested by Shell: $h_t = \lfloor N/2\rfloor$, and $h_{k} = \lfloor
h_{k+1}/2\rfloor$. The following C++ function implements Shellsort using
this sequence:

```cpp
template<typename Comparable>
void shellsort( std::vector<Comparable>& a ) {
    for (int gap = a.size() / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < a.size(); i++) {
            Comparable tmp = std::move(a[i]);
            int j = i;

            for (; j >= gap && tmp < [j-gap]; j -= gap) {
                a[j] = std::move(a[j-gap]);
            }
            a[j] = std::move(tmp);
        }
    }
}
```

Take for example the list $62, 83, 18, 53, 7, 17, 95, 86, 47, 69, 25, 28$.
Using the above function, we obtain the following:

```text
orig   => 62, 83, 18, 53, 7, 17, 95, 86, 47, 69, 25, 28
gap: 6 => 62, 83, 18, 53, 7, 17, 95, 86, 47, 69, 25, 28
gap: 3 => 53, 7, 17, 62, 25, 18, 69, 83, 28, 95, 86, 47
gap: 1 => 7, 17, 18, 25, 28, 47, 53, 62, 69, 83, 86, 95
```

### Worst-case analysis

**Theorem:** The worst-case running of Shellsort using Shell's increments is
$\Theta(N^{2})$.

> **Proof:**
> 
> Consider that $N$ is a power of $2$, which means all increments are even
> except for the last one which is $1$. We place the $N/2$ largest numbers at
> even positions and the $N/2$ smallest numbers at odd positions. To finish the
> proof, we must show the upper bound of $O(N^{2})$:
> 
> * At pass $h_k$: A pass with increment $h_k$ consists of $h_k$ inserion sorts of
> about $N/h_{k}$ elements. Since insertion sort is quadratic, the total cost of a
> pass is $O(h_{k}(N/h_{k})^{2}) = O(N^{2}/h_{k})$.
> 
> * Total cost of all passes: $O(\Sigma^{t}_{i =1} N^{2}/h_{i}) = O(N^{2}
>   \Sigma^{i}_{i=1} 1/h_{i})$. Because the increments form a geometric series with
>   common ration $2$, and the largest term in the series is $h_1 = 1$,
>   $\Sigma^{t}_{i=1} 1/h_{i} < 2$. Thus we obtain a total bound of $O(N^{2})$.
> 
> The problem with Shell’s increments is that pairs of increments are not
> necessarily relatively prime, and thus the smaller increment can have little
> effect. Among better improvements over Shell's original sequence, there are:
> 
> * Hibbard's increment sequence, which gives better results in practice (and
>   theoretically). His increments are of the form $1, 3, 7, \ldots, 2^{k} - 1$.
>   The key difference here is that consecutive increments have no common factors
>   (i.e., they're prime numbers). The Worst-case running time is $\Theta(N^{3/2})$.
> 
> * Sedgewick's increment sequence. This sequence is of the form $1, 5, 19, 41,
>   \ldots$, in which the terms are either of the form $9\cdot4^{i} - 9\cdot 2^{i} + 1$ 
>   or $4^{i} - 3\cdot 2^{i} + 1$. The worst-case running time is $O(N^{4/3})$.

## Heapsort

* Uses binary heap implemented as array
* $O(N logN)$ time bound for both worst- and average-case running time.
* In-place algorithm, uses O(1) extra memory.

The algorithm boils down to

1) starting with a given array,
2) build binary heap from N element, which is $O(N)$, and
3) perform $N$ `deleteMax()` operations which gives $O(NlogN)$.

The C++ code for the Heapsort algorithm is as follows. Unlike
the binary heap, where the data begin at array index $1$,  the array for
heapsort contains data at position $0$ so minor changes are required.

```cpp
template<typename Comparable>
void heapsort( std::vector<Comparable>& a ) {
    // buildHeap
    for (int i = a.size()/2 - 1;   i >= 0; i--)
        percolateDown(a, i, a.size());

    for (int j = a.size() - 1; j>0; j--) {
        std::swap(a[0], a[j]);
        percolateDown(a, 0, j);
    }
}

// Returns the index of the left child.
inline int leftChild( int i ) { return 2 * i + 1; }

template<typename Comparable>
void percolateDown( std::vector<Comparable>& a , int i, int n )  {
    int child;
    Comparable tmp;

    for (tmp = std::move(a[i]); leftChild(i) < n; i = child) {
        child = leftChild(i);
        if (child != n - 1 && a[child] < a[child+1])
            child++;
        if (tmp < a[child])
            a[i] = std::move(a[child]);
        else
            break;
    }

    a[i] = std::move(tmp);
}
```

## Mergesort

* Run in $O(N logN)$ worst-case running time.
* Number of comparisons is near optimal.
* Recursive
* Fundamental operation is the merging of two sorted lists, which takes $O(N)$
  and needs for extra O(N) space to merge arrays.
* Good for linked list since there's no need for random access.
* There's no need for extra space to merge sublists.
* Stable

```cpp
template<typename Comparable>
void mergeSort( std::vector<Comparable>& a ) {
    std::vector<Comparable> tmpArray(a.size());

    mergeSortInternal(a, tmpArray, 0, a.size()-1);
}

template<typename Comparable>
void mergeSortInternal(
    std::vector<Comparable>& a,   // array to sort
    std::vector<Comparable>& tmp, // array to place merged result
    int left,                     // leftmost index of the subarray
    int right,                    // rightmost index of the subarray
) {

    if (left < right) {
        int center = (left + right) / 2;              // get middle index
        mergeSortInternal(a, tmp, left, center);      // sort left subarray
        mergeSortInternal(a, tmp, center + 1, right); // sort right subarray
        merge(a, tmp, left, center + 1, right);       // merge sorted subarrays
    }
}

template<typename Comparable>
void merge(
    std::vector<Comparable>& a,   // array of Comparable items
    std::vector<Comparable>& tmp, // array to place merged result
    int leftPos,                  // leftmost index of subarray
    int rightPos,                 // index of the start of the second half
    int rightEnd                  // rightmost index of subarray
) {
    
    int leftEnd = rightPos = 1;
    int tmpPos = leftPos;
    int numElements = rightEnd - leftPos + 1;

    while (leftPos <= leftEnd && rightPos <= rightEnd) {
        if (a[leftPos] <= a[rightPos])
            tmp[tmpPos++] = std::move(a[leftPos++]);
        else
            tmp[tmpPos++] = std::move(a[rightPos++]);
    }

    // copy rest of left half
    while (leftPos <= leftEnd)
        tmp[tmpPos++] = std::move(a[leftPos++]);

    // copy rest of right half
    while (rightPos <= rightEnd)
        tmp[tmpPos++] = std::move(a[rightPos++]);

    for (int i = 0; i < numElements; i++, rightEnd--)
        a[rightEnd] = std::move(tmp[rightEnd]);
}
```

### Worst-case analysis

We have to write a recurrence relation for the running time. We will
assume that $N$ is a power of $2$ so that we always split into even halves.
For $N = 1$, the time to mergesort is constant, which we will denote by $1$.
Otherwise, the time to mergesort $N$ numbers is equal to the time to do
two recursive mergesorts of size $N/2$, plus the time to merge the two
sorted subarrays (which is linear). Mathematically, this could expressed
as follows:

\begin{align*}
T(1) &= 1 \\
T(N) &= 2T(N/2) + N \\
\end{align*}

This is a recurrence relation and finding Mergesort's running time
devolves into solving it:

\begin{align*}
T(N) &= 2T(N/2) + N \\
T(N) &= 4T(N/4) + 2N \\
T(N) &= 8T(N/8) + 3N \\
\ldots \\
T(N) &= 2^{k}T(N/2^{k}) + k\cdot N
\end{align*}

Using $k = log N$, we obtain

\begin{align*}
T(N) = N\cdot T(1) + N \cdot log N = N\cdot log N + N
\end{align*}

Thus, mergesort's running time is $O(N\text{log}N)$.

