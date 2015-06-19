#include "heap.h"
#include "platform.h"
#include "runtime.h"


namespace r {

	Heap::Heap() {
		_space = Platform::AllocateMemory(0x2000, false);
		_allocationTop = reinterpret_cast<unsigned int>(&_space[0]);
	}
	

	HeapObject * Heap::Allocate(int size) {

		unsigned int top = _allocationTop;
		_allocationTop += size;

		return (HeapObject *)top;
	}


}