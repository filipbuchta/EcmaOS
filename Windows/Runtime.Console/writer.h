#pragma once

#include "../../Kernel/Runtime/global.h"

typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;


class Writer {
public:

	template<typename T>
	class Slot {
	public:
		Slot(Writer * writer, int offset) : _writer(writer), _offset(offset) { }

		void Set(const T & value) {
			*(T*)(&(_writer->_buffer)[_offset]) = value;
		}

		Slot<T> At(int index) {
			return Slot<T>(_writer, _offset + sizeof(T) * index);
		}

		T* operator-> () {
			return (T*)(&(_writer->_buffer)[_offset]);
		}
	private:
		Writer * _writer;
		int _offset;
	};


	Writer()
	{
		_capacity = 1024;
		_buffer = (unsigned char *)malloc(_capacity);
	}


	template <typename T>
	Slot<T> CreateSlot(int count)
	{
		EnsureCapacity(_position + sizeof(T) * count);
		int position = GetPosition();
		_position += sizeof(T) * count;
		return Slot<T>(this, position);
	}


	template <typename T>
	Slot<T> CreateSlot()
	{
		return CreateSlot<T>(1);
	}

	template <typename T>
	Slot<T> SlotAt(int offset)
	{
		return Slot<T>(this, offset);
	}

	void Write(unsigned char * value, int length) {
		for (int i = 0; i < length; i++) {
			Write(value[i]);
		}
	}

	template<typename T>
	void Write(const T& value) {
		EnsureCapacity(_position + sizeof(T));
		*(reinterpret_cast<T*>(&_buffer[_position])) = value;
		_position += sizeof(T);
	}

	void WriteSLEB128(unsigned int value) {
		bool more = true;
		while (more) {
			int8_t byte = value & 0x7F;
			bool byte_sign = byte & 0x40;
			value >>= 7;

			if ((value == 0 && !byte_sign) || (value == -1 && byte_sign)) {
				more = false;
			}
			else {
				byte |= 0x80;
			}

			Write<int8_t>(byte);
		}
	}

	void WriteULEB128(unsigned int value) {
		do {
			uint8_t byte = value & 0x7F;
			value >>= 7;
			if (value != 0) byte |= 0x80;
			Write<uint8_t>(byte);
		} while (value != 0);
	}

	void WriteString(const char * value)
	{
		do {
			Write<char>(*value);
		} while (*value++);
	}

	void Align(int align) {
		int delta = _position % align;

		int padding = align - delta;

		for (int i = 0; i < padding; i++) {
			Write<unsigned char>(0x00);
		}
	}

	void EnsureCapacity(int capacity) {
		if (_capacity < capacity) {
			while (_capacity < capacity) { _capacity *= 2; }
			_buffer = (unsigned char *)realloc(_buffer, _capacity);
		}
	}

	int GetPosition() { return _position; }

	unsigned char * GetBuffer() { return _buffer; }

private:
	unsigned char * _buffer;
	int _capacity;
	int _position;
};