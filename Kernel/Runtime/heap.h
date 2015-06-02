#pragma once

namespace r {

	template <typename T>
	class Handle {
	public:
		explicit Handle(T* location) { _location = location; }
		T* GetLocation() { return _location; }
	private:
		T* _location;
	};

	class HeapObject {
	public:
		static const int HeaderOffset = 0;
	};

	class JSObject : public HeapObject
	{
		
	};

	class JSFunction : public JSObject {
	public:
		static const int EntryOffset = HeapObject::HeaderOffset;
		static const int Size = EntryOffset + sizeof(int);
	};

	class Number : public HeapObject {
	public:
		static const int ValueOffset = HeapObject::HeaderOffset;
		static const int Size = ValueOffset + sizeof(double);

	};

	class String : HeapObject {

	};

	class Boolean : HeapObject {
	public:
		static const int ValueOffset = HeapObject::HeaderOffset;
		static const int Size = ValueOffset + sizeof(bool);
	};

	class Heap
	{
	public:
		Heap::Heap();

		JSFunction * GetRuntime_Log() { return _runtime_log; }

		Boolean * GetTrueValue() { return _trueValue; }
		Boolean * GetFalseValue() { return _falseValue; }
		HeapObject * GetUndefinedValue() { return _undefinedValue; }

		HeapObject * Heap::Allocate(int size);

		void CreateInitialObjects();
		unsigned int GetAllocationTop() { return (unsigned int)(&_allocationTop); }

	private:
		Boolean * _trueValue;
		Boolean * _falseValue;
		HeapObject * _undefinedValue;
		
		JSFunction * _runtime_log;

		unsigned int _allocationTop;
		unsigned char * _space;
	};
}

