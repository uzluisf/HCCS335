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
        @brief Find the smallest item in a subtree.
        @return Comparable
        */
        const Comparable& findMin() const;

        /*
        @brief Find the largest item in a subtree.
        @return Comparable
        */
        const Comparable& findMax() const;

        /*
        @brief Insert x into the tree; duplicates are ignored.
        @return void
        */
        void insert( const Comparable& x );
        void insert( Comparable&& x );

        /*
        @brief Remove x from the tree.
        @return void
        */
        void remove( const Comparable& x );

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
            Comparable  item;
            BinaryNode* left;
            BinaryNode* right;

            // copy constructor
            BinaryNode( const Comparable& item, BinaryNode* left, BinaryNode* right)
                : item{ item }, left{ left }, right{ right } {}
            // move constructor
            BinaryNode( Comparable&& item, BinaryNode* left, BinaryNode* right)
                : item{ item }, left{ left }, right{ right } {}
        };

        BinaryNode* _root;

        BinaryNode* _findMin( BinaryNode* t) const;
        BinaryNode* _findMax( BinaryNode* t) const;

        void _insert( const Comparable& item, BinaryNode*& t );
        void _insert( Comparable&& item, BinaryNode*& t );
        void _remove( const Comparable& x, BinaryNode*& t);

        bool _contains( const Comparable& x, BinaryNode* t) const;
        
};

/******************************************************************************
PRIVATE METHODS
******************************************************************************/
template<typename Comparable>
typename BinarySearchTree<Comparable>::BinaryNode* BinarySearchTree<Comparable>::_findMin( BinaryNode* t ) const {
    if (t == nullptr) return nullptr;
    if (t->left == nullptr) return t;
    return _findMin(t->left);
}

template<typename Comparable>
typename BinarySearchTree<Comparable>::BinaryNode* BinarySearchTree<Comparable>::_findMax( BinaryNode* t ) const {
    if (t != nullptr) {
        while (t->right != nullptr)
            t = t->right;
    }
    return t;
}

template<typename Comparable>
void BinarySearchTree<Comparable>::_insert( const Comparable& x, BinaryNode*& t ) {
    if (t == nullptr) {
        t = new BinaryNode{ x, nullptr, nullptr };
    }
    // visit left subtree
    else if (x < t->item) {
        _insert(x, t->left);
    }
    // visit right subtree
    else if (x > t->item) {
        _insert(x, t->right);
    }
    else { ; } // Duplicate; do nothing
}

template<typename Comparable>
void BinarySearchTree<Comparable>::_insert( Comparable&& x, BinaryNode*& t ) {
    if (t == nullptr) {
        t = new BinaryNode{ std::move(x), nullptr, nullptr };
    }
    // visit left subtree
    else if (x < t->item) {
        _insert(std::move(x), t->left);
    }
    // visit right subtree
    else if (t->item < x) {
        _insert(std::move(x), t->right);
    }
    else { ; } // Duplicate; do nothing
}

template<typename Comparable>
void BinarySearchTree<Comparable>::_remove( const Comparable& x, BinaryNode*& t) {
    if (t == nullptr) {
        return; // item not found; do nothing
    }

    if (x < t->item) {
        _remove(x, t->left);
    }
    else if (t->item < x) {
        _remove(x, t->right);
    }
    // node with two children
    else if (t->left != nullptr && t->right != nullptr) {
        Comparable item = _findMin(t->right)->item;
        t->item = item;
        _remove(item, t->right);
    }
    // node with less than two children
    else {
        BinaryNode* oldNode = t;
        t = (t->left != nullptr) ? t->left : t->right;
        delete oldNode;
    }
}

template<typename Comparable>
bool BinarySearchTree<Comparable>::_contains( const Comparable& x, BinaryNode* t) const {
    if (t == nullptr) {
        return false;
    }
    // look in the left subtree
    else if (x < t->item) {
        return _contains(x, t->left);
    }
    // look in the right subtree
    else if (t->item < x) {
        return _contains(x, t->right);
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

//template<typename Comparable>
//BinarySearchTree<Comparable>::BinarySearchTree( const BinarySearchTree& rhs ) {
//}

/******************************************************************************
PUBLIC METHODS
******************************************************************************/

template<typename Comparable>
const Comparable& BinarySearchTree<Comparable>::findMin() const {
    return (_findMin(_root))->item;
}

template<typename Comparable>
const Comparable& BinarySearchTree<Comparable>::findMax() const {
    return (_findMax(_root))->item;
}

template<typename Comparable>
void BinarySearchTree<Comparable>::insert( const Comparable& x ) {
    _insert(x, _root);
}

template<typename Comparable>
void BinarySearchTree<Comparable>::insert( Comparable&& x ) {
    _insert(x, _root);
}

template<typename Comparable>
void BinarySearchTree<Comparable>::remove( const Comparable& x ) {
    _remove(x, _root);
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
    std::cout << _root->item << "\n";
}

#endif
