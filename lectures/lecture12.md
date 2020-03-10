# Heaps

Priority queues are a special kind of binary tree called a **heap**,
which means “a disorganized pile.” This is in contrast to binary
search trees that can be thought of as a highly organized pile.

## Model

A **priority queue** is a data structure that allows at least the
following two operations: 

* `insert`, for inserting an element into the priority queue; and 
* `deleteMin`, which ﬁnds, returns, and removes the minimum element in
  the priority queue.

The `insert` operation is the equivalent of `enqueue`, and `deleteMin`
is the priority queue equivalent of the queue’s `dequeue` operation.

## Simple implementations

There are several obvious ways to implement a priority queue:

* Use a simple linked list, performing insertions at the front in
  $O(1)$ and traversing the list, which requires $O(N)$ time, to
  delete the minimum.

* Use an always-sorted linked list sorted; this makes insertions
  expensive ($O(N)$) and deleteMins cheap ($O(1)$).

* Use a binary search tree which gives an $O(log\;N)$ average running
  time for both operations. This is true in spite of the fact that
  although the insertions are random, the deletions are not. Using a
  search tree could be overkill because it supports a host of
  operations that are not required. The basic data structure we will
  use will not require links and will support both operations in $O(log\;N$)
  worst-case time. Insertion will actually take constant time on
  average, and our implementation will allow building a priority queue
  of $N$ items in linear time, if no deletions intervene.

If choosing between a simpled linked list or an always-sorted linked
list, the former is probably the better idea of the two, based on
the fact that there are never more deleteMins than insertions.

## Binary heap: an implicit binary tree

A **heap** is a binary tree that is completely ﬁlled, with the possible
exception of the bottom level, which is filled from left to right. Such
a tree is known as a **complete binary tree**. It is easy to show that 
a complete binary tree of height $h$ has between $2h$ and $2h+1 − 1$ nodes
which implies that the height of a complete binary tree is $log\;N$,
clearly $O(log\;N)$.

**Eytzinger’s method** allows us to represent a complete binary tree
as an array by laying out the nodes of the tree in breadth-first order
In this way, the root is stored at position 1, the root’s left child
is stored at position 2, the root’s right child at position 3, the
left child of the left child of the root is stored
at position 4, and so on.

![Eytzinger’s method represents a complete binary tree as an array](images/eytzinger-method-complete-binary-tree.png "eytzinger method complete binary tree")

If we apply Eytzinger’s method to a sufficiently large tree, some
patterns emerge:

* The left child of the node at index $i$ is at index $2i+1$.
* The right child of the node at index $i$ is at index $2i + 2$.
* The parent of the node at index $i$ is at index $\frac{i − 1}{2} = \lfloor i/2\rfloor$.

For a heap, links aren't required and the operations required to traverse
the tree are extremely simple and likely to be very fast on most
computers. A heap data structure will, then, consist of an array and
an integer representing the current heap size. 

### Heap-order property

A binary heap uses the above mentioned technique to implicitly
represent a complete binary tree in which the elements are
*heap-ordered* (*heap-order property*): 

> In a heap, the value stored at any index $i$ is not smaller than the
> value stored at index parent($i$), with the exception of the root
> value, which has no parent. It follows that the smallest value in
> the priority queue is therefore stored at position $0$ (the root).

Since we want to be able to ﬁnd the minimum quickly, it makes sense
that the smallest element should be at the root. If we consider that
any subtree should also be a heap, then any node should be smaller
than all of its descendants.

![Two completes (only the left tree is a heap)](images/heap-and-not-heap.png "heap vs not heap")

### Basic operations

