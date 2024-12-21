#include <iostream>
#include <vector>
#include "DirectorSkipList.hpp"
using namespace std;

// Constructor for DirectorSkipList with default capacity and levels
DirectorSkipList::DirectorSkipList() {
    capacity = DEFAULT_CAPACITY;
    levels = DEFAULT_LEVELS;
    head = new DirectorSLNode();
    head->next = vector<DirectorSLNode*>(levels, nullptr);
}

// Constructor for DirectorSkipList with given capacity and levels
DirectorSkipList::DirectorSkipList(int _cap, int _levels) {
    capacity = _cap;
    levels = _levels;
    head = new DirectorSLNode();
    head->next = vector<DirectorSLNode*>(_levels, nullptr);
}

// Destructor for DirectorSkipList that deletes all nodes in the skip list (but not the MovieNodes, which are shared with other data structures)
DirectorSkipList::~DirectorSkipList() {
    DirectorSLNode* curr = head;
    while (curr != nullptr) {
        DirectorSLNode* next = curr->next[0];
        delete curr;
        curr = next;
    }
    head = nullptr;
}

// Inserts a movie node into the skip list with the specified director
void DirectorSkipList::insert(string director, MovieNode* _movie) {
    DirectorSLNode* ptr = head;
    DirectorSLNode* lasts[levels];

    for (int i = levels - 1; i >= 0; i--) {
        while ((ptr->next[i] != nullptr) && (ptr->next[i]->director < director)) {
            ptr = ptr->next[i];
        }
        lasts[i] = ptr;
    }
    ptr = ptr->next[0];

    if ((ptr != nullptr) && (ptr->director == director)) {
        ptr->addMovie(_movie);
    }
    else {
        ptr = head;

        int n_levels = 1;
        while (n_levels < levels && rand() % 2 == 0) {
            n_levels++;
        }

        DirectorSLNode* director_ = new DirectorSLNode(director, n_levels);
        director_->addMovie(_movie);
        for (int i = 0; i < n_levels; i++) {
            director_->next[i] = lasts[i]->next[i];
            lasts[i]->next[i] = director_;
        }
        size++;
    }
}

// Searches for a node in the skip list with the specified director
DirectorSLNode *DirectorSkipList::search(string director) {
    DirectorSLNode* curr = head;
    for (int i = levels - 1; i >= 0; i--) {
        while ((curr->next[i] != nullptr) && (curr->next[i]->director < director)) {
            curr = curr->next[i];
        }
    }
    curr = curr->next[0];
    if ((curr != nullptr) && (curr->director == director)) {
        return curr;
    }
    return nullptr;
}

// Pretty-prints the skip list
void DirectorSkipList::prettyPrint() {
    DirectorSLNode* curr = head;
    for (int i = levels - 1; i >= 0; i--) {
        curr = curr->next[i];
        while (curr != nullptr) {
            cout << curr->director << "-> ";
            curr = curr->next[i];
        }
        cout << endl;
        curr = head;
    }
}