# Lecture 5: Lists, Stacks, Queues, and the STL

## Implementation of a vector

The main details of the implementation of the `Vector` class is as follows:

* It'll maintain the primitive array (via a pointer variable to the block of
allocated memory), the array capacity, the current number of stored items.

* It'll implement the "big-five" to provide **deep-copy** semantincs.

* It'll provide the `resize` and `reserve` methods that will change the size of
the `Vector` and the capacity of the `Vector` respectively.

* It'll overload the `operator[]`.

* It'll provide several basic methods, such as `size`, `empty`, `clear`, 
`back`, `pop_back`, and `push_back`.

* It'll provide support for the nested types `iterator` and `const_iterator`,
and associated `begin` and `end` methods.

**Implementation:** [implementations/Vector](https://github.com/uzluisf/HCCS335/blob/master/implementations/cpp/lib/Vector.h)

**Why create a class `Vector`? Is it better than a simple array?** There are few
reasons why creating `Vector` class template makes sense. The main one is that
`Vector` will be a first-clas type, which means that 1) it can be copied, and 
2) the memory it uses can be reclaimed back (via its destructor). This is not
the case for primitie array in C++.

## Implementation of a list

The main details of the implementation of the `List` class template is as
follows:

* The underlying data structure is a **doubly linked list** and thus we must
mantain pointers to both ends of the list. This is in order to mantain constant
time cost per operation, so as long as the operation occurs at a known
position. The known position can be either end or at a position specified by an
iterator.

* Four classes will be provided to better compose the `List` class template:

  * The `List` class itself, which contains links to both ends, the size of the
  list, and several methods.

  * The `Node` class, which is likely to be a private nested class. A node
  contains the data and pointers to the previous and next nodes, along with
  appropriate constructors.

  * The `const_iterator` class, which abstracts the notion of a position, and is
  a public nested class. It stores a pointer to the curren node, a provodes
  implementation of the basic iterator operations.

  * The `iterator` class, which abstracts the notion of a position, and is a
  public nested class. It has similar functionality to `const_iterator`, except
  that `operator*` returns a **reference** to the item being viewed, rather than
  a **constant reference**.

  * Extra nodes are at both the front and end of the list. The node at the front
  represents the beginning marker while the one at the end represents the
  endmarker. These are the **sentinel nodes**; the one at the front is the
  **header node** and the one at the end is the **tail node**. Using the
  sentinels simplify the coding by removing a host of special cases (e.g.,
  removing the first node).

