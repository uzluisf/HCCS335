# Priority Queues(Binomial Queues) and Sorting

## Binomial queues

Both leftist and skew heaps support merging, insertion, and `deleteMin`
in $O(log N)$ time per operation. Binomial queues support all three operations in
$O(log N)$ worst-case time per operation, **but** insertions take constant time
on average.

Structurally, a **binomial queue** differs from all the priority queue
implementations in that a *binomial queue is not a heap-ordered
tree but rather a collection of heap-ordered trees*, known as a **forest**. Each of
the heap-ordered trees is of a constrained form known as a binomial tree.
There is at most one binomial tree of every height. A binomial tree of height
$0$ is a one-node tree; a binomial tree, $B_k$ , of height $k$ is formed by
attaching a binomial tree, $B_{k-1}$ , to the root of another binomial tree,
$B_{k-1}$.

![Binomial trees $B_0$, $B_1$, $B_2$, $B_3$ and $B_4$](images/binomial-trees-b1-b4.png)

A binomial tree, $B_k$, consists of a root with children $B_0$, $B_1$, $\ldots$,
$B_{k-1}$. Binomial trees of height $k$ have exactly $2^k$ nodes, and the 
**number of nodes at depth** $d$ is the **binomial coefficient** $\binom{k}{d}$.

If we impose heap order on the binomial trees and allow at most one binomial
tree of any height, we can represent a priority queue of any size by a
collection of binomial trees. Suppose, we've a priority queue of size 13. This
could be represented by the forest $B_3$, $B_2$, $B_0$. In binary
representation, $13 = 1 \times 2^{3} + 1 \times 2^{2} + 0 \times 2^{1} + 1
\times 2^{0}$, which also represents the fact that $B_3$, $B_2$, and $B_0$ are
present in the representation and $B_1$ is not
(i.e., $0 \times 2^{1}$).

For example, a priority queue of $6$ elements could be represented
as the binomial queue down below.

![Binomial queue $H_1$ with six elements](images/binomial-queue-six-elements.png "")

### Operations

#### Merge

Merging two binomial queues is a conceptually easy operation. Consider 
the two binomial queues, $H_1$ and $H_2$ with $6$ and $7$ elements 
respectively down below.

![Two binomial queues $H_1$ and $H_2$](images/two-binomial-queues-h1-h2.png "")

The merge is performed by essentially adding the two queues together and
performed as follows:

* Let $H_3$ be the new binomial queue. Since $H_1$ has no binomial tree of
  height 0 and $H_2$ does, we can just use the binomial tree of height $0$ in
  $H_2$ as part of $H_3$. Next, we add binomial trees of height $1$. Since both
  $H_1$ and $H_2$ have binomial trees of height $1$, we merge them by making
  the larger root a subtree of the smaller, creating a binomial tree of height 
  $2$, as shown down below. Thus, $H_3$ will not have a binomial tree of height
  $1$.

![Merge of the two $B_1$ trees in $H_1$ and $H_2$](images/merge-of-b1-trees-in-h1-and-h2.png "")

* There are now three binomial trees of height $2$, namely, the original trees
  of $H_1$ and $H_2$ plus the tree formed by the previous step. We keep one
  binomial tree of height $2$ in $H_3$ and merge the other two, creating a
  binomial tree of height $3$. Since $H_1$ and $H_2$ have no trees of height
  $3$, this tree becomes part of $H_3$ and we are finished. The resulting
  binomial queue is shown down belown.

![Binomial queues $H_3$: result of merging $H-1$ and $H_2$](images/binomial-queue-h3-result.png)


Since merging two binomial trees takes constant time with almost any reasonable
implementation, and there are $O(log N)$ binomial trees, the merge takes 
$O(log N)$ time in the worst case. To make this operation efﬁcient, 
*we need to keep the trees in the binomial queue sorted by height*,
which is certainly a simple thing to do.

#### Insertion

Insertion is just a special case of merging, since we merely create a one-node
tree and perform a merge. The worst-case time of this operation is likewise
$O(log N)$. More precisely, if the priority queue into which the element is being
inserted has the property that the smallest nonexistent binomial tree is $B_i$,
the running time is proportional to $i + 1$.

