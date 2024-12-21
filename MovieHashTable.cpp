#include<iostream>
#include <vector>
#include "MovieHashTable.hpp"
using namespace std;

// Constructor for MovieHashTable with default size
/*
    Sets the table_size to the default capacity
    Sets table to an array of Movie Nodes pointers with size table_size
    Sets all movie nodes in array to nullptr
*/
MovieHashTable::MovieHashTable() {
    table_size = DEFAULT_HTABLE_CAPACITY;
    table = new MovieNode *[table_size];

    for (int i = 0; i < table_size; i++) table[i] = nullptr;
}

// Constructor for MovieHashTable with given size
/*
    Sets the table_size to the size s inputed by user
    Sets table to an array of Movie Nodes pointers with size table_size
    Sets all movie nodes in array to nullptr
*/
MovieHashTable::MovieHashTable(int s) {
    table_size = s;
    table = new MovieNode *[table_size];

    for (int i = 0; i < table_size; i++) table[i] = nullptr;
}

// Destructor for MovieHashTable that deletes all nodes in the hash table
/*
    Loops through all table indexes, and then their next pointers, deleting them
    After all have been looped through, the table is deleted
*/
MovieHashTable::~MovieHashTable() {
    MovieNode* prev;
    MovieNode* curr;
    for (int i = 0; i < table_size; i++) {
        curr = table[i];
        prev = nullptr;

        while (curr != nullptr) {
            prev = curr;
            curr = curr->next;

            delete prev;
        }
    }

    delete[] table;
}

// Hash function for MovieHashTable that returns an index in the hash table for a given movie title.
// Students must use their identikey to come up with a creative hash function that minimizes collisions
// for the given IMDB-Movie database to ensure efficient insertion and retrieval of movie nodes.
/*
    Algorithm:
    1. All characters in string title input are parsed, adding the ASCII val to index
    2. The value of index is then multiplied by 110489571
    3. We use MOD function on index with table_size to hopefully produce unique index
    4. If a unique index is not produced, use linear probing to create a new index, add one to the collision each time
    5. Return the final index
*/
int MovieHashTable::hash(string title) {
    int index = 0;

    for (char c : title) {
        index += (int)c;
    }

    index *= 110489571;
    index %= table_size;

    while (table[index] != nullptr && table[index]->title != title) {
        index = (index + 1) % table_size;
        setCollisions();
    }

    return index;
}

// Inserts a movie node into the hash table with the specified title
/*
    Uses our hash function to create an index. Then sets that index in table array to the given movie node
    Don't need to check for collisions as we did that in our hash function
*/
void MovieHashTable::insert(string title, MovieNode* movie) {
    int index = hash(title);
    table[index] = movie;
}

// Searches for a node in the hash table with the specified title
/*
    Uses our hash function to find the index of this title. We then return the index of table array that is produced
*/
MovieNode* MovieHashTable::search(string title) {
    int index = hash(title);
    return table[index];
}

// Returns the number of collisions that have occurred during insertion into the hash table
int MovieHashTable::getCollisions() {
    return n_collisions;
}

// Increments the number of collisions that have occurred during insertion into the hash table
void MovieHashTable::setCollisions() {
    n_collisions++;
}
