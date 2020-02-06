# Lecture 2: C++ Basics (cont.)

## Parameter passing

Many languages, including C++, pass all parameters using **call-by-value**:
the actual argument is copied into the formal parameter. However, this might be
utterly inefficient if large complex objects are being passed since they're
copied in their entirety. Historically C++ has had three ways to pass
parameters:

* **call-by-value:** Useful to pass small objects that shouldn't be mutated by the
function.

```cpp
double average( double a, double b );
double x = 2.5, y = 3.5;
double z = average(x, y); // x and y remain unchanged

void swap( double a, double b ); // swap a and b
swap(x, y); // Not what's expected; x and y remain unchanged
```

* **call-by-reference (call-by-lvalue-reference):**  Useful for all type of
  objects that may be changed by
the function.

```cpp
double x = 2.5, y = 3.5;
swap( &a, &b );
swap(x, y); // now x = 3.5 and y = 2.5
```

* **call-by-constant-reference:**  Useful for large objects that are expensive
to copy and that must not be changed by the function. For this, the parameter
is declared as a reference and the keyword `const` is used to signal that it
cannot be modified.

```cpp
string randomItem( const vector<string> &arr ); // return a random item in arr
```

There's still another fourth way to pass parameters:

* **call-by-rvalue-reference:** Instead of copying a temporary object stored
in an rvalue, a move is used; moving an object's state is easier than copying,
as *it may involve just a simpler pointer change*. Functions know if a value
is temporary or not based on their signature so the primary use for this
type of parameter passing is overloading a function based on whether a parameter
is an lvalue or rvalue.

```cpp
string randomItem( const vector<string>  &arr ); // return random item in lvalue arr (single &)
string randomItem(       vector<string> &&arr ); // return random item in rvalue arr (double &)

vector<string> v = {"hello", "world"};
string x = randomItem(v);                  // invoke lvalue function
string x = randomItem({"hello", "world"}); // invoke rvalue function
```

This idiom is particularly useful for defining the behavor of `=` and in writing
constructors.


## Return passing

In C++, there are several mechanism for returning from a function:

* **return-by-value:** A copy of the object is returned which can be inefficient,
however in C++11 return-by-value may still be efficient for large objects if
the returned objects are rvalues.

```cpp
double average( double a, double b );
LargeType randomItem( const vector<LargeType> &arr ); // potentially inefficient
vector<int> partialSum( const vector<int> &arr );     // efficient in C++11
```

The following are two versions of the function `randomItem`. The second
version avoids the creation of a temporary `LargeType` object, but only if
the caller accesses it with a constant reference:

```cpp
LargeType randomItem1( const vector<LargeType> &arr ) {
	return arr[ randomInt(0, arr.size() -1) ];
}

const LargeType & randomItem2( const vector<LargeType> &arr ) {
	return arr[ randomInt(0, arr.size() -1) ];
}

vector<LargeType> vec;
LargeType item1 = randomItem2(vec);        // copy
LargeType item2 = randomItem2(vec);        // copy
const LargeType &item3 = randomItem2(vec); // no copy
auto &item4 = randomItem2(vec);            // no copy
```

* **return-by-constant-reference:** This avoid creating an immediate copy of 
an object. However, the caller must also use a constant reference to access
the return value; otherwise, there will be still a copy. What does the constant
reference mean? It means that we don't want to allow changes to be made
by the caller by using the return value.

```cpp
const LargeType & randomItem2( const vector<LargeType> &arr ) {
	return arr[ randomInt(0, arr.size() -1) ];
}

vector<LargeType> vec;
const LargeType &item3 = randomItem2(vec); // no copy
```

* **return-by-reference:** A reference is returned and the caller can modify
the returned value. This is used in a few places to allow the caller of a
function to have modifiable access to the internal data representation
of a class.

## C++11 `std::swap` and `std::move`

Given that copying large objects is expensive, C++11 allows the programmer to
easily replace expensive copies with moves provided the object’s class supports
moves.

Take the following example of a `swap` function that swap its arguments by three
copies:

```cpp
void swap( vector<string> &x, vector<string> &y ) {
    vector<string> tmp = x;
    x = y;
    y = tmp;
}
```

