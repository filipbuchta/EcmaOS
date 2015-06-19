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
		static const unsigned int TypeHandleOffset = 0;
		static const unsigned int PropertyTableOffset = TypeHandleOffset + sizeof(unsigned int);

		static const unsigned int HeaderSize = sizeof(unsigned int); // TypeHandle
	};

	class Heap
	{
	public:
		Heap::Heap();

		HeapObject * Heap::Allocate(int size);

		unsigned int * GetAllocationTop() { return &_allocationTop; }

	private:
		
		unsigned int _allocationTop;
		unsigned char * _space;
	};
}

