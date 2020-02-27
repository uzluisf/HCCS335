#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <functional>

/*
Function object template for the hash function.
*/
template<typename Key>
class hash {
    public:
        size_t operator()( const Key& k) const;
};

/*
A hash routine for strings.
*/
template<>
class hash<std::string> {
    public:
        size_t operator()( const std::string& key ) {
            size_t hashVal = 0;
            for( char ch : key ) hashVal = 37 * hashVal + ch;
            return hashVal;
        }
};

/*
A hash routine for ints.
*/
template<>
class hash<int> {
    public:
        size_t operator()( const int key ) {
            return key;
        }
};

/*
Few helper functions
*/
bool is_prime( int n );
int next_prime( int n );

/*
Class that implements a hash table using the separate chaining strategy.
Objects must provide a hash function and equality operators.
*/
template<typename T>
class HashTable {
    public:
        explicit HashTable( int size = 101 );
        
        /*
        @brief Check if the hash table contains object x.
        @return bool
        */
        bool contains( const T& x ) const;
        
        /*
        @brief Empty the hash table.
        @return void
        */
        void makeEmpty();

        /*
        @brief Insert object x into the hash table.
        @return bool
        */
        bool insert( const T& x );
        bool insert( T&& x );

        /*
        @brief Remove object x from the hash table.
        @return bool
        */
        bool remove( const T& x);
       
    private:
        /*
        This is an array of list. Each list contains the objects that map
        to the same array's key.
        */
        std::vector<std::list<T>> _lists;

        /*
        Number of items currently in the table.
        */
        int _num_of_items;
        
        void _rehash();

        /*
        Generic hash function.
        */
        size_t _myhash( const T& x ) const;
};

/*
Private methods
*/
template<typename T>
void HashTable<T>::_rehash() {
    // get hold of current table.
    std::vector<std::list<T>> oldLists = _lists;

    // create new double-sized, empty table. Table size is prime to ensure
    // a good distribution of the keys.
    _lists.resize( next_prime(2 * _lists.size()));
    for (auto& list : _lists) list.clear( );
    
    // copy old table into new, resized table.
    _num_of_items = 0;
    for (auto& list : oldLists)
        for (auto& x : list)
            insert(std::move(x));
}

template<typename T>
size_t HashTable<T>::_myhash( const T& x ) const {
    static hash<T> hf;
    return hf(x) % _lists.size();
}

/*
Public methods
*/
template<typename T>
HashTable<T>::HashTable( int size ) : _num_of_items{ 0 } {
    _lists.resize(size);
}

template<typename T>
bool HashTable<T>::contains( const T& x ) const {
    auto& list = _lists[ _myhash(x) ];
    return find(list.begin(), list.end(), x) != list.end();
}

template<typename T>
void HashTable<T>::makeEmpty() {
    for (auto& list : _lists) {
        list.clear();
    }
}

template<typename T>
bool HashTable<T>::insert( const T& x ) {
    std::cout << 
        x.get_name() <<
        " => " <<
        _myhash(x) << "\n";

    auto& list = _lists[ _myhash(x) ];
    auto itr = find(list.begin(), list.end(), x);

    if (itr != list.end()) return false;

    list.push_back(x);

    if (++_num_of_items > _lists.size()) _rehash();

    return true;
}

template<typename T>
bool HashTable<T>::insert( T&& x ) {
    std::cout << 
        x.get_name() <<
        " => " <<
        _myhash(x) << "\n";


    auto& list = _lists[ _myhash(x) ];
    auto itr = find(list.begin(), list.end(), x);

    if (itr != list.end()) return false;

    list.push_back(std::move(x));

    if (++_num_of_items > _lists.size()) _rehash();

    return true;
}

template<typename T>
bool HashTable<T>::remove( const T& x) {
    auto& list = _lists[ _myhash(x) ];
    auto itr = find(list.begin(), list.end(), x);

    if (itr == list.end()) return false;

    list.erase(itr);
    _num_of_items -= 1;

    return true;
}

/*
@brief Check if an integer is a prime number. Not that efficient.
@return bool
*/
bool is_prime( int n ) {
    if (n == 2 || n == 3) return true;
    if (n == 1 || n % 2 == 0) return false;

    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;

    return true;
}

/*
@brief Get the next prime after the integer n.
@return int
*/
int next_prime( int n ) {
    if (n % 2 == 0) ++n;
    while (!is_prime(n)) n += 2;
    return n;
}

#endif

