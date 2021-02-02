/*
	Homework #3
	Student: Vishnu Rampersaud
	Class: CSCI 335
	Professor: Stamos
	Due Date: November, 05, 2019

	This is the spell_check.cc file. 
	It is a spell checker program for Part(C) of Homework #3
*/

#include "quadratic_probing.h"
#include "linear_probing.h"
#include "double_probing.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
using namespace std;

// Function declarations

// Creates a hash table of your choice using the file passed to it
// HashTableType: the hash table and type of hash table you want to create 
template <typename HashTableType>
void create_hashtable(HashTableType&, const string&);

// Checks the words in a file, and matches them to the hash table that was created
// Reads each line in the file, and calls the parse_line() function to separate the line into individual words
// Calls check_spelling() to compare each word with the hash table 
template <typename HashTableType>
void check_file_for_spelling_errors(HashTableType&, const string&);

// Takes in a line from the file as an input, and separates this line into words
// Removes all punction from each word, such as commas or periods, by
// checking each charater of the word so it can be accurately compared to the hash table
// Stores these words into a vector, which will eventually contain all words in the file
// vector used: vector<string> words_vector; 
void parse_line(const string&, vector<string>&);

// Converts all uppercase letters to lowercase letters in order to properly compare each word to the hash table
void convert_to_lowercase(string&);

// Checks to see if the word is a duplicate before storing it into the word vector
// vector used: vector<string> words_vector; 
// words_vector: a word vector that contains all words in the file
bool is_duplicate(const string&, const vector<string>&);

// Checks each word in the word vector, which now holds all the words in the file, to the hash table 
// If the word is not in the hash table, then it is not in our dictionary (hash table), so we count it as a misspelled word
// If we encounter a misspelled word, we try the three cases (each case has its own function) 
// on it to see if we can produce a word that is in our hash table
// Three cases are: 
// a) Adding one character in each possible position 
// b) Removing one character from the word 
// c) Swapping adjacent characters in the word 
template <typename HashTableType>
void check_spelling(HashTableType&, vector<string>&);

// Case 'a' for misspelled words 
// adds one character in each possible position 
// if one of the words produced is in our hash table, we store it in a vector to keep track of the changes 
// vector used: vector<string> add_changes; 
template <typename HashTableType>
void add_one_character(HashTableType&, const string&, vector<string>&);

// Case 'b' for misspelled words
// removes one character from each possible position from the word 
// if one of the words produced is in our hash table, we store it in a vector to keep track of the changes 
// vector used: vector<string> remove_changes; 
template <typename HashTableType>
void remove_one_character(HashTableType&, const string&, vector<string>&);

// Case 'c' for misspelled words
// Swaps each adjacent characters in the word
// if one of the words produced is in our hash table, we store it in a vector to keep track of the changes 
// vector used: vector<string> swap_changes; 
template <typename HashTableType>
void swap_adjacent_characters(HashTableType&, const string&, vector<string>&);

// Displays the corrections for misspelled words that were found using the three cases
// Takes in a vector that contains the corrections of misspelled words (add_changes, remove_changes or swap_changes) 
void display_corrections(const string&, const string&, vector<string>&);

// End of function declarations 



// Creates a hash table of your choice using the file passed to it
// HashTableType: the hash table and type of hash table you want to create 
template <typename HashTableType>
void create_hashtable(HashTableType& hash_table, const string& words_filename) {
	ifstream input_file;
	input_file.open(words_filename);

	string line;

	hash_table.MakeEmpty();

	while (!input_file.eof()) {
		getline(input_file, line);
		if (line.empty()) { continue; }
		hash_table.Insert(line);
	}

	input_file.close();
}

// Checks the words in a file, and matches them to the hash table that was created
// Reads each line in the file, and calls the parse_line() function to separate the line into individual words
// Calls check_spelling() to compare each word with the hash table 
template <typename HashTableType>
void check_file_for_spelling_errors(HashTableType& hash_table, const string& document)
{
	ifstream document_file;
	document_file.open(document);

	string line;
	vector<string> words_vector;

	while (!document_file.eof())
	{
		getline(document_file, line);
		if (line.empty()) { continue; }
		parse_line(line, words_vector);
	}

	check_spelling(hash_table, words_vector);
}

