#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <algorithm> // for swap, move, etc. functions
#include <stdexcept> // for exceptions

template<typename Object>
class Vector {
    public:
        //typedef Object* interator;
        //typedef const Object* const_iterator;
        static const int SPARE_CAPACITY = 16;

        // one-argument constructor
        explicit Vector( int initSize = 0 );

        // copy constructor
        Vector( const Vector &rhs );

        // copy assignment
        Vector & operator=( const Vector &rhs );

        // destructor
        ~Vector();

        // move constructor
        Vector( Vector &&rhs );

        // move assignment
        Vector & operator=( Vector &&rhs );

        /*
        @brief Set the vector's size, after possibly expanding the capacity.
        @param newSize the new size for the vector.
        @return void
        */
        void resize( int newSize );

        /*
        @brief Expand the vector's capacity. It can also be used to shrink
               the underlying array if the specified new capacity is at least
               as large as the size.
        @param newCapacity the vector's new capacity
        @return void
        */
        void reserve( int newCapacity );

        /*
        @brief Provide array indexing.
        @return Object
        */
        Object & operator[]( int index );

        /*
        @brief Provide array indexing.
        @return Object
        */
        const Object & operator[]( int index ) const;

        /*
        @brief Check if vector is empty.
        @return bool
        */
        bool empty() const;

        /*
        @brief Return the number of items in the vector.
        @return int
        */
        int size() const;

        /*
        @brief Return the number of items the vector can hold.
        @return int
        */
        int capacity() const;

        /*
        @brief Push new object into the vector.
        @return void
        */
        void push_back( const Object &x );

        void push_back( Object &&x );

        /*
        @brief Pop object from the vector's end.
        @throw logic_error exception
        @return void
        */
        void pop_back();

        /*
        @brief Return object from the vector's end.
        @return Object
        */
        const Object & back() const;


        // iterators
        Object* begin();
        const Object* begin() const;

        Object* end();
        const Object* end() const;

    private:
        int size_;        // the number of items are currently in the vector.
        int capacity_;    // the number of items the vector can hold.
        Object* objects_; // the array of items.

};

template<typename Object>
Vector<Object>::Vector( int initSize ) :
    size_{ initSize }, capacity_{ initSize + SPARE_CAPACITY }
{
    objects_ = new Object[ capacity_ ];
}

// copy constructor
template<typename Object>
Vector<Object>::Vector( const Vector &rhs ) :
    size_{ rhs.size_ }, capacity_{ rhs.capacity_ }, objects_{ nullptr }
{
    objects_ = new Object[ capacity_ ];
    for (int k = 0; k < size_; k++) {
        objects_[k] = rhs.objects_[k];
    }
}

// copy assignment
template<typename Object>
Vector<Object> & Vector<Object>::operator=( const Vector &rhs ) {
    Vector copy = rhs;
    std::swap(*this, copy);
    return *this;
}

// destructor
template<typename Object>
Vector<Object>::~Vector() {
    delete [] objects_;
}

// move constructor
template<typename Object>
Vector<Object>::Vector( Vector &&rhs ) :
    size_{ rhs.size_ }, capacity_{ rhs.capacity_ }, objects_{ rhs.objects_ }
{
    rhs.objects_ = nullptr;
    rhs.size_ = 0;
    rhs.capacity_ = 0;
}

// move assignment
template<typename Object>
Vector<Object> & Vector<Object>::operator=( Vector &&rhs ) {
    std::swap(size_, rhs.size_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(objects_, rhs.objects_);

    return *this;
}


/*
@brief Set the vector's size, after possinly expanding the capacity.
@param newSize the new size for the vector.
@return void
*/
template<typename Object>
void Vector<Object>::resize( int newSize ) {
    if (newSize > capacity_) {
        reserve(newSize * 2);
    }
    size_ = newSize;
}

/*
@brief Expand the vector's capacity. It can also be used to shrink
       the underlying array if the specified new capacity is at least
       as large as the size.
@param newCapacity the vector's new capacity
@return void
*/
template<typename Object>
void Vector<Object>::reserve( int newCapacity ) {
    if (newCapacity < size_) return;

    Object *newArray = new Object[newCapacity];
    for (int k = 0; k < size_; k++) {
        newArray[k] = std::move(objects_[k]);
    }

    capacity_ = newCapacity;
    std::swap(objects_, newArray);
    delete [] newArray;
}

template<typename Object>
Object & Vector<Object>::operator[]( int index ) {
    return objects_[index];
}

template<typename Object>
const Object & Vector<Object>::operator[]( int index ) const {
    return objects_[index];
}

template<typename Object>
bool Vector<Object>::empty() const {
    return size() == 0;
}

template<typename Object>
int Vector<Object>::size() const {
    return size_;
}

template<typename Object>
int Vector<Object>::capacity() const {
    return capacity_;
}

template<typename Object>
void Vector<Object>::push_back( const Object &x ) {
    if (size_ == capacity_) {
        reserve(2 * capacity_ + 1);   
    }
    objects_[size_] = x;
    size_ += 1;
}

template<typename Object>
void Vector<Object>::push_back( Object &&x ) {
    if (size_ == capacity_) {
        reserve(2 * capacity_ + 1);
    }
    objects_[size_] = std::move(x);
    size_ += 1;
}

template<typename Object>
void Vector<Object>::pop_back() {
    if (size_ == 0) {
        throw std::logic_error("Cannot pop from an empty vector.\n");
    }
    size_ -= 1;
}

template<typename Object>
const Object & Vector<Object>::back() const {
    if (size_ == 0) {
        throw std::logic_error("Cannot last item from an empty vector.\n");
    }

    return objects_[size_ - 1];
}

template<typename Object>
Object* Vector<Object>::begin() { return &objects_[0]; }

template<typename Object>
const Object* Vector<Object>::begin() const { return &objects_[0]; }

template<typename Object>
Object* Vector<Object>::end() { return &objects_[size()]; }

template<typename Object>
const Object* Vector<Object>::end() const { return &objects_[size()]; }

#endif
