# Lecture 10: Hashing (cont.)

## Collision resolution

### Open addressing

Another approach to implementing hashing is to store $N$ key-value
pairs in a hash table of size $M > N$, relying on empty entries in the
table to help with collision resolution. Such methods are called
*open-addressing* hashing methods.

This approach still uses linked lists, however unlike separate
chaining, it tries alternative cells until an empty cell is
found. More formally, cells $h_{0}(x), h_{1}(x), h_{2}(x), \ldots$ are
tried in succession, where $h_{i}(x) = (hash(x) + f(i)) mod
TableSize$, with $f(0) = 0$. The function, $f$, is the collision
resolution strategy. Because all the data go inside the table, a
bigger table is needed in such a scheme than for separate chaining
hashing. Generally, the load factor should be below $\lambda = 0.5$
for a hash table that doesn’t use separate chaining.

The simplest open-addressing method is called *linear probing*. 

#### Linear probing

When there is a collision (when we hash to a table index that is
already occupied with a key different from the search key), then we
just check the next entry in the table (by incrementing the
index). Linear probing is characterized by identifying three possible
outcomes:

* Key equal to search key (i.e., search hit)
* Empty position (i.e., search miss)
* Key not equal to search key (i.e., try next entry)

We hash the key to a table index, check whether the search key matches
the key there, and continue (incrementing the index, wrapping back to
the beginning of the table if we reach the end) until finding either
the search key or an empty table entry. It is customary to refer to
the operation of determining whether or not a given table entry holds
an item whose key is equal to the search key as a **probe**.

![Linear probing](images/hash-table-linear-probing.png "linear probing")

As long as the table is big enough, a free cell can always be found,
but the time to do so can get quite large. Worse, even if the table is
relatively empty, blocks of occupied cells start forming. This effect,
known as **primary clustering**, means that any key that hashes into the
cluster will require several attempts to resolve the collision, and
then it will add to the cluster.

The expected number of probes using linear probing is roughly
$\frac{1}{2}(1 + 1/(1-\lambda)^{2})$ for insertions and unsuccessful
searches, and $\frac{1}{2}(1 + 1/(1-\lambda))$.

**Random collision resolution:** Assume a huge table where clustering
is not an issue and each probe is indepedent of the previous probes,
then the expected number of probes in miss equals the expected number
of probes to fin an empty cell, that is, $1/(1-\lambda)$.

*Proof:* The probability of selecting an empty cell equals $1-\lambda = p$
(i.e., probability of success). The probability of selecting a
non-empty cell is $\lambda = 1-p$ (i.e., probability of
failure). Finding an empty cell is like flipping a coin $N$ times
until success (assume coin is biaded having probability $p$ of
selecting success). The number of probes is thus a random variable $X$
having a Geometric Probability Distribution. The expected value of $X$
is thus $1/\text{Prob. of success} = 1/(1-\lambda)$.


#### Quadratic probing

Quadratic probing is a collision resolution method that eliminates the
primary clustering problem of linear probing. Quadratic probing is
what you would expect -- the collision function is quadratic. The popular
choice is $f(i) = i^2$.

![Quadratic probing](images/hash-table-quadratic-probing.png "quadratic probing")

* `insert(89)`
  * $h_{0}(89) = ((89 + 0^{2}) \text{ mod } 10) = 9$. Cell $9$ unoccupied so insert $89$.
* `insert(18)`
  * $h_{0}(18) = ((18 + 0^{2}) \text{ mod } 10) = 8$. Cell $8$ unoccupied so insert $18$.
* `insert(49)`
  * $h_{0}(49) = ((49 + 0^{2}) \text{ mod } 10) = 9$. Cell $9$ occupied.
  * $h_{1}(49) = ((49 + 1^{2}) \text{ mod } 10) = 0$. Cell $0$ unoccupied so insert $49$.
* `insert(58)`
  * $h_{0}(58) = ((58 + 0^{2}) \text{ mod } 10) = 8$. Cell $8$ occupied.
  * $h_{1}(58) = ((58 + 1^{2}) \text{ mod } 10) = 9$. Cell $9$ occupied.
  * $h_{2}(58) = ((58 + 2^{2}) \text{ mod } 10) = 2$. Cell $2$ unoccupied so insert $58$.
* `insert(69)`
  * $h_{0}(69) = ((69 + 0^{2}) \text{ mod } 10) = 9$. Cell $9$ occupied.
  * $h_{1}(69) = ((69 + 1^{2}) \text{ mod } 10) = 0$. Cell $0$ occupied.
  * $h_{2}(69) = ((69 + 2^{2}) \text{ mod } 10) = 3$. Cell $3$ unoccupied so insert $69$.

For linear probing, it is a bad idea to let the hash table get nearly
full, because per- formance degrades. For quadratic probing, the
situation is even more drastic: There is no guarantee of ﬁnding an
empty cell once the table gets more than half full, or even before the
table gets half full if the table size is not prime. This is because
at most half of the table can be used as alternative locations to
resolve collisions.

**Theorem:** If quadratic probing is used, and the table size is
prime, then a new element can always be inserted if the table is at
least half empty.

*Proof:* Let $T$ be the table size and odd prime greater than $3$. We
show that the first $\lceil T/2\rceil$ alternative locations
(including the initial location $h_{0}(x)$) are all distinct. Two of
these locations are $h(x) + i^{2} (\text{mod } T)$ and $h(x) + j^{2} (\text{mod } T)$,
where $0 \leq i$ and $j \leq \lfloor T/2\rfloor$. Suppose, for the
sake of contradiction, that these locations are the same, but $i \neq
j$. Then

\begin{align*}
h(x) + i^{2} &= h(x) + j^{2} \quad (\text{mod } T) \\
i^{2} &= j^{2} \quad (\text{mod } T) \\
i^{2} - j^{2} &= 0 (\text{mod } T) \\
(i - j)(i + j) &= 0 (\text{mod } T) \\
\end{align*}

Since $T$ is prime, it follows that either $(i-j)$ or $(i+j)$ is equal
to $0$ (mod $T$). Since $i$ and $j$ are distinct, the first option is
not possible. Since $0 \leq i$ and $j \leq \lfloor T/2 \rfloor$, the
second option is also impossible. Thus, the first $\lceil T /2 \rceil$
alternative locations are distinct. If at most $\lfloor T/2 \rfloor$
positions are taken, then an empty spot can always be found.

## Applications

When $log(N)$ is too big...

* Symbol tables in interpreters/compilers
* Real-time databases (in main memory or disk)
    * Air traffic control
    * Packet routing
    * Graphs where nodes are strings
    * Password checking
* Spell-checkers

When associative memory is needed...

* Dynamic programming
    * Cache results of previous computation
    * Chess endgames
* Text processing applications

## Hash tables summary

* Used to index large amounts of data.
* Array index of each key is calculated using the key itself.
* Collisions between keys that hash to the same index are resolved
  using some collision resolution strategy (e.g., closed addressing,
  open addressing, etc.).
* Insertion, deletion and search occur at constant time.
* Hashing is widely used in database indexing, compilers, caching,
  password authentication, etc.
