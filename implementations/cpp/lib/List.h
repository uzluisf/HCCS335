#ifndef LIST_H
#define LIST_H

#include <algorithm>

template<typename Object>
class List {
    private:
        struct Node {
            Object data;
            Node* prev; // pointer to previous node
            Node* next; // pointer to previous node

            Node(
                const Object& d = Object{},
                Node* p = nullptr,
                Node* n = nullptr
            ) : data{ d }, prev{ p }, next{ n } {}

            Node(
                Object&& d,
                Node* p = nullptr,
                Node* n = nullptr
            ) : data{ std::move(d) }, prev{ p }, next{ n } {}
        };

        int   _size;
        Node* _head;
        Node* _tail;

        void _init() {
            _size = 0;
            _head = new Node;
            _tail = new Node;
            _head->next = _tail;
            _tail->prev = _head;
        }

    public:
        /**********************************************************************
        BIG FIVE & CONSTRUCTORS
        **********************************************************************/

        List();                              // zero-parameter constructor
        ~List();                             // destructor
        List( const List& rhs );             // copy constructor
        List& operator=( const List& rhs ); // copy assignment
        List( List&& rhs );                  // move constructor
        List& operator=( List&& rhs );      // move assignment

        /**********************************************************************
        ITERATOR CLASSES
        **********************************************************************/

        class const_iterator;
        class iterator;

        /**********************************************************************
        METHODS
        **********************************************************************/
        iterator begin();
        const_iterator begin() const;
        
        iterator end();
        const_iterator end() const;

        /*
        @brief Return the number of items in the list.
        @return int
        */
        int size() const;

        /*
        @brief Return whether list is empty.
        @return bool
        */
        bool empty() const;

        /*
        @brief Remove all elements from the list.
        @return void
        */
        void clear();

        /*
        @brief Get element at the front of the list.
        @return Object
        */
        Object& front();

        const Object& front() const;

        /*
        @brief Get element at the rear of the list.
        @return Object
        */
        Object& back();

        const Object& back() const;

        /*
        @brief Push element to the front of the list.
        @return void
        */
        void push_front( const Object& x );

        void push_front( const Object&& x );

        /*
        @brief Push element to the front of the list.
        @return void
        */
        void push_back( const Object& x );

        void push_back( const Object&& x );

        /*
        @brief Remove element at the front of the list and return it.
        @return Object
        */
        void pop_front();

        /*
        @brief Remove element at the end of the list and return it.
        @return Object
        */
        void pop_back();

        /*
        @brief Insert element into the list.
        @return iterator
        */
        iterator insert( iterator itr, const Object& x );
        iterator insert( iterator itr, Object&& x );

        iterator erase( iterator iter );
        iterator erase( iterator from, iterator to );

};

/**********************************************************************
BIG FIVE & CONSTRUCTORS
**********************************************************************/

template<typename Object>
List<Object>::List() {
    _init();
}

template<typename Object>
List<Object>::~List() {
    clear();
    delete _head;
    delete _tail;
}

template<typename Object>
List<Object>::List( const List& rhs ) {
    _init();
    for (auto& x : rhs) { push_back(x); }
}

template<typename Object>
List<Object>& List<Object>::operator=( const List& rhs ) {
    List copy = rhs;
    std::swap(*this, copy);
    return *this;
}

template<typename Object>
List<Object>::List( List&& rhs ) : _size{ rhs._size }, _head{ rhs._head }, _tail{ rhs._tail } {
    rhs._size = 0;
    rhs._head = nullptr;
    rhs._tail = nullptr;
}

template<typename Object>
List<Object>& List<Object>::operator=( List&& rhs ) {
    std::swap(_size, rhs._size);
    std::swap(_head, rhs._head);
    std::swap(_tail, rhs._tail);

    return *this;
}

/**********************************************************************
ITERATOR CLASSES
**********************************************************************/

template<typename Object>
class List<Object>::const_iterator {
    public:
        const_iterator() : current{ nullptr } {}
    
        const Object& operator*() const {
            return retrieve();
        }
    