**implementation:** [implementations/BinaryHeap](https://github.com/uzluisf/HCCS335/blob/master/implementations/cpp/lib/BinaryHeap.h)

#### `insert`

To insert an element `X` into the heap, we create a hole in the next
available location, since otherwise, the tree will not be complete. If
`X` can be placed in the hole without violating heap order, then we do
so and are done. Otherwise, we slide the element that is in the hole’s
parent node into the hole, thus bubbling the hole up toward the
root. We continue this process until `X` can be placed in the
hole. 

This general strategy is known as a **percolate up**; the new element
is percolated up the heap until the correct location is
found.

```text
Input: X is the element to insert
* array is the underlying structure for the heap.
* currentSize is the number of elements in the array.

insert( X ):
    if currentSize = (array.size - 1):
        array.resize(array.size * 2)
    
    # percolate up
    Int hole = currentSize++
    copy = X
    
    array[0] = copy
    while x < array[hold div 2]:
        array[hole] = array[hole div 2]
        hole = hole div 2
    array[hole] = array[0]
```

![Inserting an element in a binary heap](images/heap-insertion.png "inserting an element in a binary heap")

#### `deleteMin`

In this case, finding the minimum is easy; the hard part is removing
it. When the minimum is removed, a hole is created at the root (i.e.,
the smallest element is in the root). Since the heap now becomes one
smaller, it follows that the last element `X` in the heap must move
somewhere in the heap. If `X` can be placed in the hole, then we are
done. This is unlikely, so we slide the smaller of the hole’s children
into the hole, thus pushing the hole down one level. We repeat this
step until `X` can be placed in the hole. Thus, our action is to place
`X` in its correct spot along a path from the root containing *minimum*
children. This general strategy is known as a **percolate down**.

![Creation of the hole at the root](images/binary-heap-first-step-hole-creation.png "creation of the hole at the root")

![Next two steps in deleteMin](images/binary-heap-next-two-steps.png "next two steps in deleteMin")

![Last two steps in deleteMin](images/binary-heap-last-two-steps.png "last two steps in deleteMin")

A frequent implementation error in heaps occurs when there are an even
number of elements in the heap, and the one node that has only one
child is encountered. You must make sure not to assume that there are
always two children, so this usually involves an extra test.

One extremely tricky solution is always to ensure that your algorithm
thinks every node has two children. Do this by placing a sentinel, of
value higher than any in the heap, at the spot after the heap ends, at
the start of each *percolate down* when the heap size is even. You
should think very carefully before attempting this, and you must put
in a prominent comment if you do use this technique. Although this
eliminates the need to test for the presence of a right child, you
cannot eliminate the requirement that you test when you reach the
bottom, because this would require a sentinel for every leaf.

The worst-case running time for this operation is $O(log\;N)$. On average,
the element that is placed at the root is percolated almost to the
bottom of the heap (which is the level it came from), so the average
running time is $O(log\;N)$.

### Other operations

#### decreaseKey

* The `decreaseKey(p, delta)` operation lowers the value of the item
  at position `p` by a positive amount `delta`.

* It might violate the heap order, it must be fixed by a *percolate up*.

* It could be useful to system administrators: They can make their
  programs run with highest priority.


#### increaseKey

* The `increaseKey(p, delta)` operation increases the value of the item at position `p` by a positive amount `delta`.

* This is done with *percolate down*.

* Many schedulers automatically drop the priority of a process that is consuming excessive CPU time.

#### remove

* The `remove(p)` operation removes the node at position `p` from the heap.

* This is done by performing `decreaseKey(p, Inf)` and then performing `deleteMin()`.

* When a process is terminated by a user (instead of finishing
  normally), it must be removed from the priority queue.
  
#### buildHeap

The binary heap is sometimes constructed from an initial collection of
items. This constructor takes as input $N$ items and places them into
a heap. Obviously, this can be done with $N$ successive `insert`s. Since
each insert will take $O(1)$ average- and $O(log\;N)$ worst-case time, the
total running time of this algorithm would be $O(N)$ average- (from $N \times O(1)$) but
$O(N\;log\;N)$ (from $N \times O(log\;N)$) worst-case. Since this is a special
instruction and there are no other operations intervening, and we
already know that the instruction can be performed in linear average
time, it is reasonable to expect that with reasonable care a linear
time bound can be guaranteed.

The general algorithm is to place the N items into the tree in any
order, maintaining the structure property. Then, if `percolateDown(i)`
percolates down from node `i`, the `buildHeap` can
be used by the constructor to create a heap-ordered tree.

```cpp
BinaryHeap( const std::vector<T>& items )
    : array{ items.size() + 10 }, currentSize{ items.size() }
{
    for (int i = 0; i < items.size(); i++) {
        array[i + 1] = items[i];
    }
    buildHeap()
}

/*
Establish heap order property from an arbitrary
arrangement of items. Runs in linear time.
*/
void buildHeap() {
    for (int i = currentSize / 2; i > 0; i--) {
       percolateDown(i); 
    }
}

/*
Percolate down in the heap.
The hole is the index at which the percolate begins.
*/
void percolateDown( int hole ) {
    int child;
    T tmp = std::move(array[hole]);
    
    while (hold * 2 <= currentSize) {
        child = hole * 2;
        if (child != current && array[child + 1] < array[child]) child++;
        if (array[child] < tmp) array[hole] = std::move(array[child]);
        else break;
        hold = child;
    }
    
    array[hole] = std::move(tmp);
}
```

To bound the running time of `buildHeap`, we must bound the sum of the
heights of all the nodes in the heap. What we would like to show is
that this sum is $O(N)$.

**Theorem:** For the perfect binary tree of height $h$ containing
$2^{h+1}-1$ nodes, the sum of the heights of the nodes is $2^{h+1} -
1 - (h+1)$.

**Proof:** The tree consists of $2^{0} = 1$ node at height $h$, $2^{1}
= 2$ nodes at height $h-1$, $2^{2}$ nodes at height $h-2$, and in
general $2^i$ nodes at height $h-i$. The sum of the heights of all the
nodes is then

\begin{align*}
S &= \sum^{h}_{i=0} 2^{i}(h-i) \\
&= h + 2(h-1) + 4(h-2) + 8(h-3) + \cdots + 2^{h-1} \\
\end{align*}

Multiplying by $2$ gives the equation:

$$
2S = 2h + 4(h-1) + 8(h-2) + 16(h-3) + \cdots + 2^{h} \\
$$

Subtracting these equations:

\begin{align*}
2S - S = -h + 2 + 4 + 8 + \cdots + 2^{h-1} + 2^{h} = (2^{h+1} - 1) - (h+1)
\end{align*}

which proves the theorem.

A complete tree is not a perfect binary tree, but the result we have
obtained is an upper bound on the sum of the heights of the nodes in a
complete tree. Since a complete tree has between $2^h$ and $2^{h+1}$ nodes,
this theorem implies that this sum is $O(N)$, where $N$ is the number of
nodes.
