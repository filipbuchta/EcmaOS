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

	class Object {

	};

	class Number : Object {
	public:
		Number(double value) { _value = value; }


		static const int ValueOffset = 0;
		static const int Size = ValueOffset + sizeof(double);

	private:
		double _value;
	};

	class String : Object {

	};

	class Boolean : Object {

	};

	class Heap
	{
	public:
		Heap::Heap();
		Boolean * GetTrueValue() { return _trueValue; }
		Boolean * GetFalseValue() { return _falseValue; }
		Object * GetUndefinedValue() { return _undefinedValue; }
		void CreateInitialObjects();
		unsigned int  GetAllocationTop() { return (unsigned int)(&_allocationTop); }

	private:
		Boolean * _trueValue;
		Boolean * _falseValue;
		Object * _undefinedValue;
		
		unsigned int _allocationTop;
		unsigned char * _space;
	};
}

