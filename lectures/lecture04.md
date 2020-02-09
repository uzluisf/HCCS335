# Lecture 4: Algorithm analysis (cont.) and lists/stacks/queues

## Binary search

> Given an integer $X$ and integers $A_0, A_1, \ldots, A_{N-1}$, which are
> **presorted** already in memory, find index $i$ such that $A_i = X$, or
> return $-1$ if $X$ is not in the group of integers.

[`implementations/binary-search`](https://github.com/uzluisf/HCCS335/tree/master/implementations/binary-search/)
```
Input: array A of elements and item x which we are searching for.
Output: item where item is found or -1 if not found.

BinarySearch( A, x ):
    low ← 0
    high ← A.size - 1
    while low ≤ high:
        mid ← (low + high) / 2
        if A[mid] < x:
            low = mid + 1
        else if A[mid] > x:
            high = mid - 1
        else:
            return mid
    return -1
```

### Running time

We have the following recurrence relation $T(N) = 1 + T(N/2)$ and $T(1) = 1$.
We must solve it to find out the algorithm's running time:

\begin{align*}
T(N) &= 1 + T(N/2) \\
&= 1 + 1 + T(N/2^{2}) \\
&= 1 + 1 + 1 + T(N/2^{3}) \\
\cdots \\
&= k + T(N/2^k) \\
\end{align*}

If $N$ is a power of 2 (i.e., $N = 2^k$ with $k = log(N)$) we'll have:

\begin{align*}
T(N) &= k + T(N/2^k) \\
&= k + T(1) = log(N) + 1 \\
\end{align*}

This means that $T(N) = O(log N)$.

## List/stacks/queue ADTs

An **abstract data type** (ADT) is a set of objects (lists, sets, graphs, etc.)
together with a set of related operations (add, remove, size, etc.). They
provide a template for the objects, not how the objects and their respective set
of operations are implemented.

### The List ADT

We usually deal with a general list of the form $A_0, A_1, A_2, \ldots, A_{N-1}$.
We say that the size of this list is $N$. We will call the special list of
size 0 an **empty list**. The **position** of the element $A_i$ in a list is $i$.

The List ADT could describe the followin operations:

find(x)
: Return the position of the occurrence of item $x$.

insert(x, i)
: Insert some element $x$ at position $i$.

remove(i)
: Remove some element at position $i$.

### Vector in the STL

**Pros:**

* Constant tome indexing
* Fast to add data at the end (not the front).

**Cons:**

* Slow to add data in the middle.
* Inefficient for searches.

#### Operations

* `void push_back(const T &x)` - add `x` at the end of the list.
* `void pop_back()` - remove element at the end of the list.
* `const T &back() const` - return the element at the end of the list.
* `const T &front() const` - return the element at the front of the list.
* `void push_front(const T &x)` - add `x` to the front of the list.
* `void pop_front()` - remove the element at the front of the list.

### List in the STL

**Pros:**

* Implemented as a doubly linked list.
* Fast insertion/removal of items in any position.

**Cons:**

* No indexing.
* Inefficient for searches.

#### Operations

* `T & operator[](int idx)` - return element at index `idx` with no bounds
checking.
* `T &at(int idx)` - return element at index `idx` with bounds checking.
* `int capacity() conts` - return internal capacity of vector`.
* `void reserve(int new_capacity)` - set new capacity and possibly void
expansion of vector.

## Containers

A **container** is a holder object that stores a collection of other objects. 
This is usually implemented as a class templates which provides it with great
flexibility for the types supported as elements.

A container

* manages the storage space for its elements and provides member functions to
access them either directly or through **iterators**.

* replicates structures commonly used in programming such as dynamic arrays
(e.g., vectors), queues, stacks, heaps (e.g., priority queues), linked lists,
trees (e.g., sets), associative arrays (e.g., maps), etc.

## Iterators

Some operations on lists require the notion of a position. In the STL, a
position is represented by some nested type known as an **iterator**. In
particular, for a `list<string>`, the position is represented by the type
`list<string>::iterator`; for a `vector<int>`, `Vector<int>::iterator`.

### How to get an iterator

The STL lists (and all other STL containers) define a pair of methods:

* `iterator begin()`: returns an appropriate iterator representing the first
item in the container.

* `iterator end()`: returns an appropriate iterator representing the endmarker
in the container. This endmarker is "out-of-bounds".

### Iterator methods

* `itr++` and `++itr`:  advances the iterator `itr` to the next location.

* `*itr`: returns a reference, which may or may not be modifiable, to the object
  stored at iterator `itr`'s location.

* `itr1 == itr2`: returns true if iterators `itr1` and `itr2` refer to the same
location and false otherwise.

* `itr1 != itr2`: returns true if iterators `itr1` and `itr2` refer to a
different location and false otherwise.

For example the code:

```cpp
for (int i = 0; i <!= v.size(); i++) {
    std::cout << v[i] << "\n";
}
```

could be rewritten as follows using iterators:

```cpp
for (vector<int>::iterator itr = v.begin(); itr != v.end(); itr++) {
    std::cout << *itr << "\n";
}
```

Alternatively:

```cpp
vector<int>::iterator itr = v.begin();
while (itr != v.end()) {
    std::cout << *itr++ << "\n";
}
```

### Container operations that require iterators

* `iterator insert( iterator pos, const T & x )`: adds `x` into the list, prior
to the position by the iterator `pos`. This is a constant operator for `list`,
but not for `vector`. The return value is an iterator representing the position
of the inserted item.

* `iterator erase( iterator pos )`: removes the object at position and return
the position of the element that followed `pos` prior to the call. It
invalidates `pos`, making it stale.

* `iterator erase( iterator start, iterator pos )`:  removes all items beginning
at position up to, but not including `end`. An entire list `c` can be erased by 
`c.erase( c.begin(), c.end() )`.

### Example: Using `erase` on a list

```cpp
/*
 * @brief Deletes every other element from lst, starting from the first item.
 * @param lst a list, or any object that supports iterators and erase.
 */
template <typename Container>
void removeEveryOtherItem( Container &lst ) {
    typename Container::iterator itr = lst.begin(); // could be shortened to 
                                                    // auto itr = lst.begin();
    while (itr != lst.end()) {
        itr = lst.erase(itr);
        if (itr != lst.end()) itr++;
    }
}
```

### `const_iterators`

The result of `*itr` is both the value of the item that the iterator is viewing
but also the item itself. This is very powerful but also introduces some
complications.

Let's analyze the following routine that works for both `vector` and `list` and
runs in linear time:

```cpp
template <typename Container, typename Object>
void change( Container &c, const Object &newValue ) {
    typename Container::iterator itr = c.begin();
    while (itr != c.end()) {
        *itr = newValue; // set current iterator's value to newValue
        itr++;           // advance the pointer to next item
    }
}
```

The potential problem arises if `Container c` was passed to a routine using
call-by-constant reference, meaning we would expect that not changes would be
allowed to `c`, and the compiler would ensure this by nota allowing calls to any
`c`'s mutators. For example, consider the following code that prints a `list` of
integers but also tries to do some changes:

```
void print( const list<int> &lst, ostream &out = cout ) {
    typename Container::iterator itr = lst.begin();
    while (itr != lst.end()) {
        out << *itr << "\n";
        *itr = 0;   // <= This is the suspect.
        itr++;
    }
}
```

The solution provided by the STL is that every collection contains not only an
`iterator` nested type but also `const_iterator` nested type. The main
difference between them is that `operator*` for `const_iterator` returns a
constant reference, and thus `itr*` for a `const_iterator` cannot appear on the
left-hand side of an assignment statement. 

Further the compiler will force you to use a `const_iterator` to traverse a
*constant* collection and does so by providing two versions versions of `begin`
and two versions of `end`:

* `iterator begin()`
* `const_iterator begin() const`
* `iterator end)`
* `const_iterator end() const`

**Note:** The two versions of `begin` can be in the same class only because
of the const-ness of a method (whether an accessor or mutator) is
considered to be part of the signature. The trick is overloading `operator[]`.


Using `auto` to declare your iterators means the compiler will deduce for you
whether an `iterator` or `const_iterator` is substituted. This exempt the programmer 
from having to keep track of the correct iterator type and is precisely one
of the intended uses of `auto`.

### Printing a container


```cpp
/*
 * @brief Prints out the container on the output stream.
 * @param c a given container.
 * @param out an output stream.
 */
template <typename Container>
void print( const Container &c, ostream &out = cout ) {
    if (c.empty()) {
        out << "(empty)";
    }
    else {
        typename Container::iterator itr = begin(c); 
        out << "[" << *itr++; // print first item
        while (itr != end(c)) {
            out << ", " << *itr++;
        }
        out << "]" << "\n";
    }
}
```

