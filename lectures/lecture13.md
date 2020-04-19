# Heaps (cont.)

## The selection problem

> Suppose you have a group of $N$ numbers and would like to determine the $k^{th}$
> largest. The input is a list of $N$ elements, which can be totally ordered, and
> an integer $k$.

There are several algorithms to solve the problem:

* **1st algorithm:** Read the elements into an array and sort them, and return
  the appropriate element. Assuming a simple sorting algorithm, the running
  time is $O(N^2)$.

* **2nd algorithm:** Read $k$ elements into an array and sort them. The smallest
  of these is in the $k^{\text{th}}$ position. We process the remaining elements
  one by one. As an element arrives, it is compared with the $k^{\text{th}}$
  element in the array. If it's larger, then the $k^{\text{th}}$ element is
  removed, and the new element is placed in the correct place among the
  remaining $k-1$ elements. When the algorithm ends, the element in the
  $k^{\text{th}}$ position is the answer. The running time is $O(N \cdot k)$.

* **3rd algorithm:** Assume we're interested on finding the
  $k^{\text{th}}$ smallest element. Read the $N$ elements into an array. We then
  apply the `buildHeap` algorithm to this array. Finally, we perform $k$
  `deleteMin` operations. The last element extracted from the heap is the
  answer.

  The worst-case timing is $O(N)$ to construct the heap, if `buildHeap` is used,
  and $O(log\;N)$ for each `deleteMin`. Since there are $k$ `deleteMin`s, we
  obtain a total running time of $O(N + klog\;N)$. If $k = O(N/log\;N)$, then
  the running time is dominated by the `buildHeap` operation and is $O(N)$. For
  larger values of $k$, the running time is $O(klog\;N)$. If $k = \lceil N/2
  \rceil$, then the running time is $\Theta(Nlog\;N)$.

  Notice that if we run this program for $k = N$ and record the values as they
  leave the heap, we will have essentially sorted the input ﬁle in $O(Nlog\;N)$
  time.

* **4th algorithm:** Here we return to the original problem and find the
  $k^{\text{th}}$ largest element. We use the same idea from the 2nd algorithm.
  At any point in time we will maintain a set $S$ of the $k$ largest elements.
  After the first $k$ elements are read, when a new element is read it is
  compared with the $k^{\text{th}}$ largest element, which we denoted by
  $S_{k}$. Notice that $S_{k}$ is the the smallest element in $S$. If the new
  element is larger, then it replaces $S_{k}$ in $S$. $S$ will then have a new
  smallest, which may or may not not be the newly added element. At the end of
  the input, we find the smallest element in $S$ and return it as the answer.

  The first $k$ elements are placed into the heap in total $O(k)$ time with a
  call to `buildHeap`. The time process each of the remaining elements $O(1)$,
  to test if the element goes into $S$, plus $O(log\;k)$, to delete $S_{k}$ and
  insert the new element if this is necessary. Thus, the total time is
  $O(k+(N-k) log\;k) = O(Nlog\;k)$. This algorithm also gives a abound of
  $\Theta(Nlog\;N$ for finding the median.


## d-heaps

Binary heaps are so simple that they are almost always used when priority queues
are needed. A **$d$-heap** is a simple generalization of a heap where all nodes
all nodes have $d$ children (thus, a binary heap is a 2-heap).

![A d-heap (d = 3)](images/d-heap-example.png "d-heap example")

A $d$-heap is much shallower than a binary heap, which improves the running time
of `insert`s to $O(log_{d}\;N)$. However, for large $d$, the `deleteMin`
operation is more expensive, because even though the tree is shallower, the
minimu of $d$ children must be found, which takes $d-1$ comparisons using a
standard algorithm. This raises the time for this operation to $O(dlog_{d}N)$.
If $d$ is a constant, both running times are, of course, $O(log\;N)$. 

$d$-heaps are interesting in theory, because there are many algorithms where the
number of insertions is much greater than the number of `deleteMin`s (and thus a
theoretical speedup is possible). They are also of interest when the priority
queue is too large to ﬁt entirely in main memory. In this case, a d-heap can be
advantageous in much the same way as B-trees. Finally, there is evidence
suggesting that $4$-heaps may outperform binary heaps in practice.

The most glaring weakness of the heap implementation, aside from the inability
to perform `find`s, is merging two heaps into one. This extra operation is known
as a `merge`. There are quite a few ways to implement heaps so that the running
time of a merge is $O(log\;N)$.

## Leftist heaps

HERE

### Leftist heap property

We deﬁne the **null path length**, $npl(X)$, of any node $X$ to be the length of
the shortest path from $X$ to a node without two children. Thus, the $npl$ of a
node with zero or one child is $0$, while $npl(nullptr) = −1$.

### Leftist heap property

## Skew heaps

A **skew heap** is a self-adjusting version of a leftist heap that is incredibly
simple to implement. The relationship of skew heaps to leftist heaps is
analogous to the relation between splay trees and AVL trees. Skew heaps are
binary trees with heap order, but there is no structural constraint on these
trees. Unlike leftist heaps, no information is maintained about the null path
length of any node. The right path of a skew heap can be arbitrar- ily long at
any time, so the worst-case running time of all operations is O(N). However, as
with splay trees, it can be shown (see Chapter 11) that for any M consecutive
operations, the total worst-case running time is O(M log N). Thus, skew heaps
have O(log N) amortized cost per operation.