For example, $H_3$ (from previous examples) is missing a binomial tree of height
$1$, so the insertion will terminate in two steps. Since each tree in a 
binomial queue is present with probability $1/2$, it follows that we expect an
insertion to terminate in two steps, so the average time is constant. 

As an example, let's see the binomial queues that are formed by inserting 
the numbers $1$ through $7$ in order.

![Insertions into binomial queue](images/binomial-queue-insertions.png "")

While inserting, we find inserting $4$ is a bad case. We merge $4$ with $B_0$,
ontaining a new tree of height $1$. We then merge this tree with $B_1$,
obtaining a tree of height $2$, which is the new priority queue. We count this
as $3$ steps (two merges plus the stopping case). The next insertion after $7$
is inserted is another bad case and would require $3$ tree merges.

#### deleteMin

A `deleteMin` can be performed by first finding the binomial tree with the
smallest root. Let this tree be $B_k$, and let the original priority queue be
$H$. We remove the binomial tree $B_k$ from the forest of trees in $H$, forming
the new binomial queue $H'$. We also remove the root of $B_k$, creating 
binomial trees $B_0$, $B_1$, $\ldots$, $B_{k-1}$, which collectively form
priority queue $H''$. We finish the operation by merging $H'$ and $H''$.

As an example, suppose we perform a `deleteMin` on $H_3$ (from previous examples),
The minimum root is $12$, so we obtain the two priority queues $H'$ and
$H''$ as shown below.

