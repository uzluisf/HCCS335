# Lecture 7: AVL trees (cont.), splay trees, and B-trees

## AVL tree implementation

**implementation:** [implementations/AVLTree](https://github.com/uzluisf/HCCS335/blob/master/implementations/cpp/lib/AVLTree.h)

### Insertion

```bash
Input: X is the item to insert and T is the node that roots the subtree.

insert( X, T ):
    if T = null:
        T ← new AVLNode(X, null, null)
    else if X < T.item:
        insert(X, T.left)
    else if T.item < X:
        insert(X, T.right)

    balance(T)
```

### Balance

```bash
Brief: Balance the subtree T by performing either a single or double rotation.
Input: T is the tree node to be rebalanced.

balance( T ):
    if T = null:
        return
    if T.left.height - T.right.height > 1:
        if T.left.left.height ≥ T.left.right.height:
            rotate-with-left-child(T)
        else:
            double-with-left-child(T)
    else if T.right.height - T.left.height > 1:
        if T.right.right.height ≥ T.right.left.height:
            rotate-with-right-child(T)
        else:
            double-with-right-child(T)

    T.height = max(T.left.height, T.right.height) + 1
```

### Left-left single rotation (case 1)

```bash
Brief: Rotate binary tree with left child. This is a single rotation for case 1.
Input: K2 is the tree node that needs to be rebalanced.

rotate-with-left-child( K2 ):
    K1        = K2.left
    K2.left   = K1.right
    K1.right  = K2
    k2.height = max(K1.left.height, K2.right.height) + 1
    K1.height = max(K1.left.height, K2.height) + 1
    K2        = K1
```

### Left-right double rotation (case 2)

```bash
Brief: Rotate binary tree with left child. This is a double rotation for case 2.
Input: K3 is the tree node that needs to be rebalanced.

double-with-left-child( K3 ):
    rotate-with-right-child(K3.left)
    rotate-with-left-child(K3)
```

### Deletion

## Amortized cost

Consider a sequence of $M$ operations (insert/delete/find) and suppose the total
cost is $O(M * f(N))$, irrespective of the actual sequence of operations;
the average cost if $O(f(N))$ for each operation. This is called **amortized
running time**. One caveat of this is that individual operations in the sequence
can be expensive.

## Splay trees

A **splay tree** is a tree with amortized time of $O(logN)$. It guarantees that
any $M$ consecutive tree operations starting from an empty tree take at most
$O(MlogN)$ time (i.e., the tree has an $O(logN)$ amortized cost per operation). 

The trick is to rebalance the tree after the `find()` operation. This is done by
bringing the item return by `find()` to the tree's root while applying AVL
rotations on the way to the root. The practical utility of this method is that
in many applications when a node is accessed, it is likely to be accessed again
in the near future.

Furthermore, splay trees don't require the maintenance of height or balance
information, thus saving space and simplifying the code to some extent.

### A simple idea (that doesn't work)

One way to perform the restructing described above is by performing single
rotations from the bottom up. This means that we rotate every node on the access
path with its parent. For example, consider the following figures that show
what happens after an access (a `find`) on $k_1$ in the a tree. The rotations
have the effect of pushing $k_1$ all the way to the root making future accesses
on $k_1$ easy but unfortunately, it pushed another node ($k_3$) almost as deep
as $k_1$ used to be.

![Before rotation: pushing k1 to the root](images/pushing-k1-to-the-root-0th-rotation.png "oth rotation: pushing k1 to the root")

![1st rotation: pushing k1 to the root](images/pushing-k1-to-the-root-1st-rotation.png "1st rotation: pushing k1 to the root")

![2nd rotation: pushing k1 to the root](images/pushing-k1-to-the-root-2nd-rotation.png "2nd rotation: pushing k1 to the root")

![3rd and 4th rotations: pushing k1 to the root](images/pushing-k1-to-the-root-3rd-4th-rotation.png "3rd & 4th rotations: pushing k1 to the root")


### Splaying

The splaying strategy is similar to the rotation from the bottom up, except that
we're a little more selective about how rotations are performed.

Let $X$ be a (non-root) node on the access path at which we are rotating. If
the parent of $X$ is the root of the tree, we merely rotate $X$ and the root. This
is the last rotation along the access path. Otherwise, $X$ has both a parent ($P$)
and a grandparent ($G$), and there are two cases, plus symmetries, to consider:

* The ﬁrst case is the **zig-zag** case. Here $X$ is a right child and $P$
is a left child (or vice versa). If this is the case, we perform a double
rotation, exactly like an AVL double rotation.

![Zig-Zag](images/avl-zig-zag.png "zig-zag")

* The second case is a **zig-zig** case: $X$ and $P$ are both left children (or,
  in the symmetric case, both right children).

![Zig-Zig](images/avl-zig-zig.png "zig-zig")

### Fundamental property of splay trees

* When access paths are long, they lead to longer-than normal search time.
* When accesses are cheap, the rotations are not as good and can be bad.
* The extreme case is the initial tree formed by the insertions. 

### Summary

8 The analysis of splay trees is difﬁcult, because it must take into account the
ever-changing structure of the tree.

* Splay trees are much simpler to program than most balanced search trees, since
  there are fewer cases to consider and no balance information to maintain.

* Some empirical evidence suggests that this translates into faster code in
  practice, although the case for this is far from complete.

* There are several variations of splay trees that can perform even better in
  practice.

## B-trees

A **B-tree** is a generalization of a binary tree, which is efficient in the
[*external memory model*]().

So far, we have assumed that we can store an entire data structure in the main
memory of a computer. Now imagine the scenario of a tree that is immensely huge
and can't fit into memory.  Now the data structure must reside on the hard,
which changes the rules of the game, because the Big-Oh model is no
longer meaningful. The problem is that a Big-Oh analysis assumes that all
operations are equal. However, this is not true, especially when disk I/O is
involved.

Here is how the typical search tree performs on disk: Suppose we want to access
the driving records for citizens in the state of Florida. We assume that we have
10,000,000 items, that each key is 32 bytes (representing a name), and that a
record is 256 bytes. We assume this does not ﬁt in main memory and that we are 1
of 20 users on a system (so we have 1/20 of the resources). Thus, in 1 sec we
can execute many millions of instructions or perform six disk accesses.

The first idea might be to use a unbalanced binary search tree but it turns into a 
disaster. In the worst case, it has linear depth and thus could require
10,000,000 disk accesses. On average, a successful search would require 
$1.38log N$ disk accesses, and since $log 10000000 ≈ 24$, an average search 
would require 32 disk accesses, or 5 sec.

In a typical randomly constructed tree, we would expect that a few nodes are
three times deeper; these would require about 100 disk accesses, or 16 sec.

An AVL tree is somewhat better. The worst case of $1.44 log N$ is unlikely to
occur, and the typical case is very close to log N. Thus an AVL tree would use
about 25 disk accesses on average, requiring 4 sec.

We want to reduce the number of disk accesses to a very small constant, such as
three or four. Basically, we need smaller trees (i.e., trees with more
branching and thus less height). An **$M$-ary search tree** allows $M$-way
branching. *As branching increases, the depth decreases*. Whereas a complete
binary tree has height that is roughly $log_{2}N, a complete $M$-ary tree has
height that is roughly $log_{M} N$.

![5-ary tree with 31 nodes](images/5-ary-tree-example.png "5-ary tree")

A **B-tree** of order $M$ is an $M$-ary tree with the following properties:

1. The data items are stored at leaves.
2. The nonleaf nodes store up to $M − 1$ keys to guide the searching;
key $i$ represents the smallest key in subtree $i + 1$.
3. The root is either a leaf or has between two and $M$ children.
4. All nonleaf nodes (except the root) have between $M/2$ and $M$ children.
5. All leaves are at the same depth and have between $L/2$ and $L$ data items, 
for some $L$.

$M$ and $L$ are determined based on disk block (one access should load a whole
node).

![B-tree of order 5](images/order-5-b-tree.png "b-tree example")

The "B-tree of order 5" figure depicts a B-tree of order 5:

* Each represents a disk block.
* All nonleaf nodes have between $3$ and $5$ children.
* $L = M = 5$, however this not need to be the case.
* Since $L = 5$, each leaf has between $3$ and $5$ data items.

In the Florida driving records example:

* A single block is supposed to hold $8,192$ bytes.
* Each key uses 32 bytes. In a B-tree of order $M$, we would've $M-1$
  keys, for a total of $32 \times (M-1) = 32M - 32$ bytes, plus $M$
  branches.
* Each branch is essentially a number of another disk block, so we can
  assume a branch is $4$ bytes. Thus the branches use $4M$ bytes.
* The total memory requirement for a nonleaf node is thus $32M - 32 +
  4M = 36M-32$.
* Since $8192 \leq 36M-32$, the largest value of $M$ for which this is
  no more thant $8192$ is $228$. Thus we choose $M = 228$.
* Since each data record is $256$ bytes, we'd be able to fit $32$
  records in a block. Thus we'd choose $L=32$.
* Each leaf has between $16$ and $32$ data records and each internal
  node (except the root) branches in at least $114$ ways.
* Since there are $10,000,000$ records, there are, at most, $625,000$
  leaves. Consequently, in the worst case, leaves would be on level
  $4$ of the tree.
  * The worst-case number of accesses is given by approximately $log_{M/2}N$.
  * The root and the next level could be *cached* in main memory, so
    that over the long run, disk accesses would be needed only for
    level tree and deeper.


### Example

### Insertion

* Put it in the appropriate leaf. 
* If the leaf is full, break it in two, adding a child to the parent. 
* If this puts the parent over the limit, split upwards recursively.
* If you need to split the root, add a new one with two children. This is the
  only way you add depth.

![B-tree of order 5: Inserting 57](images/order-5-b-tree-inserting-57.png "b-tree example-insertion")

![B-tree of order 5: Inserting 55](images/order-5-b-tree-inserting-55.png "b-tree example-insertion")

![B-tree of order 5: Inserting 40](images/order-5-b-tree-inserting-40.png "b-tree example-insertion")

### Deletion

* Delete from appropriate leaf.
* If the leaf is below its minimum, adopt from a neighbor if possible.
* If that’s not possible, you can merge with the neighbor. This causes the
  parent to lose a branch and you continue upward recursively.

![B-tree of order 5: Inserting 40](images/order-5-b-tree-deleting-99.png "b-tree example-deletion")

