# Lecture 11: Hashing (cont.)

## Double hashing

For the collision resolution method known as **double hashing**, one popular
choice is $f(i) = i\cdot hash_{2}(x)$ which says that we apply a second hash
function to $x$ and probe at a distance $hash_{2}(x)$, 2hash_{2}(x), \ldots,$
and so on. A poor choice of $hash_{2}(x)$ would be disastrous. A few things
to ensure:

* The function must never evaluate to zero. It is also important to make sure 
* All cells can be probed. A function such as $hash_{2}(x) = R − (x mod R)$,
with $R$ a prime smaller than the table size $T$, will work well.

If the table size is not prime, it's possible to run out of alternative
locations prematurely. However if double hashing is correctly implemented,
simulations imply that the expected number of probes is almost the same as for
a random collision resolution strategy which makes double hashing theoretically
interesting.

Nevertheless, *quadratic probing does not require the use of a second hash
function* and is thus likely to be simpler and faster in practice, especially for
keys like strings whose hash functions are expensive to compute.

## Rehashing

If the table gets too full:

* The running time for the operations will start taking too long.
* Insertions might fail for open addressing hashing with quadratic resolution.
  This can happen if there are too many removals intermixed with insertions.

**Solution:** Build another table that is about twice as big (e.g., $70\%$ full)
(with an associated new hash function) and scan down the entire original hash
table, computing the new hash value for each (nondeleted) element and inserting
it in the new table.

### Example

Suppose the elements $13, 15, 24$, and $6$ are inserted into a linear probing hash
table of size 7. The hash function is $h(x) = x \text{ mod } 7$ where $x$ is the
key (e.g., $h(13) = 6$, $h(15) = 1$, etc.). The resulting table is:

![Hash table with linear probing with input $13, 15, 6, 24$](images/rehashing-example-01.png "reshahing example 01")

If $23$ is inserted into the table, the resulting table will be over 70 percent
full:

![Hash table with linear probing after $23$ is inserted](images/rehashing-example-02.png "reshahing example 02")

Because the table is so full, a new table is created. The size of this table is
$17$, because this is the ﬁrst prime that is twice as large as the old table
size. The new hash function is then $h(x) = x \text{ mod } 17$. The old
table is scanned, and elements $6, 15, 23, 24$, and $13$ are inserted into
the new table:

![Hash table after reshahing](images/rehashing-example-03.png "reshahing example 03")

Rehashing can be implemented in several ways with quadratic probing. Some
alternatives are:

* Rehash as soon as the table is half full.
* Rehash only when an insertion fails.
* A third, middle-of-the-road strategy is to rehash when the table reaches a
  certain load factor. Since performance does degrade as the load factor
  increases, the third strategy, implemented with a good cutoff, could be best.

Rehashing for separate chaining hash tables is similar.

## STL's `unordered_set` and `unordered_map`

* Hash table implementation of sets and maps.
* Same functionality as `set` and `map`, but no ordered capabilities:
   * Items in `unordered_set` (or keys in `unordered_map`) must provide an overloaded
   `operator==` and a hash function.
   * `unordered_set` and `unordered_map` can be instantiated with
     function objects that provide a hash function and equality operator.

