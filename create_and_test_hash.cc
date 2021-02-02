/*
	Homework #3
	Student: Vishnu Rampersaud
	Class: CSCI 335
	Professor: Stamos
	Due Date: November, 05, 2019

	This is the create_and_test_hash.cc file.
	It is a file to check hash table information for Part(A) and Part(B) of Homework #3
*/

#include "quadratic_probing.h"
#include "linear_probing.h"
#include "double_probing.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Function Declarations 

// Gathers statistics about the hash table such as: 
// number of collisions, avg number of collisions, number of items, 
// load factor, and the size of the hash table
// Also calls create_hashtable() to create the hash table 
// Calls check_query() to search for queries
template <typename HashTableType>
void TestFunctionForHashTable(HashTableType&, const string&, const string&); 

// Creates a hash table of your choice using the file passed to it
// HashTableType: the hash table and type of hash table you want to create 
template <typename HashTableType>
void create_hashtable(HashTableType&, const string&); 

// Checks the words in a file, and matches them to the hash table that was created
// If it was found, it displays "found", and the number of probes it took to find 
// If it was not found, it displays "not found", and the number of probes it took to search
template <typename HashTableType>
void check_query(HashTableType&, const string&); 

// End of function declaration 


// Gathers statistics about the hash table such as: 
// number of collisions, avg number of collisions, number of items, 
// load factor, and the size of the hash table
// Also calls create_hashtable() to create the hash table 
// Calls check_query() to search for queries
template <typename HashTableType>
void TestFunctionForHashTable(HashTableType &hash_table, const string &words_filename, const string &query_filename) {
 
  cout << "TestFunctionForHashTables..." << endl;
  cout << "Words filename: " << words_filename << endl;
  cout << "Query filename: " << query_filename << endl;
  cout << endl;

  create_hashtable(hash_table, words_filename);

  cout << "Statistics for this Hash Table: " << endl;
  cout << "Collisions: " << hash_table.get_number_of_collisions() << endl;
  cout << "Number of items: " << hash_table.get_number_of_items() << endl;
  cout << "Size of hash table: " << hash_table.get_size_of_hash_table() << endl;
  cout << "Load factor: " << hash_table.get_load_factor() << endl;
  cout << "Avg. number of collisions: " << hash_table.get_average_number_of_collisions() << endl;
  cout << endl;

  cout << "Beginning search for query words: " << endl;
  check_query(hash_table, query_filename); 

}

// Creates a hash table of your choice using the file passed to it
// HashTableType: the hash table and type of hash table you want to create 
template <typename HashTableType>
void create_hashtable(HashTableType &hash_table, const string &words_filename) {
	ifstream input_file; 
	input_file.open(words_filename); 
	
	string line;  
	
	hash_table.MakeEmpty();

	while (!input_file.eof()) {
		getline(input_file, line); 
		if (line.empty()) {continue;}
		hash_table.Insert(line); 
	}

	input_file.close(); 	
}

// Checks the words in a file, and matches them to the hash table that was created
// If it was found, it displays "found", and the number of probes it took to find 
// If it was not found, it displays "not found", and the number of probes it took to search
template <typename HashTableType>
void check_query(HashTableType& hash_table, const string& query_filename) {
	ifstream input_file;
	input_file.open(query_filename);

	string word; 

	while (!input_file.eof()) {
		getline(input_file, word); 
		if (word.empty()) { continue; }

		bool found = hash_table.search(word); 

		if (found) {
			cout << word << ": Found in " << hash_table.get_number_of_probes() << " probes" << endl;
		}
		else {
			cout << word << ": Not Found in " << hash_table.get_number_of_probes() << " probes" << endl;
		}
	}

	input_file.close();
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

// Main for program create_and_test_hash
int
main(int argc, char **argv) {
  if (argc != 4) {
    cout << "Usage: " << argv[0] << " <wordsfilename> <queryfilename> <flag>" << endl;
    return 0;
  }
  
  
  const string words_filename(argv[1]);
  const string query_filename(argv[2]);
  const string param_flag(argv[3]);
  
  // Stores file names into a vector so it would be easier to check if they exist 
  vector<string> files; 
  files.push_back(words_filename); 
  files.push_back(query_filename); 

  // Loop to test each file to see if it exists 
  for (unsigned int i = 0; i < 2; i++) {
    if (!fileExist(files[i])) {
      return 0; 
    }
  }

  if (param_flag == "linear") {
    HashTableLinear<string> linear_probing_table;
	cout << "\nLinear -----> \n" << endl;
    TestFunctionForHashTable(linear_probing_table, words_filename, query_filename);    
  } else if (param_flag == "quadratic") {
    HashTable<string> quadratic_probing_table;
	cout << "\nQuadratic -----> \n" << endl;
    TestFunctionForHashTable(quadratic_probing_table, words_filename, query_filename); 
  } else if (param_flag == "double") {
    HashTableDouble<string> double_probing_table;
	cout << "\nDouble Hashing -----> \n" << endl;
    TestFunctionForHashTable(double_probing_table, words_filename, query_filename);    
  } else {
    cout << "Uknown tree type " << param_flag << " (User should provide linear, quadratic, or double)" << endl;
  }
  return 0;
}
