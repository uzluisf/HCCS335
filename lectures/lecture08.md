# Lecture 08: Sets and maps

## STL containers

The *vector* and *list* containers are inefficient for search and
insert. Thus, the STL provides the *set* and *map* containers which
guarantee logarithmic time for insertion, deletion and searching.

## The set container

A *set* is an ordered container that doesn't allow duplicates:

* It stores objects of type `Key` in sorted order.
* The `Value` is the `Key` itself, without any additional data.

### Insertion

The `insert` method returns an `iterator` which represents the
position of the newly inserted item or the existing item that caused
the insertion to fail (i.e., no duplicates).

The STL deﬁnes a class template called `pair` that is little more than a
struct with members first and second to access the two items in the
pair. There are two different `insert` routines: 

* `pair<iterator,bool> insert( const Object & x );`
* `pair<iterator,bool> insert( iterator hint, const Object & x );`

`pair<T1, T2` is a heterogenous pair; it holds one object of type `T1`
and another of type `T2`. For example:

```cpp
pair<bool, double> result; // a pair with key as boolean and value as double
result.first = true; // the method first access the pair's key
result.second = 2.5; // the method second accesses the pair's value
```

### Insertion with hint

The two-parameter `insert` allows the speciﬁcation of a hint, which represents the
position where `x` should go. If the hint is accurate, the insertion is
fast, often $O(1)$. If not, the insertion is done using the normal
insertion algorithm and performs comparably with the one-parameter
insert. For instance, the following code might be faster using the
two-parameter `insert` rather than the one-parameter `insert`:

```cpp
set<int> s;
for (int i = 0; i < 10; i++) {
	const auto result = s.insert(s.end(), i);
}
```

### erase

There are several versions of `erase`:

* `int erase( const Object & x );`: This version removes `x` (if
  found) and returns the number of items actually removed, which is
  obviously either 0 or 1.
* `iterator erase( iterator itr );`: This one behaves the same as in
  `vector` and `list`, and it removes the object at the position given
  by the iterator, returns an iterator representing the element that
  followed `itr` immediately prior to the call to erase, and
  invalidates `itr`, which becomes stale.

* `iterator erase( iterator start, iterator end );`: This one behaves
  the same as in a `vector` or `list`, removing all the items starting
  at `start`, up to but not including the item at `end`.
  
### find

The set provides a `find` routine that returns an iterator
representing the location of the item (or the endmarker if the search
fails). This provides considerably more information, at no cost in
running time. The signature of `find` is iterator `find( const Object & x
) const;`.

By default, ordering uses the `less<Object>` function object, which
itself is implemented by invoking `operator<` for the `Object`. An
alternative ordering can be speciﬁed by instantiating the `set`
template with a function object type. For instance, we can create a
`set` that stores string objects, ignoring case distinctions by using
the `CaseInsensitiveCompare` function object:

```cpp
class CaseInsensitiveCompare {
	public:
		bool operator( )( const string & lhs, const string & rhs ) const {
			return strcasecmp( lhs.c_str( ), rhs.c_str( ) ) < 0;
		}
};

set<string,CaseInsensitiveCompare> s;
s.insert("Hello");
s.insert("HeLLo");
std::cout << "The size is: " << s.size( ) << std::endl; // size is 1
```

## The map container

A `map` is used to store a collection of ordered entries that consists of keys and their values:

* Keys must be unique, but several keys can map to the same values. Thus values need not be unique.
* The keys in the `map` are maintained in logically sorted order.

The `map` also supports `insert`, `find`, `erase`, `begin`, `end`,
`size`, and `empty`. For `insert`, one must provide a
`pair<KeyType,ValueType>` object. Although `find` requires only a key,
the `iterator` it returns references a `pair`.

There's another operation that yields simple syntax. The
array-indexing operator is overloaded for maps as follows:

```cpp
ValueType& operator[] ( const KeyType & key );
```

The semantics of `operator[]` are as follows:

* If `key` is present in the `map`, a reference to the corresponding value
is returned. 

* If `key` is not present in the map, it is inserted with a default value
into the map and then a reference to the inserted default value is
returned. The default value is obtained by applying a zero-parameter constructor
or is zero for the primitive types.

