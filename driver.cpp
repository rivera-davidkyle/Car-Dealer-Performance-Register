/*****************************************
 ** This file contains the test drive program for project 4.
 ** This program tests exceptions, edge cases, insertions,
 ** and removals in the heap and hash table classes.
 **
 ***********************************************/

#include <iostream>
#include <ctime>
#include <iomanip>
#include "hash.h"
#include "car.h"
using namespace std;
// Prototype for 16 bit cyclic hash function; implementation is after
// main().
unsigned cyclic_hash16(string key);
unsigned linear_probe_test(string key);

int main() {
	// Max heap test: Insertion
	cout << "\nMax-heap test: Insertion 3-2-1-5-4, should return 5-4-1-2-3 in order" << endl;
	Heap<Car> test_h;
	test_h.insert(Car("mustang", 3, "muscle car"));
	test_h.insert(Car("mustang", 2, "muscle car"));
	test_h.insert(Car("mustang", 1, "muscle car"));
	test_h.insert(Car("mustang", 5, "muscle car"));
	test_h.insert(Car("mustang", 4, "muscle car"));
	test_h.dump();

	// Max heap test: Deletion
	cout << "\nMax-heap test: remove root, should return 4-3-1-2" << endl;
	test_h.removeTop();
	test_h.dump();

	// Max heap test: Exceptions
	cout << "\nMax-heap test: exceptions" << endl;
	test_h.removeTop();
	test_h.removeTop();
	test_h.removeTop();
	test_h.removeTop();
	try
	{
		test_h.removeTop();
	}
	catch(exception &e)
	{
	  cout << "\nremoveTop() exception" << endl;
	  cout << e.what() << endl;
	  try
	  {
		  test_h.readTop();
	  }
	  catch(exception &e)
	  {
		  cout << "\nreadTop() exception" << endl;
		  cout << e.what() << endl;
	  }
	}

	cout << "\nHash Table Test" << endl;
  // Create a vector of cars to insert in the hash table
  vector<Car> cars;
  cars.push_back(Car("x101", 5, "super car"));
  cars.push_back(Car("x101", 12, "mega car"));
  cars.push_back(Car("x101", 4, "shack of cars"));
  cars.push_back(Car("challenger", 10, "mega car"));
  cars.push_back(Car("challenger", 5, "car world"));
  cars.push_back(Car("stratos", 7, "car world"));
  cars.push_back(Car("stratos", 15, "super car"));
  cars.push_back(Car("stratos", 8, "shack of cars"));
  cars.push_back(Car("challenger", 3, "car joint"));
  cars.push_back(Car("gt500", 3, "shack of cars"));
  cars.push_back(Car("miura", 28, "mega car"));
  cars.push_back(Car("gt500", 11, "super car"));

  // Create the hash table of length 10 using cyclic_hash function
  HashTable<Car> ht(10, cyclic_hash16);

  vector<Car>::iterator itCar;
  for (itCar=cars.begin();itCar!=cars.end();itCar++)
  {
	  ht.insert(*itCar);
  }
  cout << "\nDump of ht:\n";
  ht.dump();

  // Extract some orders

  Car d;
  vector<string> orders;

  orders.push_back("challenger");
  orders.push_back("miura");
  orders.push_back("gt500");
  orders.push_back("gt500");
  orders.push_back("gt500");

  vector<string>::iterator itString;

    for ( itString=orders.begin();itString!=orders.end();itString++ ) {
    cout << "\nGet next '" << *itString << "' order...\n";
    if ( ht.getNext(*itString, d) ) {
      cout << "  " << d << endl;
    } else {
      cout << "  No " << *itString << " orders!\n";
    }
  }

  cout << endl;

  cout << "\nDump of ht:\n";
  ht.dump();

  // Simple test of copy constructor; also need to test assignment
  // operator!
  cout << "\nCreate copy of hash table, remove two 'stratos' orders, dump both tables...\n";

  HashTable<Car> ht2(ht);

  ht2.getNext("stratos", d);
  ht2.getNext("stratos", d);

  cout << "\nDump of ht:\n";
  ht.dump();

  cout << "\nDump of ht2:\n";
  ht2.dump();

  // Assignment copy test
  cout << "\nCreate assignment copy of hash table, remove all 'x101' orders, dump both tables..." << endl;

  HashTable<Car> ht3(5, cyclic_hash16);
  ht3 = ht;

  ht3.getNext("x101", d);
  ht3.getNext("x101", d);
  ht3.getNext("x101", d);

  cout << "\nDump of ht:\n";
  ht.dump();

  cout << "\nDump of ht3:\n";
  ht3.dump();

  // Hash collision test
  cout << "\nLinear probe test: Create new hash table with new hash fn." << endl;
  HashTable<Car> test(10, linear_probe_test);
  Car test1("mustang", 3, "muscle car");
  Car test2("maserati", 4, "super car");

  test.insert(test1);
  test.insert(test2);
  cout << "\nDump of test:\n";
  test.dump();

  // Full hash table edge case
  cout << "\nHash table full: should just return false" << endl;
  vector<Car> test_vec;
  test_vec.push_back(Car("mclaren", 4, "super car"));
  test_vec.push_back(Car("mini", 3, "mini car"));
  test_vec.push_back(Car("mazda", 2, "tuner car"));
  test_vec.push_back(Car("morgan", 3, "luxury car"));
  test_vec.push_back(Car("mercedez", 3, "luxury car"));
  test_vec.push_back(Car("mazzanti", 4, "super car"));
  test_vec.push_back(Car("mitsubishi", 2, "super car"));
  test_vec.push_back(Car("mega", 3, "micro car"));
  for(int i = 0; i < static_cast<int>(test_vec.size()); i++)
  {
	  test.insert(test_vec[i]);
  }

  if(test.insert(Car("corvette", 2, "super car")))
  {
	  cout << "\nTest Failed: returned true" << endl;
  }
  else
  {
	  cout << "\nTest Passed!" << endl;
  }
  cout << "\nDump of test:\n";
  test.dump();

  // Hash Table: getNext() on linear probed hash table
  cout << "\nHash Table: getNext() on same hash keys; remove mazda" << endl;
  test.getNext("mazda", d);
  test.dump();




  return 0;
}

unsigned cyclic_hash16(string key) {
  unsigned usize = 16;
  unsigned s = 5; // shift by 5
  unsigned h = 0;
  for ( int i = 0; i < static_cast<int>(key.length()); i++ ){
    h = (( h << s ) | ( h >> (usize - s) ));
    h += key[i];
    h = h & 0xffff;
  }
  return h;
}
unsigned linear_probe_test(string key)
{
	unsigned ascii = key[0];
	return ascii;
}