In C++11, if the right-hand side of the assignment operator (or constructor) is
an rvalue, then if the object supports moving, we can automatically avoid
copies. In the example above, we know that `vector` supports moving so instead
of copy operations we could do move operations. These could be done either
by casting the right-hand side of an assignment to an rvalue reference or
by using `std::move`.

```cpp
// Using type-casting
void swap( vector<string> &x, vector<string> &y ) {
    vector<string> tmp = static_cast<vector<string> &&>( x );
    x = static_cast<vector<string> &&>( y );
    y = static_cast<vector<string> &&>( tmp );
}

// Using std::move, equivalent to casting but more succint
void swap( vector<string> &x, vector<string> &y ) {
    vector<string> tmp = std::move(x);
    x = std::move(y);
    y = std::move(tmp);
}
```

**NOTE:**  `std::move` doesn't move anything; rather, it makes a value (either
lvalue or rvalue) subject *to be moved*.

It's worth noting that `std::swap` is already part of STL and works for any
part:

```cpp
vector<string> x;
vector<string> y;
std::swap(x, y); // x contains y's contents and y contains x's contents
```

## The big five

In C++, classes come with five special functions already written for each class.
These are the **destructor**, **copy constructor**, **move constructor**,
**copy assignment operator**, and **move assignment operator**. In many cases,
you accept the default behavior provided by the compiler can be relied on. Sometimes
you cannot.

Let's assume the following interface for the `IntCell` class:

```cpp
#ifndef INTCELL_H
#define INTCELL_H

class IntCell {
    public:
        explicit IntCell( int initialValue = 0 ) { value_ = new int{initialValue}; }
        int read() const {}
        void write( int x ) {}
    private:
        int *value_;
};

#endif
```

### Destructor

This function is called whenever an object goes out of scope or is subjected to
a `delete` operation. Typically, the only responsibility of the destructor is
to free up any resources that were acquired during the use of the object.
This includes calling `delete` for any corresponding `new`s, closing any files
that were opened, and so on.

```cpp
IntCell::~IntCell() {
    delete value_;
}
```

### Copy constructor and move constructor

These two constructors are required to construct a new object, initialized
to the same state as another object of the same type.

* If the existing object is an **lvalue**, then it's a **copy constructor**.
* If the existing object is an **rvalue**, then it's a **move constructor**.

```cpp
// Copy constructor, the parameter is an lvalue of the same type.
IntCell::IntCell( const IntCell &rhs ) {
    stored_ = new int{*rhs.value_};
}

// Move constructor, the parameter is an rvalue of the same type.
IntCell::IntCell( IntCell &&rhs ) : value_{rhs.value_} {
    rhs.value_ = nullptr; 
}
```

**When is either constructor called?**

* During a declaration with initialization.

```cpp
IntCell B = C;   // Copy constructor if C is lvalue; Move constructor is C is rvalue
IntCell B { C }; // same as above
```

* An object passed using call-by-value (rarely done).

* An object returned by value.

### Copy assignment and move assignment (operator `=`)

The assignment operator is called when `=` is applied to two objects that have
both been previously constructed. Given the expression `lhs = rhs`, then 
the state of `rhs` (right hand side) is copied into `lhs` (left hand side).

* If rhs is an **lvalue**, this is done using the copy assignment operator.
* If rhs is an **rvalue**, this is done using the move assignment operator.

```cpp
// Copy assignment operator
IntCell & IntCell::operator=( const IntCell &rhs ) {
    if (this &= &rhs) {
        *value_ = *rhs.value_;
    }
    return *this;
}

// Move assignment operator
IntCell & IntCell::operator=( IntCell &&rhs ) {
    std::swap(value_, rhs.value_);
    return *this;
}
```

In C++11, the copy assignment operator could be implemented more idiomatically
with a copy-and-swap idiom:

```cpp
// Copy assignment operator
IntCell & IntCell::operator=( const IntCell &rhs ) {
    IntCell copy = rhs; // calls the copy constructor
    std::swap(*this, copy);
    return *this;
}
```

### Defaults

It's often the case that the defaults "big five"  are perfectly acceptable, 
so nothing need to be done. If a class consists of data members that are
exclusively primitive types and objects for which the defaults make sense, the
class defaults will usually make sense. Thus a class whose data members are `int`,
`double`, `string`, and even `vector<string>` can accept defaults.

