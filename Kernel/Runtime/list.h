#pragma once

#include "global.h"


template <class T>
class List {

public:
	List();
	List(const List<T>& copy);
	~List();

	List<T>& operator=(const List<T>& copy);

	void Push(const T& value);
		void Insert(int position, const T & value);
	T Pop();

	T* GetBuffer() { return _buffer; }
	int GetSize() { return _size; }

	T* begin() { return _size > 0 ? &_buffer[0] : nullptr; }
	T* end() { return _size > 0 ? &_buffer[_size] : nullptr; }

	T Get(int index);


private:
	int _size;
	int _capacity;

	void Reserve(int capacity);

	T* _buffer;


};

template<class T>
List<T>::~List() {
	//delete[] _buffer;
}


template<class T>
List<T>::List()
	: _capacity(0),
	_size(0),
	_buffer(nullptr)
{
}

template<class T>
List<T>::List(const List<T> &copy)
{
	_capacity = copy._capacity;
	_size = copy._size;
	_buffer = new T[_capacity];
	for (int i = 0; i < _size; i++) {
		_buffer[i] = copy._buffer[i];
	}
}

template<class T>
List<T> & List<T>::operator=(const List<T> &copy) {
	_capacity = copy._capacity;
	_size = copy._size;
	_buffer = new T[_capacity];
	for (int i = 0; i < _size; i++) {
		_buffer[i] = copy._buffer[i];
	}

	return *this;
}

template <class T>
T List<T>::Pop() {
	if (_size == 0) {
		return T();
	}
	_size--;
	return _buffer[_size];
}

template <class T>
void List<T>::Push(const T& value) {
	if (_size >= _capacity) {
		Reserve(_size + 8);
	}

	_buffer[_size] = value;
	_size++;
}

template <class T>
void List<T>::Insert(int position, const T& value) {
	if (_size >= _capacity) {
		Reserve(_size + 8);
	}

	for (int i = _size; i > position; i--) {
		_buffer[i] = _buffer[i - 1];
	}

	_buffer[position] = value;
	_size++;
}



template <class T>
void List<T>::Reserve(int capacity) {
	if (capacity <= _capacity) {
		return;
	}

	_capacity = capacity;
	T* buffer = (T*) malloc(sizeof(T) * capacity);

	for (int i = 0; i < _size; i++) {
		buffer[i] = _buffer[i];
	}
//	delete[_size - 1] _buffer;
	_buffer = buffer;
}


template <class T>
T List<T>::Get(int index) {
	return _buffer[index];
}