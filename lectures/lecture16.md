# Lecture 16: The disjoint sets class

The disjoint sets class is an efﬁcient data structure to solve the equivalence
problem which is easy to implement (e.g., routines read just a a few lines of
code and simple array can be used), extremely fast (i.e., constant average time
per operation), and very interesting from a theoretical point of view (e.g., 
its analysis is extremely difﬁcult).

## Equivalent relations

A **relation** $R$ is defined on a set $S$ if for every pair of element $(a,
b)$, where the elements $a$ and $b$ are elements of the set $S$, $a\; R\; b$
is either true or false. If $a\; R\;b$ is true, then $a$ is related to $b$.

An **equivalence relation** is a relation $R$ that satisfies three properties:

* (*Reflexive*) $a\; R\; a$, for all $a\in S$.
* (*Symmetric*) $a\; R\; b$ if and only if $b\; R\; a$.
* (*Transitive*) $a\; R\; a$ and $b\; R\; c$ implies that $a\; R\; c$.

## Dynamic equivalence problem

Given an equivalence relation $\sim$, the natural problem is to decide, for any
$a$ and $b$, if $a \sim b$.

If the relation is stored as a two-dimensional array of Boolean variables, then,
of course, this can be done in constant time. The problem is that the relation
is usually not explicitly, but rather implicitly, defined.

Suppose the equivalence relation is defined over the five-element set $\{a_1,
a_2, a_3, a_4, a_5\}$. There are 25 pairs of elements (e.g., $a_1 \sim a_2$,
$a_1 \sim a_3$, etc), each of which is either related or not related. As
an equivalence relation, we have:

* Reflexivity: $a_i \sim a_i$ where $i \in 1\ldots 5$.
* Simmetry: $a_2 \sim a_1$, $a_3 \sim a_2$
* Transitivity: $a_1 \sim a_2$, $a_2 \sim a_3$, $a_1 \sim a_3$

The **equivalence class** of an element $a \in S$ is the subset of $S$ that
contains all the elements that are related to $a$. To decide if $a\sim b$,
we need only to check whether $a$ and $b$ are in the same equivalence class,
which provides the strategy to solve the equivalence problem.


## Disjoint sets

* Input is initially a collection of $N$ sets, each with one element.
* Initial representation is that all relations (except reflexive relations) are false.
* Each set has a different element, so that $S_{i} \cap S_{j} = \emptyset$,
which makes the sets **disjoints** (i.e., no elements in common).

## Union-find

**Find:**

* This operation reyurns the name of the set (i.e., equivalence class)
containing a given element.

**Union:**

* If we want to add the relation $a\sim b$, then we must first find if $a$ and
  $b$ are related:
  * Perform a `find` operations on $a$ and $b$ and check if they're in the same
    equivalent class.
  * If already related, do nothing.
  * If not realted, apply the `union` to the equivalence classes of both `a` and
    `b`, and create a new class. The result of the `union` is to create a new 
    set $S_k = S_i \cup S_j$), which destroy the originals and preserve the
    **disjointness** of all sets.


In conjunction, this algorithm is known as the disjoint set **union-find**
algorithm. This algorithm 

* is **dynamic** because, during the course of the algorithm, the sets can
  change via the `union` operation.

* operates **online**, meaning that whenever `find` is performed, it must
give an answer before continuing. Similar to an oral exam.

* can also operate **offline**, meaning the sequence of `union`s and `find`s
is given, and you need to calculate final sets. Similar to a written exam.

## List-based implementations

The list-based implementation of union/find:

* Each set is stored in a sequence represented with a *linked list*.
* Each node should store an object containing the element and a reference to the
  set name.

When doing a unoin, we always move elements from the smaller set to the larger
one.Thus, each time an elment is moved it goes to a set whose size is at least
double its old set. This means an element can be moved at most $O(logN)$ times.

Therefore, the total time needed to do $M$ `find`s and $N-1$ `union`s is $O(M +
NlogN)$.

### A better implementation

Here:

* Each set (i.e., equivalence class) is represented by a tree, since each
  element in a tree has the same root.
* The root is used to name the set (i.e., the representative).
* Initially, each set contains a single element.
* Can be implemented using an array. For exaple, each entry $s[i]$ in the array
  represents the parent of element $i$. If $i$ is a root, then $s[i] = -1$.