**Implementation:** [implementations/List](https://github.com/uzluisf/HCCS335/blob/master/implementations/cpp/lib/List.h)

![Doubly linked list](./images/node-dll.png "Node and doubly linked list illustration")

### Insertion

![Adding a node](./images/dll-insertion.png "Insertion operation in a DLL")

### Erasure

![Removing a node](./images/dll-erasure.png "Insertion operation in a DLL")

## The Stack ADT

A **stack** (sometimes known as a LIFO (*last in, first out*) list) is a special
list where insertions and deletions can be performed only from the end of the
list (a.k.a., the **top**).

The fundamental operations on a stack are:

push
: Push an item to the top of the stack. Equivalent to `insert`.

pop
: Pop the most recently inserted item. The topmost item can be examined by
using the `top` routine.

![Stack model](./images/stack-model.png "Stack model")

### Implementations

Two population implementations of stacks are:

* **Linked list implementation**

This implementation of a stack uses a singly
linked list. A `push` operation is performed by inserting at the front of the
list, a `pop` operation is performed by deleting the element at the front of the
list, and a `top` operation merely examines the element at the front of the list
by returning its value.

**Implementation:** [implementations/LLStack](https://github.com/uzluisf/HCCS335/blob/master/implementations/cpp/lib/LLStack.h)

* **Array implementation**

This implementation of a stack avoids pointers and
is usually the most popular solution. It uses the `back`, `push_back`, and
`pop_back` implementation from `vector`. Associated with each stack are `array`
to stores the items and `top_of_stack` which is $-1$ for an empty stack.

**Implementation:** [implementations/ArrayStack](https://github.com/uzluisf/HCCS335/blob/master/implementations/cpp/lib/ArrayStack.h)

### Applications

After the array, the stack is probably the most fundamental data structure in
computer science and for this reason, the application of stacks can be found in
many places. Some of them are:

* **Balancing symbols**

Given that compiler check programs for syntax error, a
useful tool to check for the lack of symbols that might cause errors is a
program that checks whether everything is balanced (i.e., every right brace, 
bracket, and parenthesis must correspond to its left counterpart). For instance,
the sequence `[()]` is legal, but `[(])` is wrong. This algorithm uses a stack
and could be described as follows:

> Make an empty stack. Read characters until end of ﬁle. If the character is an
> opening symbol, push it onto the stack. If it is a closing symbol and the stack
> is empty, report an error. Otherwise, pop the stack. If the symbol popped is not
> the corresponding opening symbol, then report an error. At end of ﬁle, if the
> stack is not empty, report an error.

* **Postfix expressions**

This notation, also known as 
[**reverse Polish notation**](https://en.wikipedia.org/wiki/Reverse_Polish_notation),
is a mathematical notation in which operators follow their operands, in
contrast to Polish notation, in which operators
precede their operands. For example, the infix expression
$((15 ÷ (7 − (1 + 1))) × 3) − (2 + (1 + 1))$ can be written in reverse Polish
notation as $15\:7\:1\:1 + − ÷ 3 × 2\:1\:1 + + −$ which yields $5$ regardless of the
direction of the evaluation. An algorithm that uses a stack and that processes
the expression from left to right is described below:

```bash
Input: EXPR is an expression in reverse Polish notation.
Output: EXPR 

EvaluatePostfixExpression( EXPR ):
    stack ← []
    for EXPR → token:
      if token = operator:
          operand_2 ← stack.pop()
          operand_1 ← stack.pop()
          result ← evaluate operator_1 and operand_2 with operator
          stack.push(result)
      else if token = operand:
          stack.push(token)
    return stack.pop()
```

## The Queue ADT

A **queue** (sometimes known as a FIFO (*first in, first out*) list) is a speciallist where insertions are performed at one end and deletions are performed at
the other end. 

The fundamental operations on a queue are:

enqueue
: Insert an element at the end of the list (called the rear).

dequeue
: Delete (and return) the element at the start of the list (called the front).

![Queue model](./images/queue-model.png "Stack model")

### Implementations

Two population implementations of stacks are:

* **Linked list implementation**.

This is implementation is similar to its counterpart stack implementation. 

**Implementation:** [implementations/LLQueue](https://github.com/uzluisf/HCCS335/blob/master/implementations/cpp/lib/LLQueue.h)

* **Array implementation**

For this implementation, we keep an array, `array`,
and the positions `front` and `back`, which represents the ends of the queue.
Likewise, we keep track of the number of items currently in the queue,
`current_size`. To `enqueue` an element `x`, we increment `current_size` and
`back`, then set `_array[back] = x`. To `dequeue` an element, we set the return
value to `array[front]`, decrement `current_size`, and increment `front`.

However there's one potential problem with this implementation. After a certain
number of operations, a queue might appear full since `back` is now at the last
array index, and the next `enqueue` would be in a nonexistent position. A simple
solution would be to wrap around to the beginning whenever `front` or `back`
gets to the end of the array. This is known as a **circular array**
implementation for which modular arithmetic is used.

**Implementation:** [implementations/ArrayQueue](https://github.com/uzluisf/HCCS335/blob/master/implementations/cpp/lib/ArrayQueue.h)

### Applications

There are many algorithms that used queues to give efficient running times. Some
examples are:

* Jobs submitted to a printer are arranged in order of arrival. Thus, they're
essentially placed in a queue.

* Virtually every real-life line is supposed to be a queue. For instance, lines
at ticket counters are queues, because the service is first-come first-served.

* There are many network setups of personal computers in which the disk is attached to one machine, known as the file server. Users on other machines are given access to files on a first-come first-served basis, so the data structure is a queue.

## Trees

Recursively, a **tree** is either empty or consists a root node $r$ and zero
or more non-empty subtrees whose roots are connected by an edge. 

* Used to implement file systems of several popular OSs
* Used to evaluate arithmetic expressions
* Support O(logN) search
* Set and map classes.
