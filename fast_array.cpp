/*

Fast Array 2.0 (fast_array) is a fast, simple and easy to use similar to C++'s std::vector or Java's ArrayList, with some small differences.



Author: Edward Alexander
Copyright: Edward Alexander ©2016
Date: August 17, 2016



Version: 2.0
Revision: 519
Code: E.A.2.519.1



Known bugs: - Throws std::length_error when using an std::string



Details available:
- size : The current position of the last element.
- total_size : The capacity of the list.



Actions available:

- void grow_array(size_t new_size);
	Grows array according to the new_size, throws if the new_size is smaller than the current size.
- void shrink_array();
	Shrinks the array to half its size, losing every entry after its half.
- void add(T entry);
	Adds an entry after the last position and increasing the array's size the one and a half its original size.
- void add_at(size_t position, T entry);
	Adds an entry at the specified position, throws if the position is smaller than 0 or bigger than the array's size.
- void remove(size_t position);
	Removes the entry at the specified position, throws if the position is smaller than 0 or bigger than the position of the last entry.
- T get(size_t position);
	Gets the entry at the specified position, throws if the position is smaller than 0 or bigger than the position of the last entry. 
- void release();
	Releases the used resources.


Guide:
	- An array can be made either with the default size or with a custom one.
	- add_at(size_t position, T entry) only works if there are at least 2 elements in the array, if so, it will add a new element after the specified position.
	- As such, add(T entry) is recommend whenever adding an element at a random position.
	- When going through elements, the size detail is the one that should be used.
	- The only time total_size should be used is to check the list's capacity.
	- Whenever growing the array, the new size has to always be bigger than the old one.
	- Whenever shrinking the array, only shrink_array() has to be used.
	- An alternative to add(T entry), add_at(size_t position, T entry) and remove(size_t position) is the [] Subscript operator.

Log:
	Version 2 : Rewrites the whole logic..
		- Revision 519 (October 7th 2016) :
			- Adds: - [] Subscript operator overload

			- Rewrites : - Changed all size_t parameters to int

		- Revision 517:
			- Adds : - Constructor defaults to 100 if the entered size is 0

		- Revision 513:
			- Adds : - Logs
					 - Guide
					 - Details

	    - Revision 317:
			- Adds : - void copy_array(T * _new_entries, T * _entries); (Internal)

		- Revision 315:
			- Rewrites : - void grow_array(size_t new_size);
						 - void shrink_array();

		- Revision 313:
			- Adds : - void grow_array(size_t new_size);
					 - void shrink_array();
					 - Revisions

			- Rewrites : - void remove(size_t position);
						 - void add(T entry);
						 - void add_at(size_t position, T entry);
			
	Version 1 : Shows the idea of the fast_array logic, formerly named advanced_array. 

Current status: Working, no bugs found.

*/

#include "stdafx.h"



template <typename T>
class fast_array
{

private:
	/// Current position in the list.
	size_t _index = 0;

	/// Current capacity of the list.
	size_t _capacity = 0;



	/// The list.
	T * _entries;



	/// Copies from _entries to _new_entries.
	void copy_array(T * _new_entries, T * _entries);

	/// Copies from _entries to _new_entries, based on the specified _size.
	void copy_array(T * _new_entries, T * _entries, size_t _size);

	/// Copies from _entries to _new_entries, leaving an empty place at _position where the new _entry will go.
	void copy_array_add(T * _new_entries, T * _entries, size_t _position);

	/// Copies from _entries to _new_entries, skipping at _position, deleting the _entry.
	void copy_array_remove(T * _new_entries, T * _entries, size_t _position);



	/// Makes a new list that's the same size as the original, afterwards it copies it and adds the new _entry.
	void add_entry(T _entry, size_t _position);

	/// Makes a new list that's one and a half the size of the original, afterwards it copies it and adds the new _entry.
	void add_entry_increase_array(T _entry, size_t _position);



	/// Makes a new list that's the same size as the original, afterwards it copies it and removes the specified _entry.
	void remove_entry(size_t _position);



	/// Sets the _entry at _position.
	void set_entry(T _entry, size_t _position);

public:
	/// Public size of the list, that gets updated whenever an _entry is added.
	size_t size = 0;

	/// Public total size of the list, that always gets updated.
	size_t total_size = 0;



	/// Default constructor, that initializes a list with 100 elements.
	fast_array();

	/// Constructor based on capacity, that initializes a list with the specified capacity.
	fast_array(size_t capacity);



	/// Default destructor which calls release().
	~fast_array();



	/// Default destructor which calls release().
	T& operator[](size_t position);



	/// Adds an _entry at the last position which is _index.
	void add(T entry);

