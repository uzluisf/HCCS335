#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>
#include <algorithm>

template<typename Comparable>
class AVLTree {
    public:
        // zero-parameter constructor
        AVLTree();
        // copy constructor
        AVLTree( const AVLTree& rhs );
        // move constructor
        AVLTree( const AVLTree&& rhs );
        // destructor
        ~AVLTree();
        // copy assignment
        AVLTree& operator=( const AVLTree& rhs );
        // move assignment
        AVLTree& operator=( const AVLTree&& rhs );

        /*
        @brief Find the smallest item in a subtree t.
        @return Comparable&
        */
        const Comparable& findMin() const;

        /*
        @brief Find the largest item in a subtree t.
        @return Comparable&
        */
        const Comparable& findMax() const;

        /*
        @brief Check if an item x is in a subtree t.
        @return bool
        */
        bool contains( const Comparable& x );

        /*
        @brief Check if tree is empty;
        @return bool
        */
        bool isEmpty() const;

        /*
        @brief Make the tree logically empty.
        @return void
        */
        void makeEmpty();

        /*
        @brief Insert item x into subtree t; duplicates are ignored.
        @return void
        */
        void insert( const Comparable& x );

        void insert( const Comparable&& x );

        /*
        @brief Remove item x from the subtree t.
        @return void
        */
        void remove( const Comparable& x );

        /*
        @brief Print tree in order.
        @return void
        */
        void printTree() const;
        void printTree( std::ostream& out = std::cout ) const;

    private:
        struct AVLNode {
            Comparable item;
            AVLNode* left;
            AVLNode* right;
            int height;

            AVLNode( const Comparable& item, AVLNode* left, AVLNode* right, int h = 0)
                : item{ item }, left{ left }, right{ right }, height{ h } {}

            AVLNode( const Comparable&& item, AVLNode* left, AVLNode* right, int h = 0)
                : item{ std::move(item) }, left{ left }, right{ right }, height{ h } {}
        };

        AVLNode* _root;
        static const int ALLOWED_IMBALANCE = 1;

        void _insert( const Comparable& x, AVLNode* t );
        void _remove( const Comparable& x, AVLNode*& t );
        void _balance( AVLNode*& t );
        AVLNode* _findMin( AVLNode* t ) const;
        AVLNode* _findMax( AVLNode* t ) const;
        bool _contains( const Comparable& x, AVLNode* t ) const;
        void _makeEmpty( AVLNode*& t );
        AVLNode* _clone( AVLNode* t ) const;
        int _height( AVLNode* t ) const;
        int _max( int  lhs, int rhs ) const;
        void _print( std::ostream& out, AVLNode* t );
        void _rotateWithLeftChild( AVLNode*& k2 );
        void _rotateWithRightChild( AVLNode*& k1 );
        void _doubleWithLeftChild( AVLNode*& k3 );
        void _doubleWithRightChild( AVLNode*& k1 );
};

//PUBLIC

template<typename Comparable>
AVLTree<Comparable>::AVLTree() : _root{ nullptr } {}

template<typename Comparable>
AVLTree<Comparable>::AVLTree( const AVLTree& rhs ) : _root{ nullptr } {
    _root = _clone(rhs._root);
}

template<typename Comparable>
AVLTree<Comparable>::AVLTree( const AVLTree&& rhs ) : _root{ rhs._root } {
    rhs._root = nullptr;
}

template<typename Comparable>
AVLTree<Comparable>::~AVLTree() {
    makeEmpty();
}

template<typename Comparable>
AVLTree<Comparable>& AVLTree<Comparable>::operator=( const AVLTree& rhs ) {
    AVLTree copy = rhs;
    std::swap(*this, copy);
    return *this;
}

template<typename Comparable>
AVLTree<Comparable>& AVLTree<Comparable>::operator=( const AVLTree&& rhs ) {
    std::swap(_root, rhs._root);
    return *this;
}

template<typename Comparable>
const Comparable& AVLTree<Comparable>::findMin() const {
    return _findMin(_root)->item;
}

template<typename Comparable>
const Comparable& AVLTree<Comparable>::findMax() const {
    return _findMax(_root)->item;
}

template<typename Comparable>
bool AVLTree<Comparable>::contains( const Comparable& x ) {
    return _contains(x, _root);
}


template<typename Comparable>
bool AVLTree<Comparable>::isEmpty() const {
    return _root == nullptr;
}

template<typename Comparable>
void AVLTree<Comparable>::makeEmpty() {
    _makeEmpty(_root);
}

template<typename Comparable>
void AVLTree<Comparable>::insert( const Comparable& x ) {
    _insert(x, _root);
}

template<typename Comparable>
void AVLTree<Comparable>::insert( const Comparable&& x ) {
    _insert(std::move(x), _root);
}

template<typename Comparable>
void AVLTree<Comparable>::remove( const Comparable& x ) {
    _remove(x, _root);
}

template<typename Comparable>
void AVLTree<Comparable>::printTree() const {
    if (isEmpty()) {
        std::cout << "Empty tree\n";
    }
    else {
        _print(std::cout, _root);
    }
}

template<typename Comparable>
void AVLTree<Comparable>::printTree( std::ostream& out ) const {
    if (isEmpty()) {
        out << "Empty tree\n";
    }
    else {
        _print(out, _root);
    }
}

// PRIVATE

