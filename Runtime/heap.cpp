#include "heap.h"
#include "platform.h"
#include "runtime.h"


namespace r {

	Heap::Heap() {
		int size = 0x2000;
		_space = Platform::AllocateMemory(size, false);
		_allocationTop = reinterpret_cast<unsigned int>(&_space);
		_allocationLimit = _allocationTop + size;
	}
	

}