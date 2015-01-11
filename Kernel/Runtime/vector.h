#pragma once

#include "stdlib.h"


template <class T>
class Vector {

public:
	Vector();
	Vector(const Vector<T>& copy);
	~Vector();

	Vector<T>& operator=(const Vector<T>& copy);

	void Push(const T& value);
	T Pop();

	T* GetBuffer() { return _buffer; }
	int GetSize() { return _size; }

	T* begin() { return _size > 0 ? &_buffer[0] : nullptr; }
	T* end() { return _size > 0 ? &_buffer[_size] : nullptr; }


private:
	int _size;
	int _capacity;

	void Reserve(int capacity);

	T* _buffer;


};

template<class T>
Vector<T>::~Vector() {
	delete[] _buffer;
}


template<class T>
Vector<T>::Vector()
	: _capacity(0),
	_size(0),
	_buffer(nullptr)
{
}

template<class T>
Vector<T>::Vector(const Vector<T> &copy)
{
	_capacity = copy._capacity;
	_size = copy._size;
	_buffer = new T[_capacity];
	for (int i = 0; i < _size; i++) {
		_buffer[i] = copy._buffer[i];
	}
}

template<class T>
Vector<T> & Vector<T>::operator=(const Vector<T> &copy) {
	_capacity = copy._capacity;
	_size = copy._size;
	_buffer = new T[_capacity];
	for (int i = 0; i < _size; i++) {
		_buffer[i] = copy._buffer[i];
	}

	return *this;
}

template <class T>
T Vector<T>::Pop() {
	if (_size == 0) {
		return nullptr;
	}
	_size--;
	return _buffer[_size];
}

template <class T>
void Vector<T>::Push(const T& value) {
	if (_size >= _capacity) {
		Reserve(_size + 8);
	}

	_buffer[_size] = value;
	_size++;
}

template <class T>
void Vector<T>::Reserve(int capacity) {
	if (capacity <= _capacity) {
		return;
	}

	_capacity = capacity;
	T* buffer = new T[capacity];

	for (int i = 0; i < _size; i++) {
		buffer[i] = _buffer[i];
	}
	delete[] _buffer;
	_buffer = buffer;
}