// Takes in a line from the file as an input, and separates this line into words
// Removes all punction from each word, such as commas or periods, by
// checking each charater of the word so it can be accurately compared to the hash table
// Stores these words into a vector, which will eventually contain all words in the file
// vector used: vector<string> words_vector;
void parse_line(const string& line, vector<string>& words_vector)
{

	stringstream ss(line);
	string word, temp;

	while (ss >> word) {
		unsigned int i = 0;
		while (i < word.size()) {
			if (ispunct(word[i])) {
				if (word[i] == '\'') {
					word.erase(i, 2);
					i--; 
				}
				else if (word[i] == '-' && word[i+1] == '-') {

					// if there is a double hyphen, -- , there is another word after it
					// split these words into two 
					temp = word.substr(i + 2);

					// erase the unneccesary characters from original word (ex: hyphens, second word) 
					word.erase(i, string::npos);

					// parse the new second word to remove punctuation and add to words_vector
					parse_line(temp, words_vector);

				}
				else if (word[i] == '-') {

					// if there is a hyphen, -- , there is another word after it
					// split these words into two 
					temp = word.substr(i + 1); 

					// erase the unneccesary characters from original word (ex: hyphen, second word) 
					word.erase(i, string::npos);

					// parse the new second word to remove punctuation and add to words_vector
					parse_line(temp, words_vector); 
					
				}
				else if (ispunct(word[i + 1])) {
					word.erase(word.begin() + i);
					word.erase(word.begin() + i);
					i--; 
				}
				else {
					word.erase(word.begin() + i);
					i--; 
				}
			}
			i++;
		}
		convert_to_lowercase(word);
		if (is_duplicate(word, words_vector)) { continue; }
		words_vector.push_back(word);
	}

}

// Converts all uppercase letters to lowercase letters in order to properly compare each word to the hash table
void convert_to_lowercase(string& word)
{
	for (unsigned int i = 0; i < word.size(); i++) {
		word[i] = tolower(word[i]);
	}
}

// Checks to see if the word is a duplicate before storing it into the word vector
// vector used: vector<string> words_vector; 
// words_vector: a word vector that contains all words in the file
bool is_duplicate(const string& word, const vector<string>& words_vector)
{
	for (unsigned int i = 0; i < words_vector.size(); i++) {
		if (word == words_vector[i]) {
			return true;
		}
	}

	return false;
}

// Checks each word in the word vector, which now holds all the words in the file, to the hash table 
// If the word is not in the hash table, then it is not in our dictionary (hash table), so we count it as a misspelled word
// If we encounter a misspelled word, we try the three cases (each case has its own function) 
// on it to see if we can produce a word that is in our hash table
// Three cases are: 
// a) Adding one character in each possible position 
// b) Removing one character from the word 
// c) Swapping adjacent characters in the word 
template <typename HashTableType>
void check_spelling(HashTableType& hash_table, vector<string>& words_vector)
{
	vector<string> add_changes, remove_changes, swap_changes;
	string case_a = "a", case_b = "b", case_c = "c";

	// for every word in the vector, check to see if it exists in the hash table
	// if it does not, try to use the three cases on that word. 
	// if these cases produce a valid word, then store it into their respective vectors
	// Clear each vector for each word
	for (unsigned int i = 0; i < words_vector.size(); i++)
	{
		vector<string> add_changes, remove_changes, swap_changes;
		string word = words_vector[i];
		bool found = hash_table.search(word);

		if (!found) {
			add_one_character(hash_table, word, add_changes);
			remove_one_character(hash_table, word, remove_changes);
			swap_adjacent_characters(hash_table, word, swap_changes);
		}

		if (add_changes.empty() && remove_changes.empty() && swap_changes.empty()) { continue; }

		cout << "\nCorrections for: " << word << endl;
		if (!add_changes.empty()) { display_corrections(case_a, word, add_changes); }
		if (!remove_changes.empty()) { display_corrections(case_b, word, remove_changes); }
		if (!swap_changes.empty()) { display_corrections(case_c, word, swap_changes); }

		add_changes.clear();
		remove_changes.clear();
		swap_changes.clear();
	}
}