These semantics do not allow an accessor version of `operator[]`,
so `operator[]` cannot be used on a `map` that is constant. For instance,
if a map is passed by constant reference, inside the routine,
`operator[]` is unusable.

### Example

```cpp
#include <iostream>
#include <string>
#include <map>

int main() {
  std::map<std::string, double> salaries;

  salaries["Pat"] = 78000.0;

  std::cout << salaries["Pat"] << std::endl;
  std::cout << salaries["Per"] << std::endl;

  std::map<std::string, double>::const_iterator itr;
  itr = salaries.find("Paul");

  if (itr == salaries.end()) {
    std::cout << "Not an employee" << std::endl;
  }
  else {
    std::cout << itr->second << std::endl;
  }

  return 0;
}
```

## Implementation of set/map in C++

C++ requires that set and map support the basic `insert`, `erase`, and
`find` operations in logarithmic worst-case time. The underlying
implementation is a **balanced binary search tree**. 

An important issue in implementing `set` and `map` is providing support
for the `iterator` classes. Of course, internally, the `iterator`
maintains a pointer to the “current” node in the iteration. The hard
part is efﬁciently advancing to the next node. There are several
possible solutions, but the smart solution is to use **threaded** trees
which are used throughout different implementations in the STL.

This solution involves mantaining the extra links only for nodes that
have `nullptr` left or right links by using extra `Boolean` variables to
allow the routines to tell if a left link is being used as a standard
binary search tree left link or a link to the next smaller node, and
similarly for the right link.

### Threaded binary trees

A binary tree is threaded by

* making all right child pointers (that would normally be `nullptr`)
  point to the in-order successor of the node (if it exists), and

* making all left child pointers (that would normally be `nullptr`)
  point to the in-order predecessor of the node.
 
![Threaded tree, special threaded links are dashed arrows](images/threaded-binary-tree.png "threaded tree")

In this type of tree, we have the pointers reference the next node in
a in-order traversal. These are known as threads.

We need to know if a pointer is an actual link or a thread, so 
we keep a boolean for each pointer.

Why do we need threaded binary trees?

* Binary trees have a lot of wasted space.
* Threaded binary trees make the tree traversal faster since we don't
  need a stack or use recursion for traversal.
  
### Types of threaded binary trees

* **Single threaded:** Each node is threaded towards either the
  in-order predecessor o successor (left or right). This means all
  right null pointers will point to the in-order successor **or** all left
  null pointers will point to the in-order predecessor.
  
![Single threaded binary tree](images/single-threaded-tree.png "single threaded tree")

* **Double threaded:** Each node is threaded towards both the in-order
  predecessor and successor (left and right). This means that all
  right null pointers will point to the in-order successor **and** all
  left null pointers will point to the in-order predecessor.

![Double threaded binary tree](images/double-threaded-tree.png "double threaded tree")

### An example

We would like to write a program to ﬁnd all words that can be changed
into at least 15 other words by a single one-character
substitution. We assume that we have a dictionary consisting of
approximately 89,000 different words of varying lengths. Most words
are between 6 and 11 characters. 

**First approach:** The most straightforward strategy is to use a `map`
in which the keys are words and the values are vectors containing the
words that can be changed from the key with a one-character substitution.

```cpp
/*
@brief The function prints the contents of the map only for the elements
for which the vector of strings has size greater than or equal to min_words.
@param adjacentWords input map from string to vector of strings.
@param min_words minimu number of words to consider printing.
*/
void print_high_changeables(
	const std::mapt<std::string>, std::vector<std::vector>> adjacent_words,
	int min_words = 15
) {
	for (auto& entry : adjacent_words) {
		const std::vector<std::string>& words = entry.second;
		if (words.size() >= min_ords) {
			std::cout << entry.first << "(" << words.size() << "): ";
			for (auto& str : words) {
				std::cout << " " << str << std::endl;
			}
		}
	}
}
```

The function above only shows how to print the the `map` that is
eventually produced. How do we construct the `map` though?

One way is to test if two words are identicall except for a
one-character substitution. Then, we can brute-force our way testing
all pairs of words and constructing the `map`. If we find a pair of
words that differ in only one character, we can update the `map` by
adding this new word (that differs by a single character) to the `map`.

