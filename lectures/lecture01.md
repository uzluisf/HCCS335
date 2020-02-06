# Lecture 1: C++ basics

## C++ classes

```cpp
class IntCell {
    public:
        // This is a constructor
        IntCell() { _value = 0; }
        
        // This is another constructor. It takes an initial value.
        IntCell( int initialValue ) { _value = initialValue; }

        // This is an accessor (or getter) method.
        int read() { return _value; }

        // This is a setter method.
        void write( int newValue ) { _value = newValue; }

    private:
        int _value;
};

int main() {
    // Using the first constructor w/o parameters
    IntCell x;
    int xValue = x.read(); // 0
    x.write(5);
    xValue = x.read();    // 5
    
    // Using the first constructor with a single parameter
    IntCell y(12);
    int yValue = y.read(); // 12
    y.write(y.read() * 2);
    yValue = y.read();    // 24
}
```

The keywords `public` and `private` determine the visibility of class members. A
member that is `public` may be accessed by any method in any class while a
member that is `private` may only be accessed by methods in the class it's
declared. Date members are usually declared `private` to hide a class's internal
details (i.e., **information hiding**).

## Initialization list

Instead of initializing data members inside a constructor's body,
an **initialization list** can be used to do so right in the constructor's signature:

```cpp
class IntCell {
    public:
        explicit IntCell( int initialValue = 0 ) : _value { initialValue } { }
        int read() const { return _value; }
        void write( int newValue ) { _value = newValue; }

    private:
        int _value;
};
```

By default C++ does behind-the-scenes type conversions in all one-parameter
constructors. Thus, to avoid this the keyword `explicit` is used.

With **implicit type conversion**,

```cpp
IntCell obj;
obj = 37;
```

means

```cpp
IntCell temp = 37;
obj = temp;
```

With **explicit type conversion** (by using `explicit`), a "type mismatch"
compiler error is thrown instead. The use of `explicit` means a one-parameter
constructor cannot be used to generate an implicit temporary object.

## Constant member functions

A member function that examines but does not change the state of its object is
an **accessor**. In the `IntCell`, `read` is an accessor and thus it's marked
explicitly as an accessor by using the keyword `const` after the closing
parenthesis that ends the parameter list. This signals that this method doesn't
change the state of an `IntCell` and if it tries the compiler complains.

## Interface and implementation

An **interface** lists the class and its members (both data and methods) and the
**implementation** provides the implementations of the methods.

In C++, this is done by placing the interface in a `.h` file and the
implementation in a `.cpp` file. Source code that requires knowledge of the
interface must include the interface file (e.g., `#include Interf.h`). To avoid
including files multiple times, a few preprocessor commands are used in the
interface file:

```cpp
#ifndef INTCELL_H // if INTCELL_H is not defined...
#define INTCELL_H // ...define it.

// Here's the IntCell class's interface

#endif            // end the definition
```

### IntCell interface

```cpp
// IntCell.h
#ifndef INTCELL_H
#define INTCELL_H

class IntCell {
    public:
        explicit IntCell( int initialValue = 0 ) {}
        int read() const { return _value; }
        void write( int newValue ) { _value = newValue; }

    private:
        int _value;
};

#endif
```

### IntCell implementation


```cpp
// IntCell.cpp
#include "IntCell.h"  // we're making use IntCell's interf. so we include it.

IntCell::IntCell( int initialValue = ) : _value { initialValue } { }

// Return the stored value.
int IntCell::read() const { return _value; }

// Change the stored value.
void IntCell::write( int newValue ) { _value = newValue; }

};
```

Notice that we must use the **scope resolution operator** (`::`) to identify the
class each member function belongs to. Otherwise, it's assumed that the function
is in the **global scope**. Also note that the signature of an implemented
member function *must* match exactly the signature in the class interface.

### Using IntCell

```cpp
// main.cpp
#include "IntCell.h"

int main() {
	IntCell m{};
	m.write(5);
	int val = m.read(); // 5
	
	return 0;
}
```

To compile the program, run:

```
g++ -o prog main.cpp IntCell.cpp
```

The header files (e.g., `IntCell.h`) aren't listed since they're included in the
implementation files and thus are compiled too.


## C++ Objects 


In C++ objects are declared much like primitive types (e.g., `char`, `int`,
`float`, etc.):

```cpp
IntCell obj1;     // Zero parameter constructor
IntCell obj2(12); // One parameter constructor
```

However, due to an effort to standardize the uniform initialization syntax using
braces, it's recommended to declare objects as follows:

```cpp
IntCell obj1;     // Zero parameter constructor
IntCell obj1{};   // same as before
IntCell obj2{12}; // One parameter constructor
```

## `vector`s and `string`s

The C++ standard defines the classes `vector` and `string` that intend to
replace the built-in C++ array. Unlike arrays, these classes behave like
first-class objects. Thus, they can be copied with `=`, they remember how many
items they can store, and their indexing operator check that the provided valid
(to access some element) is valid.

### `vector`

```cpp
vector<int> numbers = {1, 2, 3, 4, 5}; // Or...
vector<int> numbers {1, 2, 3, 4, 5}; 
```