// Case 'a' for misspelled words 
// adds one character in each possible position 
// if one of the words produced is in our hash table, we store it in a vector to keep track of the changes 
// vector used: vector<string> add_changes; 
template <typename HashTableType>
void add_one_character(HashTableType& hash_table, const string& word, vector<string>& add_changes)
{
	//ASCII values of the letters 'a' and 'z'
	int a = int('a');
	int z = int('z');

	string temp = word;

	// Adds each letter of the alphabet, using their ASCII numbers, from 'a' to 'z' to the word
	// Compare it to the hash table, if it is found then it is a valid word
	// Store it into the changes vector
	for (unsigned int i = 0; i <= word.size(); i++) {
		for (int j = a; j <= z; j++) {
			temp.insert(temp.begin() + i, char(j));
			bool found = hash_table.search(temp);
			if (found) { add_changes.push_back(temp); }
			temp.erase(temp.begin() + i);
		}
	}
}

// Case 'b' for misspelled words
// removes one character from each possible position from the word 
// if one of the words produced is in our hash table, we store it in a vector to keep track of the changes 
// vector used: vector<string> remove_changes; 
template <typename HashTableType>
void remove_one_character(HashTableType& hash_table, const string& word, vector<string>& remove_changes)
{
	for (unsigned int i = 0; i < word.size(); i++) {
		string temp = word;
		temp.erase(temp.begin() + i);
		bool found = hash_table.search(temp);
		if (found) { remove_changes.push_back(temp); }
	}
}

// Case 'c' for misspelled words
// Swaps each adjacent characters in the word
// if one of the words produced is in our hash table, we store it in a vector to keep track of the changes 
// vector used: vector<string> swap_changes; 
template <typename HashTableType>
void swap_adjacent_characters(HashTableType& hash_table, const string& word, vector<string>& swap_changes)
{
	for (unsigned int i = 0; i < word.size(); i++) {
		string temp = word;
		temp[i] = word[i + 1];
		temp[i + 1] = word[i];
		bool found = hash_table.search(temp);
		if (found) { swap_changes.push_back(temp); }
	}
}

// Displays the corrections for misspelled words that were found using the three cases
// Takes in a vector that contains the corrections of misspelled words (add_changes, remove_changes or swap_changes) 
void display_corrections(const string& case_, const string& word, vector<string>& changes)
{

	for (unsigned int i = 0; i < changes.size(); i++) {
		cout << word << " -> " << changes[i] << " (case " << case_ << ")" << endl;
	}

}

// Function to test if the input file(s) exists 
bool fileExist(const string& file)
{
	ifstream test; 
	test.open(file); 
	if (test.fail()) {
		cout << file << " is not found. Please enter a valid file name" << endl;
		return false; 
	}

	test.close(); 

	return true; 
}

// Sample main for program spell_check
int main(int argc, char **argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <document-file> <dictionary-file>" << endl;
    return 0;
  }
  
  const string document_filename(argv[1]);
  const string dictionary_filename(argv[2]);

  // Stores file names into a vector so it would be easier to check if they exist 
  vector<string> files; 
  files.push_back(document_filename); 
  files.push_back(dictionary_filename); 

  // Loop to test each file to see if it exists 
  for (unsigned int i = 0; i < 2; i++) {
    if (!fileExist(files[i])) {
      return 0; 
    }
  }
  
  cout << "Input document filename is " << document_filename << endl;
  cout << "Input dictionary filename is " << dictionary_filename << endl;

  // Call functions implementing the assignment requirements.
  HashTableDouble<string> quadratic_probing_table;
  create_hashtable(quadratic_probing_table, dictionary_filename); 
  check_file_for_spelling_errors(quadratic_probing_table, document_filename);

  return 0;
}
