/*****************************************
 ** This file contains the hash table class for project 4.
 ** This hash table class contains insertion and removal
 ** methods, as well as linear probing for collision handling.
 **
 ***********************************************/

// Templated, hash table implementation.  Each buckets is a heap.  A
// bucket contains objects with the same key values stored as a
// max-heap based on priority.  Collisions are resolved by linear
// probing.

// To work with Heap and HashTable, the template class T must provide
// the following:
//  (a) Default constructor
//  (b) priority function (unsigned priority())
//  (c) key function (string key())
//  (d) overloaded insertion operator

#ifndef _HASH_H
#define _HASH_H

#include <string>
#include "heap.h"

using std::string;

// Hash function typedef.  The hash function must return an 'unsigned
// int' value.  Reduction mod N is the responsiblity of the caller,
// not the hash function.
typedef unsigned (*hash_fn)(string);

class Grader;

template <class T>
class HashTable {

  friend Grader;

 public:
  // Constructor.  Requires table size and hash function.
  HashTable(unsigned size, hash_fn hash);

  // Destructor, copy, and assignment
  ~HashTable();
  HashTable(const HashTable<T>& ht);
  const HashTable<T>& operator=(const HashTable<T>& ht);

  // In-line functions

  // Table size; set by constructor
  unsigned tableSize() const { return _N; }
  // Number of entries in the table
  unsigned numEntries() const { return _n; }
  // Load factor
  float lambda() const { return ((float) _n) / _N; }

  // Main hash table functions

  // insert returns 'true' if successful; 'false' otherwise
  bool insert(const T& object);
  // getNext retrieves **and removes** the highest priority order of
  // type indicated by key.  It returns 'true' if successful; 'false'
  // otherwise.
  bool getNext(string key, T& obj);

  // Dump the entire hash table
  void dump() const;

 private:
  unsigned _N;       // hash table size
  unsigned _n;       // current number of entries
  hash_fn _hash;     // hash function
  Heap<T> *_table;   // array of heaps

  // ***********************************************
  // Private helper function declarations go here! *
  // ***********************************************
};

// *****************************************
// Templated function definitions go here! *
// *****************************************
template <class T>
HashTable<T> :: HashTable(unsigned size, hash_fn hash)
{
	_hash = hash;
	_N = size;
	_n = 0;
	_table = new Heap<T>[_N];
}
template <class T>
HashTable<T> :: ~HashTable()
{
	delete[] _table;
	_N = 0;
	_n = 0;
}
template <class T>
HashTable<T> :: HashTable(const HashTable<T>& ht)
{
	_hash = ht._hash;
	_N = ht.tableSize();
	_n = ht.numEntries();
	_table = new Heap<T>[_N];
	int size = _N;
	// Copies all of the hash entries of rhs
	for(int i = 0; i < size; i++)
	{
		_table[i] = ht._table[i];
	}

}
template <class T>
const HashTable<T>& HashTable<T> :: operator=(const HashTable<T>& ht)
{
	if(_table == ht._table)
	{
		delete[] _table;
		throw std::logic_error("ERROR: Self-assignment is not allowed");
	}
	// Overwrite should not make any leaks
	delete[] _table;
	_hash = ht._hash;
	_N = ht.tableSize();
	_n = ht.numEntries();
	_table = new Heap<T>[_N];
	int size = _N;
	// Copies all of the hash entries of rhs
	for(int i = 0; i < size; i++)
	{
		_table[i] = ht._table[i];
	}
	return *this;
}
template <class T>
bool HashTable<T> :: insert(const T& object)
{
	int ind = (*_hash)(object.key()) % _N;
	int size = _N;
	// Inserts object if index is empty/has a heap with the same key
	if((_table[ind].empty())||(_table[ind].readTop().key() == object.key()))
	{
		_table[ind].insert(object);
		_n++;
		return true;
	}
	else
	{
		int hold = ind;
		// Linearly probes the index to avoid collision
		while(_table[ind].used())
		{
			ind++;
			// Mods the index if it extends beyond the array
			if(ind >= size)
			{
				ind %= _N;
			}
			// Returns false if the hash map is full
			if(ind == hold)
			{
				return false;
			}
		}
		_table[ind].insert(object);
		_n++;
		return true;
	}
	return false;
}

template <class T>
bool HashTable<T> :: getNext(string key, T& obj)
{
	int ind = (*_hash)(key) % _N;
	int size = _N;
	if((!_table[ind].empty())&&(_table[ind].readTop().key() == key))
		{
			obj = _table[ind].readTop();
			_table[ind].removeTop();
			_n--;
			return true;
		}
	else
	{
		for(int i = 0; i < size; i++)
		{
			if(!_table[i].empty())
			{
				if (_table[i].readTop().key() == key)
				{
					obj = _table[i].readTop();
					_table[i].removeTop();
					_n--;
					return true;
				}
			}
		}
		return false;
	}
}

template <class T>
void HashTable<T> :: dump() const
{
	int size = _N;
	for(int i = 0; i < size; i++)
	{
		cout << "[" << i << "]: " << endl;
		 _table[i].dump();
	}
}
#endif