However, there's some ambiguity with the declaration. For instance, `vector<int>
a(12)` declares a vector that stores 12 integers but `vector<int> a{12}`
declares a vector of size 1 with the value 12 at position 0. This is because C++
gives precedence to the initializer list. If the intention is to declare a
vector of size 12, use the old C++ syntax using parentheses: `vector<int>
a(12)`.

Instead of using the regular for loop, vectors can be looped over using the
following syntax:

```cpp
int sum = 0;
vector<int> numbers = {1, 2, 3, 4, 5};
for( int num : numbers ) {
	sum += num;
}
```

The keyword `auto` can be used to let the compiler determine the type:

```cpp
int sum = 0;
vector<int> numbers = {1, 2, 3, 4, 5};
for( int num : numbers ) {
	sum += num;
}
```

However, keep in mind that this range syntax is only appropriate when 1)
accessing elements sequentially and 2) when the index isn't needed.

## C++ details


### Pointers

A **pointer** is a variable that stores the address of a memory location (i.e.,
where an object resides). The syntax for declaring a pointer is as follows:

```cpp
char* ptr_c;   // ptr_c is a pointer-to-char
int *ptr_i;    // ptr_i is a pointer-to-int
float * ptr_f;  // ptr_f is a pointer-to-float
void *u_ptr;   // ptr_u is a untyped pointer
```

Note that the position of the asterisk `*` (known as the **indirection
operator** in this context) doesn't matter.

In order to obtain an object's address, the **address-of** operator `&` is used
and a pointer variable is used to store it:

```cpp
int num = 5;
int *ptr_num = &num; // get the address of num
int val = *ptr_num;  // get the value stored in the address ptr_num points to
*ptr_num = 6;        // now num is also 6. after all ptr_num to the same memory location
```

Thus, the indirection operator has two jobs: 1) it declares a pointer and 2) it
dereferences a pointer.

### Dynamic object allocation and garbage collection

Objects are created dynamically by allocating memory with `new` which returns a
pointer to the newly created object:

```cpp
IntCell *m;
m = new IntCell(); // OK
m = new IntCell{}; // OK
m = new IntCell;   // OK
```

If a pointer variable points at a class type, then a (visible) member of the
object being pointed at can be accessed via the `->` operator:

```cpp
int x = m->read();
m->write(6);
```

When an object that is allocated by `new` is no longer referenced, the `delete`
operation must be applied to the object (through a pointer). Otherwise, the
memory that it consumes is lost (until the program terminates). This is known as
a **memory leak**.

```cpp
delete m;
```


### Lvalues, and Rvalues

An **lvalue** is an expression that identifies a non-temporary object:

```cpp
vector<int> arr(3);
const int x = 2;
int y;
int z = x + y;
vector<string> *ptr = &arr;
```

The expressions `arr`, `x`, `y`, `z`, `ptr`, `*ptr`,  and `z` are all lvalues.

An **rvalue** is an expression that identifies 1) a temporary object or 2) a
value (e.g., literal constant) not associated with any object:

```cpp
vector<int> arr(3);
const int x = 2;
int y;
int z = x + y;
vector<string> *ptr = &arr;
```

Here, `2`, `x+y`, and `&arr` are all rvalues.

### References

A reference type allows us to define a new name for an existing value. In
classic C++, a reference can generally only be a name for an lvalue, since
having a reference to a temporary would lead to the ability to access an object
that has theoretically been declared as no longer needed, and thus may have had
its resources reclaimed for another object. However, in C++11, we can two types
of references:

* An **lvalue reference** is declared by placing an `&` after some type. An
  lvalue reference then becomes a synonym for the object it references.

```cpp
string str = "hello";
string &str_r = str;
str_r += " world"; // now str is "hello world"

string &lit_r = "hi";      // ILLEGAL: "hi" is not modifiable
string &bad_r = str + "!"; // ILLEGAL: str + "!" isn't an lvalue 
```

* An **rvalue reference** is declared by placing an `&&` after some type. An
  rvalue reference has the same characteristics as an lvalue reference except
  that, unlike an lvalue reference, *an rvalue reference can also reference an
  rvalue* (i.e., a temporary).

```cpp
string str = "hell";
string && bad1 = "hello";            // LEGAL
string && bad2 = str + "";           // LEGAL
string && sub  = str.substr( 0, 4 ); // LEGAL
```

### Uses of lvalue references

* **Aliasing complicated names.** We can rename objects that are too long and
  complicated to simpler names.

```cpp
auto &whichList = theLists[ myhash( x, theLists.size( ) ) ];
if( find( begin( whichList ), end( whichList ), x ) != end( whichList ) ) {
	return false;
}
whichList.push_back( x );
```

Note that simply writing `auto whichList = theLists[ myhash( x, theLists.size( )
) ];` wouldn't work because this would create a copy, and 
`whichList.push_back( x );` would be applied to the copy, not the original.

* **Range `for` loops.** By default, a range `for` loop cannot change the
  elements it iterates over, however taking a lvalue reference allows to modify
  those elements.

```cpp
vector<int> numbers = {1, 2, 3};

for( auto &number : numbers ) {
	number++;
}
```

* **Avoiding a copy.** Given a function that returns an element of an
  array/vector, we could return a non-modifiable reference to that element
  instead of a copy. For instance, instead of

```cpp
// @arr: A non-empty vector of strings.
// @return the maximum string in the @arr.
//  Will abort() if @arris empty.
string FindMax1(constvector<string>&arr) {
	if (arr.empty()) abort();
	int max_index=0;
	for (inti =1; i < arr.size(); ++i) {
		if(arr[max_index]<arr[i]) { max_index = i; }
	}
	return arr[max_index];
}
```

we could have

```cpp
// @arr: A non-empty vector of strings.
// @return the maximum string in the @arr.
//  Will abort() if @arris empty.
const string &FindMax1(constvector<string>&arr) {
	if (arr.empty()) abort();
	int max_index=0;
	for (inti =1; i < arr.size(); ++i) {
		if(arr[max_index]<arr[i]) { max_index = i; }
	}
	return arr[max_index];
}
```

Syntax is needed in function declarations and returns to enable the passing and
returning using references instead of copies. Notice the `const` keyword and
`&` in the function's header.