#### When do defaults fail?

The defaults fail in a class that contains a data member that is a pointer:

* The default destructor does nothing to data members that are pointers. It's
imperative that we `delete` them ourselves.

* The copy constructor and copy assignment operator both copy the value of the
pointer (i.e., a memory address) rather than the object being pointed at.
This means we end up with two class instances that contain pointers that 
point to the same objects being pointed at. This is known as **shallow copy**.
However, we would typically expect a **deep copy**, in which a clone of the
entire object is created.

> Thus, as a result, when a class contains pointers as data members, and deep
> semantics are important, we typically must implement the destructor, copy
> assignment, and copy constructors ourselves. Doing so removes the move defaults,
> so we also must implement move assignment and the move constructor. As a general
> rule, either you accept the default for all five operations, or you should
> declare all five, and explicitly define, default (use the keyword `default`),
> or disallow each (use the keyword `delete`). Generally we will defined all five.

**NOTE:** If you write any of the big-five, it would be good practice to
explicitly consider all the others, as the defaults may be invalid or
inappropriate. Changing a default method is an all or nothing situation. 

#### When don't the defaults work?

The most common situation in which the defaults do not work occurs when a data
member is a pointer type and the pointer is allocated by some object member
function (such as the constructor). A problem that might arisse is the default
copy assignment and copy constructor doing shallow copies. Another problem
is **memory leak**; an object allocated by a constructor might remain allocated
and it's never reclaimed.

## Templates

An algorithm is **type independent** if the logic of the algorithm does not
depend on the type of items it handles. When we write C++ code for a
type-independent algorithm or data structure, it's preferable to write the code
once rather than recode it for each different type. This is accomplished by
using **templates**.


### Function templates

A **function template** isn't an actual function, but instead a pattern for what
could become a function. For example, the following is a function template:

```cpp
/*
Return the maximum item in array a.
Assumes a.size() > 0.
Comparable object must provide operator< and operator=.
*/

template <typename Comparable>
const Comparable & findMax( const vector<Comparable> &a ) {
    int maxIndex = 0;
    for (int i = 1; i < a.size(); i++) {
        if (a[maxIndex] < a[i]) {
            maxIndex = i;
        }
    }
    return a[maxIndex];
}
```

Using the function template would look as follows:

```cpp
vector<int>    v1 = { 37, 12, 1, 89 };
vector<double> v2 = { 78.1, 89.8, 12.4, 1.1 };
vector<string> v3 = { 'hi', 'ha', 'ho', 'he' };
vector<IntCell> v4(75);

findMax(v1); // OK: Comparable = int
findMax(v2); // OK: Comparable = double
findMax(v3); // OK: Comparable = string
findMax(v4); // Illegal; IntCell doesn't implement operator< and thus not a Comparable
```

Thus, one of the limitations of function templates is that object passed
to the function need to implement whatever operator the function uses
internally. However, instead of relying on "hardcoded" conditions, a function
that perform the required operation could be passed alongside the object.
This type of functions are known as **function objects**.

## Function objects

Previously we implemented `findMax` as a function template but it was only
limited to objects that have an `operator<` function defined. Instead,
we need to rewrite `findMax` to accept as parameters an array of object and a
comparison function that explains how to decide which of two objects is
the larger and which is the smaller. Instead of relying on the array objects
knowing how to compare themselves, we completely decouple this information from
the object in the arrays.

**How do we pass a functions as parameters though?** One way is to define
a class with no data and one member function, and pass an instance of the class.
In fact, the function is passed by placing it inside an object, which is known
as a **function object**.

The following is the simplest implementation of the function object idea:

```cpp
#include <iostream>
#include <vector>
#include <string>

// Generic findMax, with a function object.
// Precondition: a.size() > 0.
// Comparator object is assumed to implement the isLessThan method.
template <typename Object, typename Comparator>
const Object & findMax( const std::vector<Object> &arr, Comparator cmp ) {
    int maxIndex = 0;

    for (int i = 1; i < arr.size(); i++) {
        if (cmp.isLessThan(arr[maxIndex], arr[i])) {
            maxIndex = i;
        }
    }

    return arr[maxIndex];
};

class StringComparisonByLength {
    public:
        bool isLessThan( const std::string &lhs, const std::string &rhs ) {
            return lhs.length() < rhs.length();
        }
};

class StringComparisonCaseInsentitive {
    public:
        bool isLessThan( const std::string &lhs, const std::string &rhs ) {
            return std::strcasecmp(lhs.c_str( ), rhs.c_str()) < 0; 
        }
};

int main() {
    std::vector<std::string> greetings = {"hi", "hello", "bonjour", "HOLA"};
    
    std::cout << findMax( greetings, StringComparisonByLength{} ) << "\n";
    std::cout << findMax( greetings, StringComparisonCaseInsentitive{} ) << "\n";

    return 0;
}
```

C++ function objects are implemented using this basic idea, but with some fancy
syntax. First, instead of using a function with a name, we use operator 
overloading. Instead of the function being `isLessThan`, it is `operator()`.
Second, when invoking `operator()`, `cmp.operator()(x,y)` can be shortened to 
`cmp(x,y)`.Third, we can provide a version of `findMax` that works without
a function object that uses a default ordering; the implementation uses the
Standard Library function object template less (defined in header file
functional) to generate a function object that imposes the normal default
ordering.

The following is the more idiomatic implementation:

```cpp
#include <iostream>
#include <vector>
#include <string>

// Generic findMax, with a function object.
// Precondition: a.size() > 0.
template <typename Object, typename Comparator>
const Object & findMax( const std::vector<Object> &arr, Comparator isLessThan ) {
    int maxIndex = 0;

    for (int i = 1; i < arr.size(); i++) {
        if (isLessThan(arr[maxIndex], arr[i])) {
            maxIndex = i;
        }
    }

    return arr[maxIndex];
};

// Generic findMax, using default ordering.
const Object &findMax( const vector<Object> &arr ) {
    return findMax(arr, less<Object>{} );
}

class StringComparisonByLength {
    public:
        bool operator()( const std::string &lhs, const std::string &rhs ) {
            return lhs.length() < rhs.length();
        }
};

class StringComparisonCaseInsentitive {
    public:
        bool operator()( const std::string &lhs, const std::string &rhs ) {
            return std::strcasecmp(lhs.c_str( ), rhs.c_str()) < 0; 
        }
};

int main() {
    std::vector<std::string> greetings = {"hi", "hello", "bonjour", "HOLA"};
    
    std::cout << findMax( greetings, StringComparisonByLength{} ) << "\n";
    std::cout << findMax( greetings, StringComparisonCaseInsentitive{} ) << "\n";
    std::cout << findMax( greetings ) << "\n";

    return 0;
}
```

### Class templates

In its most simplest form, a class template works much like a function template.
The following `MultiCell` is an implementation that is like `IntCell`, 
but works for any type `Object`, provided that `Object` has a zero-parameter
constructor, a copy constructor, and a copy assignment operator.

```cpp
template <typename  Object>
class MemoryCell {
    public:
        explicit MemoryCell( const Object &initialValue = Object{} ) : _value{initialValue} {}
        const Object & read() const { return _value; }
        void write( const Object &x ) { _value = x; }

    private:
        Object _value;
};
```

Notice that 

* `Object` is passed by constant reference.

* the default parameter for the constructor is not 0, because 0 might not
be a valid Object. Instead, the default parameter is the result of
constructing an Object with its zero-parameter constructor.

> If we implement class templates as a single unit, then there is very little
> syntax baggage. Many class templates are, in fact, implemented this way because,
> currently, separate compilation of templates does not work well on many
> platforms. **Therefore, in many cases, the entire class, with its implementation,
> must be placed in a .h file**. Popular implementations of the STL follow this
> strategy.

## Using matrices

This will be implemented by using a vector of vectors (e.g., a vector of `int`
vectors).