![Example of multiple `union` operations](images/disjoint-set-union-example.png "")

## C++ implementation


```cpp
class DisjointSets {
    public: 
        /*
        Construct the disjoint sets object.
        */
        explicit DisjointSets( int numElements ) : s{ numElements - 1 } {

        }

        /*
        Return the set containing x.
        */
        int find( int x ) const {
            if (s[x] < 0) { return x; }
            else          { return find(s[x]); }

        }

        /*
        Get the union of two sets.

        Not the best way.
        */
        void union( int root1, root2 ) {
            s[root2] = root1;
        }

        private:
            std::vector<int> s;
}
```

## Smart union algorithms

### Union by size

The unions above were performed rather arbitrarily, by making the second tree a
subtree of the ﬁrst. A simple improvement is always to make the smaller tree a
subtree of the larger, breaking ties by any method; we call this approach
**union-by-size**.

![Union by size](images/smart-union-by-size.png "")

We can prove that *if unions are done by size, the depth of any node is never
more than* $log N$:  To see this, note 

* A node is initially at depth $0$. When depth increases as a result of a union,
  it is placed in a tree that is at least twice as large as before.
* Thus, the node's depth can be increased at most $log\; N$ times. This implies
  that the running time for a find operation is $O(log\; N)$, and a sequence of
  $M$ operations takes $O(M\cdot log\; N).

![Worst-case tree for $N=16$](images/worst-case-tree-union-by-size.png "")

### Union by height

This alternative implementation, which also guarantees that all the trees will
have depth at most $O(log\; N)$, is **union-by-height**:

* Keep track of the height, instead of the size, of each tree.
* Perform unions by making the shallow tree a subtree of the deeper tree.

This is an easy algorithm, since the height of a tree increases only when two
equally deep trees are joined (and then the height goes up by one). Thus,
*union-by-height is a trivial modiﬁcation of union-by-size*. Since heights of
$0$ would not be negative, we actually store the negative of height, minus an
additional $1$. Initially, all entries are $-1$.

![Forest with implicit representation for union-by-size and union-by-height](images/union-by-size-and-union-by-height.png "")

The `union` routine from before could be re-implemented as follows:

```cpp
void DisjointSets::union( int root1, int root2 ) {
    // root2 is deeper so make root2 the new root.
    if (s[root2] < s[root1]) {
        s[root1] = root2;
    }
    else {
        // update height if both sets are the same
        if (s[root1] == s[root2]) {
            s[root1]--;
        }

        // make root1 the new root
        s[root2] = root1;
    }
}
```

## Path compression

The **path compression** operation improves on the $O(M\cdot log\; N)$
worst-case `find` cost for $M$ `find`s. The operation is performed during
a `find` operation and is independent of the strategy used to perform `union`.
The end goal, for `find(x)`, is to make all nodes on the path from `x` to the
root to be children of the root. 

![Path compression example](images/path-compression-example.png "")

The updated code for `find` with path compression is as follows:

```cpp
int DisjointSets::find( int x ) {
    if (s[x] < 0) { return x; }
    else          { s[x] = find(s[x]); }
}
```

Path compression is perfectly compatible with union-by-size, and thus both
routines can be implemented at the same time. As for union-by-height,
path compression is not entirely compatibe since it can change the heights of
the tree and it's not clear how to recompute them efficiently.
Then the heights stored for each tree become estimated heights (sometimes known
as **ranks**), but it turns out that **union-by-rank** (which is what this has now
become) is just as efﬁcient in theory as union-by-size.

### Union by rank

After path compression, the height of the tree may not be accurate
so a height of the tree are estimated. This estimated height
is known as **rank** and is a upper bound on the actual heights.

## Summary

* Simple data structure to maintain disjoint sets.
* Important for graph theoretical problems.
* Union step is flexible and so we get a much more effective algorithm.
* Path compressions – earliest forms of self-adjustment 
  * Seen in splay trees and skew heaps
  * Use extremely interesting because from theoretical pov one of the first
    algorithms that was simple with a not-so-simple worst case analysis.
* Any sequence of $M = \Omega(N)$ union-find operations takes a total of $O(M
  log N)$ running time.
* Model: Union/Finds in any order Union-by-rank with path compression 41


