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

	class TypeSymbol;

#pragma pack(push, 1)
	
	class HeapObject {
	public:
		TypeSymbol * TypeHandle;

		static const unsigned int TypeHandleOffset = 0;
		static const unsigned int PropertyTableOffset = TypeHandleOffset + sizeof(unsigned int);

		static const unsigned int HeaderSize = sizeof(unsigned int); // TypeHandle
	};
	
	class HeapArray : public HeapObject {
	public:
		int Length;
	};

	class HeapCharArray : public HeapArray {
	public:
		int Values[];
	};


	class HeapString : public HeapObject {
	public:
		HeapCharArray * Chars;
	};

	
#pragma pack(pop)

	class Heap
	{
	public:
		Heap::Heap();

		unsigned int * GetAllocationTopPointer() { return &_allocationTop; }
		unsigned int GetAllocationLimit() { return _allocationLimit; }
		unsigned int GetAllocatedBytes() { return _allocationLimit - _allocationTop; }

	private:

		unsigned int _allocationTop;
		unsigned int _allocationLimit;
		void * _space;
	};
}