	/// Adds an _entry at the specified position, throws if position < 0 || position > _index.
	void add_at(size_t position, T entry);



	/// Removes the _entry at the specified position, throws if position < 0 || position > _index.
	void remove(size_t position);



	/// Gets the _entry at the specified position, throws if position < 0 || position > _capacity.
	T get(size_t position);



	/// Makes a new list with the specified size, afterwards it copies it.
	void grow_array(size_t new_size);



	/// Makes a new list that's half the size of the original, afterwards it copies it.
	void shrink_array();



	/// Releases all used resources.
	void release();
};





/// Copies from _entries to _new_entries.
template <typename T>
void fast_array <T>::copy_array(T * _new_entries, T * _entries)
{
	/// _count is where it currently is.
	size_t _count = 0;

	/// Copies from the beginning till the end.
	while (_count < _capacity)
	{
		*(_new_entries + _count) = *(_entries + _count);

		_count++;
	}
}



/// Copies from _entries to _new_entries, based on the specified _size.
template <typename T>
void fast_array <T>::copy_array(T * _new_entries, T * _entries, size_t _size)
{
	/// _count is where it currently is.
	size_t _count = 0;

	/// Copies from the beginning till the end.
	while (_count < _size)
	{
		*(_new_entries + _count) = *(_entries + _count);

		_count++;
	}
}



/// Copies from _entries to _new_entries, leaving an empty place at _position where the new _entry will go.
template <typename T>
void fast_array <T>::copy_array_add(T * _new_entries, T * _entries, size_t _position)
{
	/// _before is the specific _postition, _after is next to it, _count is where it currently is.
	size_t _before = _position, _after = _position + 1, _count = 0;

	/// Copies from the beginning till the specified _position (inclusive).
	while (_count <= _before)
	{
		*(_new_entries + _count) = *(_entries + _count);

		_count++;
	}

	/// Skips at _position.
	_count = _after;

	/// Copies from _position to _position + 1 until the _capacity is full.
	while (_count < _capacity)
	{
		*(_new_entries + _count) = *(_entries + _count - 1);

		_count++;
	}
}



/// Copies from _entries to _new_entries, skipping at _position, deleting the _entry.
template <typename T>
void fast_array <T>::copy_array_remove(T * _new_entries, T * _entries, size_t _position)
{
	/// _before is before the specific _postition, _after is next to the _position, _count is where it currently is.
	size_t _before = _position - 1, _after = _position + 1, _count = 0;

	/// Copies from the beginning till the specified _position (inclusive).
	while (_count <= _before)
	{
		*(_new_entries + _count) = *(_entries + _count);

		_count++;
	}

	/// Skips after the _position.
	_count = _after;

	/// Copies from _position + 1 to _position until the _capacity is full.
	while (_count < _capacity)
	{
		*(_new_entries + _count - 1) = *(_entries + _count);

		_count++;
	}
}



/// Makes a new list that's the same size as the original, afterwards it copies it and adds the new _entry.
template <typename T>
void fast_array <T>::add_entry(T _entry, size_t _position)
{
	/// Makes a new list that's the same size as the original.
	T *_new_entries = new T[sizeof(T) * _capacity];

	( /// Copies from _entries to _new_entries, leaving an empty place at _position where the new _entry will go.
	  copy_array_add(_new_entries, _entries, _position),

	  /// Sets the _entry at _position.
	  *(_new_entries + _position) = _entry,

	  /// Deletes the old list.
	  delete[] _entries,

	  /// Updates the list.
	  _entries = _new_entries );
}



/// Makes a new list that's one and a half the size of the original, afterwards it copies it and adds the new _entry.
template <typename T>
void fast_array <T>::add_entry_increase_array(T _entry, size_t _position)
{
	/// Makes a new list that's one and a half the size of the original.
	T *_new_entries = new T[3 * _capacity / 2];

	_capacity = ( /// Copies from _entries to _new_entries, leaving an empty place at _position where the new _entry will go.
				  copy_array_add(_new_entries, _entries, _position), 

				  /// Sets the _entry at _position.
				  *(_new_entries + _position) = _entry, 

				  /// Deletes the old list.
				  delete[] _entries, 

				  /// Updates the list.
				  _entries = _new_entries, 

				  /// Increases the _capacity of the list.
		          3 * _capacity / 2 );
}



/// Makes a new list with the specified size, afterwards it copies it.
template <typename T>
void fast_array <T>::grow_array(size_t new_size)
{
	/// Makes a new list that's half the original.
	T *_new_entries = new T[new_size];

	_capacity = ( /// Copies from _entries to _new_entries.
				  copy_array(_new_entries, _entries),

				  /// Deletes the old list.
				  delete[] _entries,

				  /// Updates the list.
				  _entries = _new_entries,

				  /// Sets the _capacity of the list.
				  new_size );

	total_size = _capacity;
}



