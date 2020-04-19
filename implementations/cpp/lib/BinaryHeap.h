#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <iostream>
#include <vector>

template<typename Comparable>
class BinaryHeap {
    public:
        explicit BinaryHeap( int capacity = 100 );
        explicit BinaryHeap( const std::vector<Comparable>& items);

        // Find the minimum item in the heap.
        const Comparable& findMin() const;

        // Insert item x, allowing duplicates.
        void insert( const Comparable& x );
        void insert( Comparable&& x );

        // Remove the minimum item.
        void deleteMin();

        // Remove the minimum item and place it in minItem.
        void deleteMin( Comparable& minItem );

        // Check if heap is empty.
        bool isEmpty() const;

        // Empty the heap.
        void makeEmpty();

    private:
        int _currentSize;               // number of elements in heap
        std::vector<Comparable> _array; // the heap array

        // Establish heap order property from an arbitrary arrangement
        // of items. Runs in linear time.
        void _buildHeap();

        // Percolate down in the heap.
        // hole is the index at which the percolate begins.
        void _percolateDown( int hole );
};

template<typename Comparable>
BinaryHeap<Comparable>::BinaryHeap( int capacity ) 
    : _array( capacity + 1 ), _currentSize{ 0 }
{ }


template<typename Comparable>
BinaryHeap<Comparable>::BinaryHeap( const std::vector<Comparable> & items ) 
    : _array( items.size( ) + 10 ), _currentSize{ items.size( ) } {
    for (int i = 0; i < items.size( ); ++i) {
        _array[i + 1] = items[i];
    }

    _buildHeap( );
}

template<typename Comparable>
bool BinaryHeap<Comparable>::isEmpty() const {
    return _currentSize == 0;
}

template<typename Comparable>
const Comparable& BinaryHeap<Comparable>::findMin() const {
    if (isEmpty()) {
        std::cout << "Error: Underflow\n";
        std::abort();
    }

    return _array[1];
}

template<typename Comparable>
void BinaryHeap<Comparable>::insert( const Comparable& x ) {
    if (_currentSize == _array.size() - 1) {
        _array.resize(_array.size() * 2);
    }

    // percolate up
    _currentSize += 1;
    int hole = _currentSize;
    Comparable copy = x;

    _array[0] = std::move(copy);
    //while (x < _array[hole/2]) {
    for (; x < _array[hole/2]; hole /= 2) {
        _array[hole] = std::move(_array[hole/2]);
        //hole = hole / 2;
    }

    _array[hole] = std::move(_array[0]);
}

template<typename Comparable>
void BinaryHeap<Comparable>::insert( Comparable&& x ) {
    if (_currentSize == _array.size() - 1) {
        _array.resize((_array.size() * 2));
    }

    // percolate up
    _currentSize += 1;
    int hole = _currentSize;

    for (; hole > 1 && x < _array[hole/2]; hole /= 2) {
        _array[hole] = std::move(_array[hole/2]);
    }
    
    _array[hole] = std::move(x);
}

template<typename Comparable>
void BinaryHeap<Comparable>::deleteMin() {
    if (isEmpty()) {
        std::cout << "Error: Underflow\n";
        std::abort();
    }

    _array[1] = std::move(_array[_currentSize--]);
    _percolateDown(1);
}

template<typename Comparable>
void BinaryHeap<Comparable>::deleteMin( Comparable& minItem ) {
    if (isEmpty()) {
        std::cout << "Error: Underflow\n";
        std::abort();
    }
    
    minItem = std::move(_array[1]);
    _array[1] =  std::move(_array[_currentSize--]);
    _percolateDown(1);
}

template<typename Comparable>
void BinaryHeap<Comparable>::makeEmpty() {
    _currentSize = 0;
}

template<typename Comparable>
void BinaryHeap<Comparable>::_buildHeap() {
    for (int i = _currentSize / 2; i > 0; --i) {
        _percolateDown(i);
    }
}

template<typename Comparable>
void BinaryHeap<Comparable>::_percolateDown( int hole ) {
    int child;
    Comparable tmp = std::move(_array[hole]);

    for (; hole * 2 <= _currentSize; hole = child) {
        child = hole * 2;
        if (child != _currentSize && _array[child + 1] < _array[child]) {
            child++;
        }

        if (_array[child] < tmp) {
            _array[hole] = std::move(_array[child]);
        }
        else {
            break;
        }
    }

    _array[hole] = std::move(tmp);
}

#endif