In the word-changing example 
([Lecture 8](https://github.com/uzluisf/HCCS335/blob/master/lectures/lecture08.md)),
there were three maps:

1. A map in which the key is a *word length*, and the value is a collection of
   all words of that word length. Because the order in which word lengths are
   processed does not matter, the map an `unordered_map`.

2. A map in which the key is a *representative*, and the value is a collection of
   all words with that representative. Because the representatives are not even
   needed after the second map is built, the map can be an `unordered_map`.

3. A map in which the key is a *word*, and the value is a collection of all words
   that differ in only one character from that word. This map can also be an
   `unordered_map`.

The performance of an `unordered_map` can often be superior to a map, but it is
hard to know for sure without writing the code both ways.

## Hash Tables with Worst-Case `O(1)` Access

For separate chaining, assuming a *load factor* of 1, this is one version of
the classic **balls and bins problem**:

> Given $N$ balls placed randomly (uniformly) in $N$ bins, what is the expected
> number of balls in the most occupied bin?

The answer is well known to be $\Theta(logN/loglog N)$, meaning that on average,
we expect some queries to take nearly logarithmic time. Similar types of bounds
are observed (or provable) for the length of the longest expected probe sequence
in a probing hash table.

We would like to obtain $O(1)$ worst-case cost. In some applications, such as
hardware implementations of lookup tables for routers and memory caches, it is
especially important that the search have a deﬁnite (i.e., constant) amount of
completion time. Let us assume that $N$ is known in advance, so no rehashing is
needed. If we are allowed to rearrange items as they are inserted, then $O(1)$
worst-case cost is achievable for searches.

## Extendible hashing

Extendible hashing deals with the cases where the amount of data is too large
(i.e., the hash table is huge) to fit in main memory. We assume that at any
point we have $N$ records to store; the value $N$ changes over time.
Furthermore, at most $M$ records fit in one disk block and $M < N$. 

With either probing hashing or separate chaining hashing, we have the following
problems: 

* collisions could cause several blocks to be examined during a
search, even for a well-distributed hash table.

* when the table gets too full, an extremely expensive rehashing step must be
  performed, which requires $O(N)$ disk accesses.

The alternative solution is **extendible hashing** which allows a search to be
performed in two disk accesses. Insertions also require few disk accesses.

### B-tree approach for extendible hashing

Recall that a B-tree has depth $O(log_{M/2}N)$; as $M$ increases, the depth
of a B-tree decreases. In theory, we could $M$ to sufficiently large for 
the depth of the B-tree to become $1$. Then any search after the first would take
one disk access, since presumably the root node could be stored in main memory.
However, the problem is that the *branching factor* is so high that it would
take considerable processing to determine the leaf in which the data is in.
By reducing the time to perform this step, a practical scheme can be reached;
this is the strategy extendible hashing uses.

Let's suppose our data consists of several $6$-bit integers:

![Extendible hashing, original data](images/extend-hashing-original.png "extend
hashing original data")

The root of the tree contains $4$ pointers determined by the leading two bits of
the data. Each leaf has up to $M = 4$ elements; in each leaf the first two bits
are identical for all integers. We can let $D$ be the number of bits used by the
root, usually known as the **directory**. The number of entries in the directory
is thus $2^{D}$. For a leaf $L$, $d_{L}$ is the number of leading bits that are
common to all elements for the given leaf; $d_{L}$ will depend on the particular
leaf, and $d_{L} \leq D$.

Suppose we want to insert the key $100100$. This would be placed in the third
leaf (since its two leading bits are $10$), but the third leaf is already full.
We thus split this third leaf into two leaves, which are now determined by the 
first $d_{L} = 3$ bits for these two leaves. This requires increasing the
directory size to $3$ and the number of entries in the directory is now $2^{3} =
8$:

![Extendible hashing, insertion and directory split](images/extend-hashing-original-change01.png "extend hashing original data change 01")

Notice that all leaves not involved in the split are now pointed to by two
adjacent directory entries (e.g., first leaf is pointed to by $000$ and $001$).
Thus, although an entire directory is rewritten, none of the other leaves are
accessed.

Now suppose the key $000000$ is inserted. Then the first leaf is split,
generating two leaves with $d_{L} = 3$. Since $D = 3$, the only change required
in the directory is the updating of the $000$ and $001$ pointers:

![Extendible hashing, insertion and leaf split](images/extend-hashing-original-change02.png "extend hashing original data change 02")

All in all, this simple strategy provides quick access times for `insert` and
search operations on large databases.

There are a few important details to consider:

1. It's possible that several directory splits will be required if the elements
   in a leaf agree in more than $D + 1$ leading bits.

2. There's the possibility of duplicate keys; if there are more than $M$
   duplicates, then this algorithm doesn't work at all.

These possibilities suggest that it is important for the bits to be fairly
random. This can be accomplished by hashing the keys into a reasonably long
integer—hence the name of *extendible* hashing.

### Perfomance properties

These properties are derived after a very difﬁcult analysis. These results are
based on the reasonable assumption that the bit patterns are uniformly
distributed.

* The expected number of leaves is $(N/M)log_{2}e$. Thus the average leaf is $ln
2 = 0.69$ full. This is the same as for B-trees, which is not entirely
surprising,since for both data structures new nodes are created when the
$(M+1)$th entry is added.

* The expected size of the directory (i.e., $2^D)$ is $O(N^{1 + 1/M}/M)$. If
$M$ is very small, then the directory can get unduly lage. In this case, we can
have the leaves contain pointers to the records instead of the actual records,
thus increasing the value of $M$. This adds a second disk access to each search
operation in order to maintain a smaller directory. If the directory is too
large to ﬁt in main memory, the second disk access would be needed anyway.

## Hashing summary

* Constant average for insert/search operations.
* Load factor $\lambda$ is crucial:
  * roughly $1$ for separate chaining
  * less than $0.5$ for probing
  * Can change with rehashing (an expensive operation)
* BSTs could also be used
* If no ordering is required, hash table `set`/`map` is probably better, although
  both must be tried to decide which is better.
