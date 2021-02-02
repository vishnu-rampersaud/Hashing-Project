/*
	Homework #3
	Student: Vishnu Rampersaud
	Class: CSCI 335
	Professor: Stamos
	Due Date: November, 05, 2019

	This is the double_probing.h file.
	It is a file to implement a double hashing hash table for Part(B) of Homework #3
*/
#ifndef DOUBLE_PROBING_H
#define DOUBLE_PROBING_H

#include "quadratic_probing.h"
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream> 


// Double hashing probing implementation.
template <typename HashedObj>
class HashTableDouble {
public:
	// Type to keep track of the status of each entry in the hash table
	enum EntryType { ACTIVE, EMPTY, DELETED };

	// Constructor to make Hash table empty and initialize data variables
	explicit HashTableDouble(size_t size = 101) : array_(NextPrime(size))
	{
		MakeEmpty();
		collision_counter = 0;
		probing_search_count = 0;
	}

	// searches for 'x' in the hash table 
	bool Contains(const HashedObj& x) const {
		return IsActive(FindPos(x));
	}

	// Emptys the hash table 
	void MakeEmpty() {
		current_size_ = 0;
		for (auto& entry : array_)
			entry.info_ = EMPTY;
	}

	// Insert 'x' into a hash table if it is not there already 
	bool Insert(const HashedObj& x) {
		// Insert x as active
		size_t current_pos = FindPos(x);
		if (IsActive(current_pos))
			return false;

		array_[current_pos].element_ = x;
		array_[current_pos].info_ = ACTIVE;

		// Rehash; see Section 5.5
		if (++current_size_ > array_.size() / 2)
			Rehash();
		return true;
	}

	// Insert 'x' into a hash table if it is not there already 
	bool Insert(HashedObj&& x) {
		// Insert x as active
		size_t current_pos = FindPos(x);
		if (IsActive(current_pos))
			return false;

		array_[current_pos] = std::move(x);
		array_[current_pos].info_ = ACTIVE;

		// Rehash; see Section 5.5
		if (++current_size_ > array_.size() / 2)
			Rehash();

		return true;
	}

	// Remove 'x' from the hash table; set its info to deleted
	bool Remove(const HashedObj& x) {
		size_t current_pos = FindPos(x);
		if (!IsActive(current_pos))
			return false;

		array_[current_pos].info_ = DELETED;
		return true;
	}

	// Returns number of items in the hash table 
	int get_number_of_items()
	{
		return current_size_;
	}

	// Returns the size of the hash table
	int get_size_of_hash_table()
	{
		return array_.size();
	}

	// returns the load factor of the hash table 
	float get_load_factor()
	{
		float array_size = array_.size();
		return (current_size_ / array_size);
	}

	// returns the number of collissions that occurred 
	int get_number_of_collisions()
	{
		return collision_counter;
	}

	// Calculate and return the average number of collissions that occurred
	double get_average_number_of_collisions()
	{
		return (static_cast<double>(collision_counter) / current_size_);
	}

	// Return number of probes encountered during a search 
	int get_number_of_probes()
	{
		return (probing_search_count);
	}

	// Search for x in the hash table 
	bool search(HashedObj& x)
	{
		size_t temp_pos;

		temp_pos = search_table(x);

		if (IsActive(temp_pos)) {
			return true;
		}

		return false;
	}

private:

	// Keeps track of the element and info of each entry in the hash table
	struct HashEntry {
		HashedObj element_;
		EntryType info_;

		HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
			:element_{ e }, info_{ i } { }

		HashEntry(HashedObj&& e, EntryType i = EMPTY)
			:element_{ std::move(e) }, info_{ i } {}
	};

	// Private data variables 
	std::vector<HashEntry> array_;
	size_t current_size_;
	mutable int collision_counter;
	mutable int probing_search_count;

	// Returns true if the hash table entry is active 
	bool IsActive(size_t current_pos) const
	{
		return array_[current_pos].info_ == ACTIVE;
	}

	// Finds the position of 'x' if it is in the hash table and returns it 
	// If it is not in the hash table, it uses double hashing to find the next available 
	// position that 'x' can be inserted into. 
	// Double hashing: 
	// - Calculate the first hash function 
	// - Calculate the second hash function
	// - Set the current position according to the formula: 
	// - ( first_hash_function + offset * second_hash_function ) % (table_size)
	// - increment the offset
	// - if the new current position contains 'x', then return it
	// - if the new current position is empty, then return it 
	// - counts the number of collissions encountered in this process
	size_t FindPos(const HashedObj& x) const {
		size_t offset = 1; 
		size_t current_pos = InternalHash(x);
		size_t hash_1 = current_pos;
		size_t hash_2 = Hash2(x);

		while (array_[current_pos].info_ != EMPTY &&
			array_[current_pos].element_ != x) {
			collision_counter++;
			current_pos = ( hash_1 + offset * hash_2 ) % array_.size();
			offset++; 
			if (current_pos >= array_.size())
				current_pos -= array_.size();
		}
		return current_pos;
	}

	// Finds the position of 'x' if it is in the hash table and returns it 
	// If it is not in the hash table, it uses double hashing to find the next available 
	// position that 'x' can be inserted into. 
	// Double hashing: 
	// - Calculate the first hash function 
	// - Calculate the second hash function
	// - Set the current position according to the formula: 
	// - ( first_hash_function + offset * second_hash_function ) % (table_size)
	// - increment the offset
	// - if the new current position contains 'x', then return it
	// - if the new current position is empty, then return it 
	// - counts the number of probes encountered in this process
	size_t search_table(const HashedObj& x) const {
		size_t offset = 1;
		size_t current_pos = InternalHash(x);
		size_t hash_1 = current_pos; 
		size_t hash_2 = Hash2(x); 
		probing_search_count = 0; 
		probing_search_count++;

		while (array_[current_pos].info_ != EMPTY &&
			array_[current_pos].element_ != x) {
			probing_search_count++;
			current_pos = ( hash_1 + offset * hash_2) % array_.size();
			offset++;
			if (current_pos >= array_.size())
				current_pos -= array_.size();
		}
		return current_pos;
	}

	// Rehashes the hash table to a bigger hash table 
	void Rehash() {
		std::vector<HashEntry> old_array = array_;

		// Create new double-sized, empty table.
		array_.resize(NextPrime(2 * old_array.size()));
		for (auto& entry : array_)
			entry.info_ = EMPTY;

		// Copy table over.
		current_size_ = 0;
		for (auto& entry : old_array)
			if (entry.info_ == ACTIVE)
				Insert(std::move(entry.element_));
	}

	// Hash function to create hash key for 'x' 
	size_t InternalHash(const HashedObj& x) const {
		static std::hash<HashedObj> hf;
		return hf(x) % array_.size();
	}

	// Second hash function
	size_t Hash2(const HashedObj& x) const {
		// Using double hash function implementation 
		// hash2(X) = R - ( x % R ) 
		// R = 37
		static std::hash<HashedObj> hf;
		return (37 - (hf(x) % 37));
	}
};

#endif  // DOUBLE_PROBING_H