```cpp
/*
@brief Check if two words differ by a single character.
@return bool
*/
bool nearly_equal( const std::string& word1, const std::string& word2 ) {
	if (word1.length() != word2.length()) return false;
	int diffs = 0;
	for (int i = 0; i < word1.length(); i++) {
		if (word1[i] != word2[i]) {
			if (++diffs > 1) return false;
		}
	}
	return diffs == 1;
}

std::map<std::string, std::vector<std::string>> compute_adjacent_words(
	const std::vector<std::string>& words
) {
	std::map<std::string, std::vector<std::string>> adj_words;
	for (int i = 0; i < words.size(); i++) {
		for (int j = i; j < words.size(); j++) {
			adj_words[ words[i] ].push_back(words[j]);
			adj_words[ words[j] ].push_back(words[i]);
		}
	}
	return adj_words;
}
```

**Second approach:**

The problem with the algorithm above is that it's slow. An obvious
improvement is to avoid comparing words of different lenghts. We can
do this by grouping words by their length, and then running the
previous algorithm on each of the separate groups. To do this, we can
use a second `map` where the key is an integer representing a word length,
and the value is a collection of all words of that length.

```cpp
/*
@brief Compute a map in which the keys are words and values are
vectors of words that differ in only one character from the
corresponding key. Uses a quadratic algorithm, but speeds things
up a little by maintaining an additional map that groups words
by their length.
*/
std::map<std::string, std::vector<std::string>> compute_adjacent_words(
	const std::vector<std::string>& words
) {
	std::map<std::string, std::vector<std::string>> adj_words;
	std::map<int, std::vector<std::string>> words_by_length;
	
	// group the words by their length
	for (auto& word : words) {
		words_by_length[ word.length() ].push_back(word);
	}
	
	// work on each group separately
	for (auto& entry : words_by_length) {
		const std::vector<std::string>& groups_words = entry.second;
		
		for (int i = 0; i < groups_words.size(); i++) {
			for (int j = i + 1; j < groups_words.size(); j++) {
				if (nearly_equal(groups_words[i], groups_words[j])) {
					adj_words[ groups_words[i] ].push_back( groups_words[j] );
					adj_words[ groups_words[j] ].push_back( groups_words[i] );
				}
			}
		}
	}
	
	return adj_words;
}
```

**Third approach:** 

This third algorithm is more complex and uses additional `map`s. As before, words
are grouped by lengths, and then each group is worked on separately.

```cpp
std::map<std::string, std::vector<std::string>> compute_adjacent_words(
	const std::vector<std::string>& words
) {
	
	std::map<std::string, std::vector<std::string>> adj_words;
	std::map<int, std::vector<std::string>> words_by_length;
	
	// group words by their length
	for (auto& word : words) {
		word_by_length[ word.length() ].push_bacj(word);
	}
	
	// work on each group separately
	for (auto& entry : words_by_length) {
		const std::vector<std::string>& groups_words = entry.second;
		int group_num = entry.first;
		
		// work on each position in each group
		for (int i = 0; i < group_num; i++) {
			std::map<std::string, std::vector<std::string>> rep_to_word;
			
			// Remove one character in specified position, computing representative.
			// Words with same representatives are adjacent; so populate a map ...
			for (auto& word : groups_words) {
				std::string rep = word;
				rep.erase(i, 1);
				rep_to_word[ rep ].push_back(word);
			}
			
			// ... and then look for map values with more than one string
			for (auto& entry : rep_to_word) {
				const std::vector<std::string>& clique = entry.second;
				if (clique.size() >= 2) {
					for (int p = 0; p < clique.size(); p++) {
						for (int q = p + 1; q < clique.size(); q++) {
							adj_words[ clique[p] ].push_back(clique[q]);
							adj_words[ clique[q] ].push_back(clique[p]);
						}
					}
				}
			}
		}
	}
	
	return adj_words;
}
```

## Summary to avoid performance issues

### AVL trees

* The heights of left and right subtress differ at most by $1$; not too deep.
* Operations such as `insert` must restore the tree.

### Splay trees

* Nodes can get arbitrarily deep but after every access the tree is
  adjusted either using *zig-zag* or *zig-zig*.
* The net effect is that any sequence of $M$ operations takes
  $O(MlogN)$ which the is the same as a balanced tree.

### B-trees

* Balanced $M$-way (as opposed to binary trees) which are well suited
the external memory model. 
