/*****************************************
 ** This file contains the heap class for project 4.
 ** This heap class contains insertion and removal
 ** methods that maintain the max-heap structure
 ** in vector form.
 **
 ***********************************************/

// To work with Heap, the template class T must provide the following:
//  (a) Default constructor
//  (b) priority function (unsigned priority())
//  (c) overloaded insertion operator
// To work with Heap *and* HashTable, it also needs:
//  (d) key function (string key())

#ifndef _HEAP_H
#define _HEAP_H

#include <iostream>
#include <vector>
#include <exception>
#include <utility>   // swap

using std::vector;
using std::endl;
using std::cout;
using std::swap;
using std::range_error;

// To work with Heap and HashTable, class T must provide the
// following:
//  (a) Default constructor
//  (b) priority function (unsigned priority())
//  (c) key function (string key())
//  (d) overloaded insertion operator

class Grader;

template <class T>
class Heap {

  friend Grader;

 public:
  // Constructor
  Heap();

  // Inline functions: size, empty, used
  unsigned size() const { return _heap.size() - 1 ; }
  bool empty() const { return _heap.size() <= 1 ; }
  bool used() const { return _used ; }

  // Main heap operations: insert, read, remove
  void insert(const T& object);
  T readTop() const;
  void removeTop();

  // Dump the entire heap
  void dump() const;

  // Root node always has index 1
  static const unsigned ROOT = 1;

 private:
  vector<T> _heap;   // Vector-based heap
  bool _used;        // Has the heap ever been used?  Needed for
		     // linear probing.

  // *********************************************
  // Private helper function declarations go here!
  // *********************************************
  void insertHelper(int ind, const T& object);
  void removeTopHelper(int ind);

};

// ***************************************
// Templated function definitions go here!
// ***************************************
template <class T> Heap<T> :: Heap()
{
	_used = false;
}
template <class T>
void Heap<T> :: insert(const T& object)
{
	if(empty())
	{
		// Pushes back twice for garbage value in 0th index
		// if heap is empty
		_heap.push_back(object);
		_heap.push_back(object);
		_used = true;
	}
	else
	{
		_heap.push_back(object);
		int ind = size();
		insertHelper(ind, object);
	}
}

template <class T>
void Heap<T> :: insertHelper(int ind, const T& object)
{
	int parent_ind = ind/2;
	// If current index is root, stops recursion
	if(ind == ROOT)
	{
		return;
	}
	// Checks if parent is lower than inserted object
	if(_heap[parent_ind].priority() < object.priority())
	{
		swap(_heap[ind], _heap[parent_ind]);
		insertHelper(parent_ind, object);
	}
}
template <class T>
void Heap<T> :: dump() const
{
	for(int i = ROOT; i < static_cast<int>(_heap.size()); i++)
	{
		cout << _heap[i] << endl;
	}

}
template <class T>
T Heap<T> :: readTop() const
{
	if(empty())
	{
		throw range_error("ERROR: Heap is empty!");
	}
	return _heap[ROOT];
}
template <class T>
void Heap<T> :: removeTop()
{
	if(empty())
	{
		throw range_error("ERROR: Heap is empty!");
	}
	_heap[ROOT] = _heap[size()];
	_heap.pop_back();
	removeTopHelper(ROOT);
}
template <class T>
void Heap<T> :: removeTopHelper(int ind)
{
	int left_ind = ind*2;
	int right_ind = ind*2 + 1;
	int last_ind = _heap.size() - 1;
	// If children has higher index, then current ind is a leaf;
	// stops recursion
	if(left_ind > last_ind)
	{
		return;
	}
	// Swaps values with left child as it trickles down
	if(_heap[ind].priority() < _heap[left_ind].priority())
	{
		swap(_heap[ind], _heap[left_ind]);
		removeTopHelper(left_ind);
	}
	// Swaps values with right child as it trickles down
	if(_heap[ind].priority() < _heap[right_ind].priority())
	{
		swap(_heap[ind], _heap[right_ind]);
		removeTopHelper(right_ind);
	}
}
#endif
