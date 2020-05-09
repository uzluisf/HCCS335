# Lecture 22: Greedy algorithms (cont.)

## Bin packing problem

We are given $N$ items of sizes $s_1, s_2, \ldots, s_N$. All sizes satisfy $) <
s_i \leq 1$. The problem is to pack these items in the fewest number of bins,
given that each bin has unit capacity.

For example, the following figure shows an optimal packing for an item list with
sizes 0.2, 0.5, 0.4, 0.7, 0.1., 0.3, 0.8.

![Optimal packing for 0.2, 0.5, 0.4, 0.7, 0.1, 0.3, 0.8](images/optimal-packing-example.png "")

There are two versions of the bin packing problem:

* **online bin packing**, each item must be placed in a bin beore the next item
  can be processed.
  * Data (i.e., items of size $s_i$) arrive in sequential fashion.
  * Decided which bin to use after each $s_i$ and before $s_{i+1}$ arrives.
  * Cannot **undo** a decision later on.

* **offline bin packing**, we don't need to do anything until all the input has
  been read.
  * Wait for all data to arrive.
  * Decide packing having all data in place.

### Online bin packing

**Question:** Can an online algorithm can actually always give an optimal
answer, even if it is allowed unlimited computation? 

Consider the sequence $l_1$:

![Sequence $l_1$](images/online-bin-packing-sequence-01.png "")

What will an optimal algorithm $A$ do? It'd pack in $M$ bins.

Now consider the sequence $l_2$:

![Sequence $l_2$](images/online-bin-packing-sequence-02.png "")

What will an optimal algorithm $A$ do? It'll pack the items in $M$ bins,
waiting for the next $M$ (which won't arrive). Thus, the optimal solution 
$\lceil M/2 \rceil$ bins is not achieved.

Therefore, the answer to the question of whether or not an online algorithm
provide an optimal solution is **no** since an online algorithm cannot predict
the next item.

>**Theorem:**
>
>There are inputs that force any online bin packing algorithm to use at least
>$3/4$ the optimal number of bins.
>
>**Proof:**
>
>Suppose this is not the case, in other words, $M$ is even.
>Consider any online algorithm $A$ running on input sequence $l_1$ (sequence of
>$M$ small items and $M$ large items).
>
>Suppose that after first $M$, $b$ bins are used. At this point of the
>algorithm, the optimal number of bins $M/2$ because we can place two elements
>in each bin. Thus, we know that $2b/M < 4/3$ by our assumption of a better than
>$4/3$ performance guarantee.
>
>All bins created after $b^{\text{th}}$ bin must contain exactly one item, since
>all small items are placed in the first $b$ bins and two large items will not
>fit in a bin.
>
>* Since the first $b$ bins can have at most $2$ items each, and the remaining
   bins have one item each, we see that packing $2M$ items will require at least
   $2M-b$ bins.
>* Since the $2M$ items can be optimally packed using $M$ bins, our performance
   guarantee assures us that $(2M-b)/M < 4/3$.
>* The first inequality implies that $b/M < 2/3$ and the second one implies that
   $b/M > 2/3$, which is a contradiction.

### Simple algorithms

There are three simple algorithms that guarantee that the number of bins used is
no more than twice optimal. There are also quite a few more complicated
algorithms with better guarantees.

**Next fit**

* Extremely easy to implement
* When item $s_i$ arrives, look at last bin $B_j$. If $s_i$ fits in $B_j$, it's
placed there. Otherwise, it's placed in a new bin $B_{j+1}$.

![Next ﬁt for 0.2, 0.5, 0.4, 0.7, 0.1, 0.3, 0.8](images/next-fit-example-bin-packing.png "")

>**Theorem:**
>
>Let M be optimal number of bins required to pack a list of $I$ items.
>Then next fit never uses more than $2M$ bins. There are sequences such that
>next fit uses $2M-2$ bins.
>
>**Proof:**
>
>Consider two adjacent bins $B_j$ and $B_{j+1}$. The sum of sizes of $B_j$ and
>$B_{j+1}$ should be greater (>) than 1. So, the sum of empty space in $B_j$ and
>$B_{j+1}$ is smaller (<) than 1. Therefore at most (<) $1/2$ space is wasted
>per bin. QED.

![Optimal packing for $0.5, 2/N, 0.5, 2/N, 0.5, 2/N, \ldots$](images/optimal-fit-example-bin-packing.png "")

**First fit**

The **first fit** strategy is to scan the bins in order and place the new item
in the first bin that is large enough to hold it. Thus, a new bin is created
only when the results of previous placements have left no other alternative.

![First fit for $0.2, 0.5, 0.4, 0.7, 0.1, 0.3, 0.8$](images/first-fit-example-bin-packing.png "")

* Scan bins in order and place new item in the **first** bin that fits in.
* A naive implementation yields a running time of $O(N^{2})$. However, it can be
  implemented in $O(NlogN)$.
* At any point at *most* one bin can be more than half-empty.
* It has similar bounds as next fit, but better.

**Best fit**

* Instead of placing a new item in the first spot that is found, it is placed in
  the *tightest* spot among all bins.
* It doesn't offer a huge advantage over first fit but it has a simple
  implementation.

![Best fit for $0.2, 0.5, 0.4, 0.7, 0.1, 0.3, 0.8$](images/best-fit-example-bin-packing.png "")

### Offline bin packing

**Can we improve online solutions when all data is available?** If we are
allowed to view the entire item list before producing an answer, then we should
expect to do better. Indeed, since we can eventually find the optimal packing by
exhaustive search, we already have a theoretical improvement over the online
case.

The major problem with all the online algorithms is that it is hard to pack the
large items, especially when they occur late in the input. Thus, we can

* sort the items, placing the largest items first.
* then apply **first fit** or **best fit**, yielding the algorithms **first fit
  decreasing** and **best fit decreasing**, respectively.

**First fit decreasing**

![First fit for $0.8, 0.7, 0.5, 0.4, 0.3, 0.2, 0.1$](images/first-fit-decreasing-example-bin-packing.png "")

**Best fit decreasing**
