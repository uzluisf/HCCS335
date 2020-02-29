# Lecture 09: Hashing

If keys are small integers, we can use an array to implement an
unordered symbol table (known as **hash table**), by interpreting the
key as an array index so that we can store the value associated with
key $i$ in array entry $i$, ready for immediate access. We can
consider **hashing** (i.e., the implementation of a hash table) an
extension of this simple method that handles more complicated types of keys.
We reference key-value pairs using arrays by doing arithmetic
operations to transform keys into array indices.

Hashing is a technique used for performing insertions,
deletions, and ﬁnds in constant average time. Tree operations that
require any ordering information among the elements are not supported
efﬁciently.

Search algorithms that use hashing consist of two separate parts:

* Computing a **hash function** that transforms the search key into an
  array index. Ideally, different keys would map to different
  indices.

* Resolving collisions via a **collision-resolution process** that
  deals with situations where two or more different keys may hash to
  the same array index. Two different approaches to collision
  resolution are of relative importance: separate chaining and linear
  probing.
  
## Hash functions

If we have an array that can hold $M$ key-value pairs, then we need a
hash function that can transform any given key into an index into that
array. In other words, an integer in the range $[0, M – 1]$.

We seek a hash function that is both *easy to compute* and *uniformly
distributes the keys*: _for each key, every integer between $0$ and
$M – 1$ should be equally likely (independently for every key)._

### Hash function 1

```cpp
int hash( const std::string& key, int tableSize ) {
    int hashVal = 0;
    
    for (char ch :  key) {
        hashVal += ch;
    }
    
    return hashVal % tableSize;
}
```

This hash function is simple to implement and computes an answer
quickly. However if the table size is large, the function doesn't
distribute the keys well. For instance, suppose that the table size is
$10,007$. Suppose all the keys are $8$ or fewer characters long (e.g.,
banana). Given that an ASCII character has an integer value between
$0$ and $127$, the hash function typically can only values between $0$
and $1,016$, which is $127 \times 8$.

### Hash function 2

```cpp
int hash( const std::string& key, int tableSize ) {
    return (key[0] + 27 * key[1] + 27 * 27 * key[2]) % tableSize;
}
```

This function assumes that `key` has at least three characters. The
value $27$ represents the number of letters in the English alphabet,
plus a whitespace. This function examines ony the first three
characters, but if these are random and the table is $10,007$, then
we'd expect a reasonably equitable distribution. Unfornately, English
is not random. Although there are $26^{3} = 17,576$ possible
combination of three characters (ignoring blanks), a check of a
reasonably large dictionary reveals that the number of different legal
combinations is actually only $2,851$. Although easily computable,
this function isn't appropriate if the hash table is reasonably large.

### Hash function 3

```cpp
unsigned int hash( const std::string& key, int tableSize ) {
    unsigned int hashVal = 0;
    
    for (char ch :  key) {
        hashVal = 37 * hashVal + ch;
    }
    
    return hashVal % tableSize;
}
```

This hash function involves all characters in the `key` and can generally be expected to distribute well since it computes

$$
\sum^{keySize - 1}_{i=0} key[keySize - i - 1] \cdot 37^{i}
$$

and brings the result into the proper range. The code computes a
polynomial function (of $37$) by using the
[Horner's rule](https://en.wikipedia.org/wiki/Horner%27s_method).
Another way of computing $h_{k} = k_{0} + 37k_{1} + 37^{2}k_{2}$ is by
the formula $h_{k} = (k_{2} \times 37 + k_{1}) \times 37 + k_{0}$.

Although not necessarily good at distributing the keys, the function
is extremely simple and is reasonably fast. If the keys are very long,
the hash function will take too long to compute. A common practice is
not to use all the key's characters and letting the length and
properties of the key influence the choice. For instance, the keys
might've multiple parts, such as a mailing address, and the function
could take a couple of characters from the stree address and perhaps a
couple of characters from the city name and ZIP code.

## Collision resolution

If, when an element is inserted, it hashes to the same value as an
already inserted element, then we have a collision and need to resolve
it. As mentioned above, there are several methods for dealing with
this. However, the most simple ones are *separate chaining* and *open
addressing*.

### Closed addressing (Separate chaining)

This approach of resolving collisions is to build, for each of the $M$
array indices, a linked list of the key-value pairs whose keys hash to
that index. This method is known as *separate chaining* because items
that collide are chained together in separate linked lists.

The basic idea is to choose $M$ to be sufficiently large that the
lists are sufficiently short to enable efficient search through a
two-step process:

1. Hash to find the list that could contain the key.
2. Sequentially search through that list for the key.

For this section, we assume the keys to be the first $10$ perfect
squares ($0, 1, 4, 9, 16, 25, 36, 49, 64, 81$) and that the hashing
function is simply `hash(x) = x mod 10`. For instance, `hash(81)`
computes $1$ as the array's index; so does `hash(1)`. Thus, they're
chained together.

![Separate chaining hash table](images/separate-chaining-table.png "separate chaining hash table")

**Implementation:** [implementations/HashTable.h](https://github.com/uzluisf/HCCS335/blob/master/implementations/cpp/lib/HashTable.h)

#### Class interface for separate chaining hash table

---

```cpp
template<typename HashedObj>
class HashTable {
    public:
        explicit HashTable( int size = 0 );
        
        bool contains( const HashedObj& x ) const;
        
        void makeEmpty();
        bool insert( const HashedObj& x );
        bool insert( HashedObj&& x );
        bool remove( const HashedObj& x);
       
    private:
        std::vector<std::list<HashedObj>> _lists; // array of lists
        int _current_size;
        
        void _rehash();
        size_t _myhash( const HashedObj& x ) const;
}
```

The hash tables represented by the `HashTable` class works only for
objects that provide a hash function and equality operators.

#### Hash functions

Instead of requiring hash functions that take both the object and the
table size as parameters, we have our hash functions take only the
object as the parameter and return an appropriate integral type.

#### Search

To perform a `search`, we use the hash function to determine which
list to traverse. We then search the appropriate list.

#### Insertion

To perform an `insert`, we check the appropriate list to see whether
the element is already in place (if duplicates are expected, an extra
data member is usually kept, and this data member would be incremented
in the event of a match). If the element turns out to be new, it can
be inserted at the front of the list, since it's convenient and also
because frequently it happens that recently inserted elements are the
most likely to be accessed in the near future.

```cpp
bool insert( const HashedObj& x ) {
    auto& list = _lists[ myhash(x) ];

    if (find(list.begin(),list.end(), x) != list.end()) {
       return false; 
    }
    
    list.push_back(x);
    
    // rehash
    if (++_current_size > _lists.size()) {
        _rehash();
    }
    
    return true;
}
```

#### Separate chaining analysis

First let's start by defining the load factor $\lambda$ as the ration
of the number of elements in the hash table to the table size. The
average length of a list is $\lambda$. The effort required to perform
a search is the constant time required to evaluate the hash
function plus the time to traverse the list.

In an unsuccessful search (i.e., miss), the number of nodes to examine
is $\lambda$ on average. Thus, its average worst-case is $O(1) +
\lambda$. A successful search (i.e., hit) requires that about $1 +
(\lambda/2)$ links to be traversed.

This analysis shows that the table size is not really important but
the load factor is. The general rule for separate chaining hashing is
to make the table size about as large as the number of elements
expected (in other words, let $\lambda \approx 1$).

#### General rules for separate chaining

* Make the table size as large as the number of expected elements.
* In code, $\lambda > 1$, we expand the table size by calling a
  function to **rehash**.
* It's a good idea to keep the table size prome to ensure good
  distribution of the keys.