/// Makes a new list that's the same size as the original, afterwards it copies it and removes the specified _entry.
template <typename T>
void fast_array <T>::remove_entry(size_t _position)
{
	/// Makes a new list that's the same size as the original.
	T *_new_entries = new T[sizeof(T) * _capacity];

	( /// Copies from _entries to _new_entries, skipping at _position, deleting the _entry.
	  copy_array_remove(_new_entries, _entries, _position),

	  /// Deletes the old list.
	  delete[] _entries,

	  /// Updates the list.
	  _entries = _new_entries );
}



/// Makes a new list that's half the original, afterwards it copies it.
template <typename T>
void fast_array <T>::shrink_array()
{
	/// Makes a new list that's half the original.
	T *_new_entries = new T[2 * _capacity / 3];

	_capacity = ( /// Copies from _entries to _new_entries.
				 copy_array(_new_entries, _entries, 2 * _capacity / 3),

				 /// Deletes the old list.
				 delete[] _entries,

				 /// Updates the list.
				 _entries = _new_entries,

				 /// Decreases the _capacity of the list.
				 2 * _capacity / 3);

	total_size = _capacity;
}



/// Sets the _entry at _position.
template <typename T>
void fast_array <T>::set_entry(T _entry, size_t _position)
{
	/// Sets the _entry at _position.
	*(_entries + _position) = _entry;
}



/// Default constructor, that initializes a list with 100 elements.
template <typename T>
fast_array <T>::fast_array()
{
	/// Defaults the _capacity to 100.
	_capacity = total_size = 100;

	/// Makes a list with the default _capacity.
	_entries = new T[sizeof(T) * _capacity];
}



/// Constructor based on capacity, that initializes a list with the specified capacity.
template <typename T>
fast_array <T>::fast_array(size_t capacity)
{
	/// Defaults to 100, in case _capacity is 0.
	_capacity == 0 ? _capacity = 100 : false;

	/// Defaults the _capacity to the specified capacity.
	_capacity = total_size = capacity;

	/// Makes a list with the specified capacity.
	_entries = new T[sizeof(T) * _capacity];
}



/// Default destructor which calls release().
template <typename T>
fast_array <T>::~fast_array()
{
	/// Releases all used resources.
	release();
}



/// Overwrites an _entry at position.
template<typename T>
T& fast_array<T>::operator[](size_t position)
{
	/// Throws if the position is before 0.
	(int) position < 0 ? throw : false;

	/// Grows the array if the position is after the total size.
	position >= total_size ? (total_size = _capacity = 3 * _capacity / 2 + 1, grow_array(total_size)) : false;

	/// Increases the size if the position is after the last element.
	position >= size ? size = ++_index : false;
	
	/// Returns the element at the specified position.
	return _entries[position];
}



/// Adds an _entry at the last position which is _index.
template <typename T>
void fast_array <T>::add(T entry)
{
	/// Sets the entry at the latest position which is _index or increases capacity.
	_index < _capacity ? set_entry(entry, _index) : add_entry_increase_array(entry, _index);

	/// Increases the size.
	size = ++_index;
}



///Adds an _entry at the specified position, throws if position < 0 || position > _index.
template <typename T>
void fast_array <T>::add_at(size_t position, T entry)
{
	/// Throws if the position is after the latest _entry or before 0.
	position < _index && position > 0 ? true : throw;

	/// Increases the size.
	size = ++_index;

	/// Sets the entry at the specified position if there is enough space, otherwise it increases the array.
	_index < _capacity ? add_entry(entry, position) : add_entry_increase_array(entry, position);
}



///Removes the _entry at the specified position, throws if position < 0 || position > _index.
template <typename T>
void fast_array <T>::remove(size_t position)
{
	/// Throws if the position is after the latest _entry or before 0.
	position > _index || position < 0 ? throw : false;

	/// Decreases the size.
	size = --_index;

	/// Decreases the array and removes the _entry at the specified position if it's before capacity, otherwise it throws.
	position < _capacity ? remove_entry(position) : throw;

	_index <= _capacity / 2 ? shrink_array() : false;
}



///Gets the _entry at the specified position, throws if position >= 0 && position < _index.
template <typename T>
T fast_array <T>::get(size_t position)
{
	///Gets the _entry at the specified position, throws if position >= 0 && position < _index.
	return position >= 0 || position < _index ? *(_entries + position) : throw;
}



///Releases all used resources.
template <typename T>
void fast_array <T>::release()
{
	///Deletes the list.
	delete[] _entries;

	/// Deletes the pointer to the list.
	_entries = nullptr;
}