        const_iterator& operator++() {
            current = current->next;
            return *this;
        }
    
        const_iterator& operator++( int ) {
            const_iterator old = *this;
            ++(*this);
            return old;
        }
    
        bool operator==( const const_iterator& rhs ) const {
            return current == rhs.current;
        }
    
        bool operator!=( const const_iterator& rhs ) const {
            return !(*this == rhs);
        }
    
    protected:
        Node* current;
    
        Object& retrieve() const { return current->data; }
    
        const_iterator( Node* p ) : current{ p } { }
    
        // grant the List class access to const_iterator's nonpublic memebers.
       friend class List<Object>;
};

// inherits from const_iterator
template<typename Object>
class List<Object>::iterator : public List<Object>::const_iterator {
    public:
        iterator() {}

        Object& operator*() {
            return const_iterator::retrieve(); 
        }

        const Object& operator*() const {
            return const_iterator::operator*();
        }

        iterator& operator++() {
            this->current = this->current->next;
            return *this;
        }

        iterator operator++( int ) {
            iterator old = *this;
            ++(*this);
            return old;
        }

    protected:
        iterator( Node *p ) : const_iterator{ p } {}

        // grant the List class access to iterator's nonpublic memebers.
        friend class List<Object>;

};

/**********************************************************************
METHODS
**********************************************************************/

template<typename Object>
typename List<Object>::iterator List<Object>::begin() {
    return _head->next;
}

template<typename Object>
typename List<Object>::const_iterator List<Object>::begin() const {
    return _head->next; 
}

template<typename Object>
typename List<Object>::iterator List<Object>::end() {
    return _tail;    
}

template<typename Object>
typename List<Object>::const_iterator List<Object>::end() const {
    return _tail;
}

template<typename Object>
int List<Object>::size() const {
    return _size;
}

template<typename Object>
bool List<Object>::empty() const {
    return size() == 0;
}

template<typename Object>
void List<Object>::clear() {
    while (!empty()) {
        pop_front();
    }
}

template<typename Object>
Object& List<Object>::front() {
    return *begin();
}

template<typename Object>
const Object& List<Object>::front() const {
    return *begin();
}

template<typename Object>
Object& List<Object>::back() {
    Object x = --end();
    return *x;
}

template<typename Object>
const Object& List<Object>::back() const {
    Object x = --end();
    return *x;
}

template<typename Object>
void List<Object>::push_front( const Object& x ) {
    insert(begin(), x);
}

template<typename Object>
void List<Object>::push_front( const Object&& x ) {
    insert(begin(), std::move(x));
}

template<typename Object>
void List<Object>::push_back( const Object& x ) {
    insert(end(), x);
}

template<typename Object>
void List<Object>::push_back( const Object&& x ) {
    insert(end(), std::move(x));
}

template<typename Object>
void List<Object>::pop_front() {
    erase(begin());
}

template<typename Object>
void List<Object>::pop_back() {
    erase(--end());
}

template<typename Object>
typename List<Object>::iterator List<Object>::insert( iterator itr, const Object& x ) {
    Node *p = itr.current;
    _size += 1;
    
    Node* newNode = new Node{ x, p->prev, p };
    p->prev->next = newNode;
    p->pre = newNode;

    return p;
}

template<typename Object>
typename List<Object>::iterator List<Object>::insert( iterator itr, Object&& x ) {
    Node *p = itr.current;
    _size += 1;
    
    Node* newNode = new Node{ std::move(x), p->prev, p };
    p->prev->next = newNode;
    p->pre = newNode;

    return p;
   
}

template<typename Object>
typename List<Object>::iterator List<Object>::erase( iterator itr ) {
    Node *p = itr.current;
    iterator retVal{ p->next };
    p->prev->next = p->next;
    p->next->prev = p->prev;
    delete p;
    _size -= 1;

    return retVal;
}

template<typename Object>
typename List<Object>::iterator List<Object>::erase( iterator from, iterator to ) {
    for (iterator itr = from; itr != to;) {
        itr = erase(itr);
    }

    return to;
}


#endif
