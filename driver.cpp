#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "MovieHashTable.hpp"
#include "DirectorSkipList.hpp"
#include "MovieNode.hpp"

using namespace std;

// Function prototypes
MovieNode* parseMovieLine(string line);
void readMovieCSV(string filename,  MovieHashTable &movieTable, DirectorSkipList &directorList);
void display_menu();


// Main function
int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "Invalid number of arguments." << endl;
        cout << "Usage: ./<program name> <csv file> <hashTable size> <skipList size>" << endl;
        return 0;
    }

    srand(time(0));

    MovieHashTable HT(atoi(argv[2]));
    DirectorSkipList DSL(atoi(argv[3]), DEFAULT_LEVELS);

    readMovieCSV(argv[1], HT, DSL);

    int option = 0;
    int num_movies = 0;
    string movie_name;
    string director_name;
    MovieNode* movie = new MovieNode();
    DirectorSLNode* director_search = new DirectorSLNode();

    while (option != 5) {
        display_menu();
        cin >> option;
        cout << endl;
        switch(option) {
            case 1:
            {
                cout << "Enter movie name: ";
                cin.ignore();
                getline(cin, movie_name);
                movie = HT.search(movie_name);
                while (movie == nullptr) {
                    cout << "Sorry, but that movie could not be found. Make sure you've typed in the title correctly and try again." << endl;
                    cout << "Enter movie name: ";
                    movie_name = "";
                    getline(cin, movie_name);
                    movie = HT.search(movie_name);
                }
                cout << "The director of " << movie_name << " is " << movie->director << endl << endl;
                break;
            }
            case 2:
            {
                cout << "Enter director name: ";
                cin.ignore();
                getline(cin, director_name);
                director_search = nullptr;
                director_search = DSL.search(director_name);
                if (director_search == nullptr) {
                    cout << "Sorry, the director could not be found." << endl;
                    break;
                }
                else {
                    num_movies = director_search->movies.size();
                    cout << director_name << " directed " << num_movies << " movies" << endl;
                    break;
                }
            }
            case 3:
            {
                cout << "Enter movie name: ";
                cin.ignore();
                getline(cin, movie_name);
                movie = HT.search(movie_name);
                while (movie == nullptr) {
                    cout << "Sorry, but that movie could not be found. Make sure you've typed in the title correctly and try again." << endl;
                    cout << "Enter movie name: ";
                    movie_name = "";
                    getline(cin, movie_name);
                    movie = HT.search(movie_name);
                }
                cout << movie_name << " is a " << movie->year << " (" << movie->genre << ") film featuring \"" << movie->actors << "\"" << endl;
                cout << "Plot: \"" << movie->description << "\"" << endl;
                break;
            }
            case 4:
            {
                cout << "Enter director name: ";
                cin.ignore();
                getline(cin, director_name);
                
                director_search = nullptr;
                director_search = DSL.search(director_name);
                if (director_search == nullptr) {
                    cout << "Sorry, this director could not be found." << endl;
                    break;
                }
                else {
                    cout << director_name << " directed the following movies:" << endl;
                    for (int i = 0; i < director_search->movies.size(); i++) {
                        cout << i << ": " << director_search->movies[i]->title << endl;
                    }
                    cout << endl;
                    break;
                }
            }
        }
    }

    HT.~MovieHashTable();
    DSL.~DirectorSkipList();
    delete movie;
    delete director_search;

    return 0;
}

// Function to parse a CSV line into a MovieNode object
MovieNode* parseMovieLine(string line) {
    stringstream ss(line);
    vector<string> fields;
    string field;

    // Loop through the characters in the line
    bool in_quotes = false;
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        if (c == '\"') {
            in_quotes = !in_quotes;
        } else if (c == ',' && !in_quotes) {
            // add field to vector and reset for next field
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
            // handle double quotes inside a quoted field
            if (in_quotes && c == '\"' && i < line.length() - 1 && line[i+1] == '\"') {
                field += '\"'; // add the second double quote and skip it
                ++i;
            }
        }
    }
    fields.push_back(field);

    if (fields.size() != 12) {
        cerr << "Error: Invalid movie line format" << line << endl;
        return nullptr;
    }

    int rank = stoi(fields[0]);
    string title = fields[1];
    string genre = fields[2];
    string description = fields[3];
    string director = fields[4];
    string actors = fields[5];
    int year = stoi(fields[6]);
    int runtime = stoi(fields[7]);
    float rating = stof(fields[8]);
    int votes = stoi(fields[9]);
    float revenue = stof(fields[10]);
    int metascore = stoi(fields[11]);

    // Create a new MovieNode object with the parsed fields
    MovieNode* movie = new MovieNode(rank, title, genre, description, director, actors, year, runtime, rating, votes, revenue, metascore);
    return movie;
}

// Function to read a CSV file into a vector of MovieNode objects
void readMovieCSV(string filename,  MovieHashTable &movieTable, DirectorSkipList &directorList) {
    ifstream fin(filename);
    string line = "";

    if (!fin.is_open()) {
        cout << "Failed" << endl;
        return;
    }

    getline(fin, line);
    line = "";

    while (getline(fin, line)) {
        MovieNode* movie = parseMovieLine(line);
        movieTable.insert(movie->title, movie);
        directorList.insert(movie->director, movie);
    }
    cout << "Number of Collisions: " << movieTable.getCollisions() << endl;
}

// Function to display the menu options
void display_menu() {
    cout << "Please select an option:" << endl;
    cout << "1. Find the director of a movie" << endl;
    cout << "2. Find the number of movies by a director" << endl;
    cout << "3. Find the description of a movie" << endl;
    cout << "4. List the movies by a director" << endl;
    cout << "5. Quit" << endl << endl;
    cout << "Enter an option: ";
}