![Binomial queue $H'$, containing all the binomial trees in $H_3$ except $B_3$](images/binomial-queue-h-prime.png "")

![Binomial queue $H''$: $B_3$ with $12$
removed](images/binomial-queue-h-double-prime.png "")

The binomial queue that results from merging $H'$ and $H''$ is the final answer,
which is shown down below.

![Result of applying `deleteMin` to $H_3$](images/binomial-queue-after-deleteMin.png)

### Implementation

#### deleteMin

The `deleteMin` operation requires the ability to find all the subtrees of the root
quickly, so the standard representation of general trees is required:

* The children of each node are kept in a linked list, and each node has a
  pointer to its first child (if any). This operation also requires that the
  children be ordered by the size of their subtrees.

* We also need to make sure that it is easy to merge two trees. When two trees
  are merged, one of the trees is added as a child to the other. Since this new
  tree will be the largest subtree, it makes sense to maintain the subtrees in
  decreasing sizes. Only then will we be able to merge two binomial trees, and
  thus two binomial queues, efﬁciently. The binomial queue will be an array of
  binomial trees.

To summarize, then, each node in a binomial tree will contain the data,
first child, and right sibling. The children in a binomial tree are arranged
in decreasing rank. 

The binomial queue depicted in the figure below

![Binomial queue $H_3$ drawn as a forest](images/binomial-queue-h3-tree-forest.png "")

can be represented as a linked list as shown in the figure.

![Representation of binomial queue $H_3$](images/representation-binomial-queue-h3.png)

### Summary

#### Basic binary heap

* $O(log n)$ for insertion and `deleteMin`
* $O(1)$ average for insertion
* $O(n)$ to build the heap

#### Heaps with $𝑂(log 𝑛)$ merge operation

* Leftist heap – Simple recursive data structure
    * Does not have $O(1)$ average for insertion
* Skew heap – Amortized version of leftist heap
* Binomial queue – Based on a forest. Simple to describe and has the same
  complexity guarantees as the binary heap.

## Sorting

### Comparison-based sorting

In **comparison-based sorting**, the following conditions are assumed:

* Each algorithm is passed an array containing the elements; we assume all array
  positions contain data to be sorted. We will assume that $N$ is the number of
  elements passed to our sorting routines.

* We will also assume the existence of the $<$ and $>$ operators, which can be
  used to place a consistent ordering on the input. Besides the assignment
  operator, these are the only operations allowed on the input data.

This interface is unlike the STL sorting algorithms, where sorting is
accomplished by use of the function template `sort`. The parameters to sort
represent the start and endmarker of a (range in a) container and an optional
comparator:

```cpp
void sort( Iterator begin, Iterator end );
void sort( Iterator begin, Iterator end, Comparator cmp );
```

The iterators must support random access. The sort algorithm does not guarantee
that equal items retain their original order (if that is important, use
`stable_sort` instead of `sort`).

### Insertion order

Insertion sort is one of the simplest algorithms and it consists of $N-1$
**passes**. For pass $p=1$ through $N-1$, insertion sort ensures that the
elements in positions $0$ through $p$ are in sorted order. Insertion sort makes
use of the fact that elements in positions $0$ through $p-1$ are already known
to be in sorted order.

![Insertion sort after each pass](images/insertion-sort-example.png "")

```cpp
template<typename Comparable>
void insertionSort( std::vector<Comparable>& a ) {
    for (int p = 1; p < a.size(); p++) {
        Comparable tmp = std::move(a[p]);

        int j;
        for (j = p; j > 0 && tmp < a[j-1]; j--) {
            a[j] = std::move(a[j-1]);
        }
        a[j] = std::move(tmp);
    }
}
```

In a STL implementation of insertion sort, the sort routines receive a 
pair of iterators that represent the start and endmarker of a range.
A two-parameter sort routine uses just that pair of iterators and presumes that
the items can be ordered, while a three-parameter sort routine has a function
object as a third parameter.

```cpp
/*
The two-parameter version calls the three-parameter version, * using C++11
decltype.
*/
template<typename Iterator>
void insertionSort( const Iterator& begin, const Iterator& end ) {
    insertionSort(begin, end, less<decltype(*begin)>{});
}
```

```cpp
template<typename Iterator, typename Comparator>
void insertionSort( const Iterator& begin, const Iterator& end, Comparator lessThan) {
    if (begin == end) { return };

    Iterator j;
    for (Iterator p = begin + 1; p != end; p++) {
        auto tmp = std::move(*p);
        for (j = pl; j !+ begin && lessThan(tmp, *(j-1)); j--) {
            *j = std::move(*(j-1));
        }
        *j = std::move(tmp);
    }
}
```

The worst-case running time for the insertion sort algorithm is $\Theta(N^{2})$
and its best-case running time (when the input is presorted) is $O(N)$, in which
case a single comparison per item is performed.

### A lower bound for simple sorting algorithms

An **inversion** in an array of numbers is any ordered pair $(i, j)$ having the
property that $i < j$ but $a[i] > a[j]$. Take for example the input list 
$34, 8, 64, 51, 32, 21$. This list has $9$ inversions:

```text
(34, 8)
(34, 32)
(34, 21)
(64, 51)
(64, 32)
(64, 21)
(51, 32)
(51, 21)
(32, 21)
```

Notice that this is exactly *the number of swaps* that needed to be (implicitly)
performed by insertion sort. This is always the case, because

* swapping two adjacent elements that are out of place removes exactly one inversion, and
* a sorted array has no inversions

Since there is $O(N)$ other work involved in the algorithm, the running time of
insertion sort is $O(I + N)$, where $I$ is the number of inversions in the
original array. Thus, insertion sort runs in linear time if the number of
inversions is $O(N)$.

**Theorem:** The average number of inversions in an array of $N$ distinct elements
is $\frac{N(N-1)}{4})$.

>**Proof:**
>
>For any list, $L$, of elements, consider $L_r$, the list in reverse order. 
>The reverse list of the example is 21, 32, 51, 64, 8, 34. Consider any pair of
>two elements in the list $(x, y)$ with $y > x$. Clearly, in exactly one of $L$
>and $L_r$ this ordered pair represents an inversion. The total
>number of these pairs in a list $L$ and its reverse $L_r$ is $\frac{N(N −
>1)}{2}$. Thus, an average list has half this amount, or $\frac{N(N − 1)}{4}$
>inversions.

This theorem implies that insertion sort is quadratic on average. It also
provides a very strong lower bound about any algorithm that only exchanges
adjacent elements.

**Theorem:** Any algorithm that sorts by exchanging adjacent elements requires
$\Omega(N2)$ time on average.

>**Proof:**
>
>The average number of inversions is initially $\frac{N(N-1)}{4} =
>\Omega(N^{2})$. Each swap removes only one inversion, so $\Omega(N^2)$ swaps are
>required.


