#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <iostream>
#include <algorithm>

template<typename Comparable>
class BinarySearchTree {
    public:
        BinarySearchTree();
        ///BinarySearchTree( const BinarySearchTree& rhs );            // copy constructor
        ///BinarySearchTree( const BinarySearchTree&& rhs );           // move constructor
        ///BinarySearchTree& operator=( const BinarySearchTree& rhs ); // copy assign.
        ///BinarySearchTree& operator=( BinarySearchTree&& rhs );      // move assign.
        ///~BinarySearchTree();                                        // destructor

        /*
        @brief Insert x into the tree; duplicates are ignored.
        @return void
        */
        void insert( const Comparable& x );
        void insert( Comparable&& x );

        /*
        @brief Return true if x us found in the tree.
        @return bool
        */
        bool contains( const Comparable& x ) const;

        /*
        @brief Check if tree is empty.
        @return bool
        */
        bool isEmpty() const;

        /*
        @brief Print string representation of BTS.
        @return void
        */
        void printTree( std::ostream& out = std::cout ) const;

    private:
        struct BinaryNode {
            Comparable  _item;
            BinaryNode* _left;
            BinaryNode* _right;

            // copy constructor
            BinaryNode( const Comparable& item, BinaryNode* left, BinaryNode* right)
                : _item{ item }, _left{ left }, _right{ right } {}
            // move constructor
            BinaryNode( Comparable&& item, BinaryNode* left, BinaryNode* right)
                : _item{ item }, _left{ left }, _right{ right } {}
        };

        BinaryNode* _root;

        void _insert( const Comparable& item, BinaryNode*& t );
        void _insert( Comparable&& item, BinaryNode*& t );

        bool _contains( const Comparable& x, BinaryNode* t) const;
        
};

/******************************************************************************
PRIVATE METHODS
******************************************************************************/
template<typename Comparable>
void BinarySearchTree<Comparable>::_insert( const Comparable& x, BinaryNode*& t ) {
    if (t == nullptr) {
        t = new BinaryNode{ x, nullptr, nullptr };
    }
    // visit left subtree
    else if (x < t->_item) {
        _insert(x, t->_left);
    }
    // visit right subtree
    else if (x > t->_item) {
        _insert(x, t->_right);
    }
    else { ; } // Duplicate; do nothing
}

template<typename Comparable>
void BinarySearchTree<Comparable>::_insert( Comparable&& x, BinaryNode*& t ) {
    if (t == nullptr) {
        t = new BinaryNode{ std::move(x), nullptr, nullptr };
    }
    // visit left subtree
    else if (x < t->_item) {
        _insert(std::move(x), t->_left);
    }
    // visit right subtree
    else if (x > t->_item) {
        _insert(std::move(x), t->_right);
    }
    else { ; } // Duplicate; do nothing
}

template<typename Comparable>
bool BinarySearchTree<Comparable>::_contains( const Comparable& x, BinaryNode* t) const {
    if (t == nullptr) {
        return false;
    }
    // look in the left subtree
    else if (x < t->_item) {
        return _contains(x, t->_left);
    }
    // look in the right subtree
    else if (x > t->_item) {
        return _contains(x, t->_right);
    }
    // Found a match
    else {
        return true;
    }
}

/******************************************************************************
CONSTRUCTORS AND BIG FIVE 
******************************************************************************/

template<typename Comparable>
BinarySearchTree<Comparable>::BinarySearchTree() {
    _root = nullptr; 
}

/******************************************************************************
CONSTRUCTORS AND BIG FIVE 
******************************************************************************/

template<typename Comparable>
void BinarySearchTree<Comparable>::insert( const Comparable& x ) {
    _insert(x, _root);
}

template<typename Comparable>
void BinarySearchTree<Comparable>::insert( Comparable&& x ) {
    _insert(x, _root);
}

template<typename Comparable>
bool BinarySearchTree<Comparable>::contains( const Comparable& x ) const {
    return _contains(x, _root);
}

template<typename Comparable>
bool BinarySearchTree<Comparable>::isEmpty() const {
    if (_root == nullptr) return true;
    return false;
}

template<typename Comparable>
void BinarySearchTree<Comparable>::printTree( std::ostream& out ) const {
    // scaffolding
    std::cout << _root->_item << "\n";
}

#endif