template<typename Comparable>
void AVLTree<Comparable>::_insert( const Comparable& x, AVLNode* t ) {
    if (t == nullptr)     { t = new AVLNode{x, nullptr, nullptr}; }
    else if (x < t->item) { _insert(x, t->left);  }
    else if (t->item < x) { _insert(x, t->right); }

    _balance(t);
}

template<typename Comparable>
void AVLTree<Comparable>::_remove( const Comparable& x, AVLNode*& t ) {
    if (t == nullptr) { return; }

    if      (x < t->item) { _remove(x, t->left);  }
    else if (t->item < x) { _remove(x, t->right); }
    else if (t->left != nullptr && t->right != nullptr) {
        Comparable item = _findMin(t->right)->item;
        t->item = item;
        remove(item, t->right);
    }
    else {
        AVLNode* oldNode = t;
        t = (t->left != nullptr) ? t->left : t->right;
        delete oldNode;
    }

    _balance(t);
}

template<typename Comparable>
void AVLTree<Comparable>::_balance( AVLNode*& t ) {
    if (t == nullptr) { return; }
    else if (_height(t->left) - _height(t->right) > ALLOWED_IMBALANCE) {
        if (_height(t->left->left) >= _height(t->left->right)) {
            _rotateWithLeftChild(t); // left-left single rotation
        }
        else {
            _doubleWithLeftChild(t);
        }
    }
    else if (_height(t->right) - _height(t->left) > ALLOWED_IMBALANCE) {
        if (_height(t->right->right) >= _height(t->right->left)) {
            _rotateWithRightChild(t); // right-right single rotation
        }
        else {
            _doubleWithRightChild(t);
        }
    }

    t->height = _max(_height(t->left), _height(t->right)) + 1;
}

template<typename Comparable>
typename AVLTree<Comparable>::AVLNode* AVLTree<Comparable>::_findMin( AVLNode* t ) const {
    if (t == nullptr)       { return nullptr; }
    if (t->left == nullptr) { return t; }
    return _findMin(t->left);
}


template<typename Comparable>
typename  AVLTree<Comparable>::AVLNode* AVLTree<Comparable>::_findMax( AVLNode* t ) const {
    if (t == nullptr)        { return nullptr; }
    if (t->right == nullptr) { return t; }
    return _findMax(t->right);
}

template<typename Comparable>
bool AVLTree<Comparable>::_contains( const Comparable& x, AVLNode* t ) const {
    if (t == nullptr)     { return false; }
    else if (x < t->item) { return _contains(x, t->left); }
    else if (t->item < x) { return _contains(x, t->right); }
    else                  { return true; }
}

template<typename Comparable>
void AVLTree<Comparable>::_makeEmpty( AVLNode*& t ) {
    if (t != nullptr) {
        _makeEmpty(t->left);
        _makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}

template<typename Comparable>
typename AVLTree<Comparable>::AVLNode* AVLTree<Comparable>::_clone( AVLNode* t ) const {
    if (t == nullptr) { return nullptr; }
    else { return new AVLNode{t->item, _clone(t->left), _clone(t->right), t->height };  }
}

template<typename Comparable>
int AVLTree<Comparable>::_height( AVLNode* t ) const {
    return t == nullptr ? -1 : t->height; 
}

template<typename Comparable>
int AVLTree<Comparable>::_max( int  lhs, int rhs ) const {
    return lhs > rhs ? lhs : rhs;
}

template<typename Comparable>
void AVLTree<Comparable>::_print( std::ostream& out, AVLNode* t ) {
    _print(out, t->left);
    out << t->item << " ";
    _print(out, t->right);
}

/*
@brief Rotate binary tree node with left child. This performs a left-left
       single rotation (case 1). Update heights, then sets new root.
@return void
*/
template<typename Comparable>
void AVLTree<Comparable>::_rotateWithLeftChild( AVLNode*& k2 ) {
    AVLNode *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = _max(_height(k2->left), _height(k2->right)) + 1;
    k1->height = _max(_height(k2->left), k2->height) + 1;
    k2 = k1;
}

/*
@brief Rotate binary tree node with right child. This performs a right-right
       single rotation (case 4). Update heights, then sets new root.
@return void
*/
template<typename Comparable>
void AVLTree<Comparable>::_rotateWithRightChild( AVLNode*& k1 ) {
    AVLNode *k2 = k1->right;
    k1->right = k2->left;
    k2->right = k1;
    k1->height = _max(_height(k1->left), _height(k1->right)) + 1;
    k2->height = _max(_height(k2->right), k1->height) + 1;
    k1 = k2;
}

/*
@brief Double rotate binary tree node: first left child with its right 
       child; then node k3 with new left child. This performs a
       right-left double rotation. Update heights, then set new root.
@return void
*/
template<typename Comparable>
void AVLTree<Comparable>::_doubleWithLeftChild( AVLNode*& k3 ) {
    _rotateWithRightChild(k3->left);
    _rotateWithLeftChild(k3);
}

/*
@brief Double rotate binary tree node: first right child with its left 
       child; then node k1 with new right child. This performs a
       left-right double rotation. Update heights, then set new root.
@return void
*/
template<typename Comparable>
void AVLTree<Comparable>::_doubleWithRightChild( AVLNode*& k1 ) {
    _rotateWithLeftChild(k1->right);
    _rotateWithRightChild(k1);
}

#endif