```cpp
#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

template <typename Object>
class Matrix {
    public:
        /*
         * Create _array as having rows entries each of type vector<Object>.
         * We have a rows zero-length vectors of Object so each row is resized
         * to have cols columns. Thus this creates a two-dimensional array.
         */
        Matrix( int rows, int cols ) : _array(rows) {
            for (auto &row : array) {
                row.resize(cols);
            }
        }

        Matrix( std::vector<std::vector<Object>> v ) : _array{ v } { }

        Matrix( std::vector<std::vector<Object>> &&v ) : _array{ std::move(v) } { }

        /*
         * Array indexing operator. This returns the row (a vector<Object> at
         * the index row.
         */
        const std::vector<Object> & operator[]( int row ) const {
            return _array[row]
        }

        int numrows() const {
            return _array.size();
        }

        int numcols() const {
            return numrows() ? _array[0].size() : 0;
        }

    private:
        /*
        A matrix is represented by _array, a vector of vector<Object>.
        */
        std::vector<std::vector<Object>> _array;
};

#endif
```

---

## Class problems

1. What's $1 + 2 + 3 + \ldots + n$? Prove it.

$1 + 2 + 3 + \cdots + n = \frac{n(n+1)}{2}$

**Proof by induction:** 

* **Base case:** When $n = 1$, then $1 = 1(1+1)/2 = 1$.

* **Inductive step:** Since $1 = (1 * 2)/2$, the statement $P(1)$ is true.
Assume that $P(k)$ is true for an arbitrary positive integer $k$. We show
that $P(k+1)$ is true. In other words, 

$$
1 + 2 + 3 + \ldots + (k+1) = \frac{(k+1)(k+2)}{2}
$$

Thus

\begin{align*}
1 + 2 + 3 + \cdots + (k + 1) &= (1 + 2 + 3 + \cdots + k) + (k + 1) \\
&= \frac{k(k+1)}{2} + (k + 1) \\
&= \frac{k(k+1)}{2} + (k + 1) \\
&= \frac{(k+1)(k + 2)}{2} \\
\end{align*}

Therefore, by the principle of mathematical induction, $P(n)$ is true
for every positive integer $n$.

2. What's $1 + 2 + 4 + \cdots + 2^n$? Prove it.

\begin{align*}
n &= 0 => 1                 => 1 \\
n &= 1 => 1 + 2             => 3 \\
n &= 2 => 1 + 2 + 4         => 7 \\
n &= 3 => 1 + 2 + 4 + 8     => 15 \\
\cdots \\
n &= n => 1 + 2 + \cdots + 2^n => 2^{(n+1)} - 1
\end{align*}

Thus, $1 + 2 + 4 + \cdots + 2^n = 2^{(n+1)} - 1$.

**Proof by induction:** 

* **Base case:** When $n = 0$, then $1 = 2^{(0+1)} - 1 = 1$. When $n = 1$, then
$2^0 + 2^1 = 3 = 2^{(1+1)} - 1 = 3$.

* **Inductive step:** Since $1 = 2^{(0+1)} - 1$, the statement $P(1)$ is true.
Assume that $P(k)$ is true for an arbitrary positive integer $k$. We show
that $P(k+1)$ is true. In other words, we show that

$$
1 + 2 + 4 + \cdots + 2^{(k+1)} = 2^{(k+2)} - 1.
$$

Thus,

\begin{align*}
1 + 2 + 4 + \cdots + 2^{(k+1)} &= (1 + 2 + 4 + \cdots + 2^k) + 2^{(k+1)} \\
&= 2^{(k+1)} - 1 + 2^{(k+1)} \\
&= 2^{(k+1)} + 2^{(k+1)} - 1 \\
&= 2 \cdot 2^{(k+1)} - 1 \\
&= 2^{(k+2)} - 1 \\
\end{align*}

Therefore, by the principle of mathematical induction, $P(n)$ is true
for every non-negative integer $n$.

3. If $A_0 = 1$ and $A_n = 2A_{(n-1)} + 1$, what's a closed formula for $A_n$? Prove it.

\begin{align*}
A_1 &= 2 \cdot A_0 + 1     = 3 \\
A_2 &= 2 \cdot A_1 + 1     = 7 \\
A_3 &= 2 \cdot A_2 + 1     = 15 \\
A_4 &= 2 \cdot A_3 + 1     = 31 \\
\cdots \\
A_n &= 2 \cdot A_{(n-1)} + 1 = 2^{(n+1)} - 1 \\
\end{align*}

