#include "heap.h"
#include "platform.h"


namespace r {

	Heap::Heap() {
		_space = Platform::AllocateMemory(0x2000, false);
		_allocationTop = (unsigned int)&_space[0];

		CreateInitialObjects();
	}
	
	void Heap::CreateInitialObjects() {

		_trueValue = new Boolean();
		_falseValue = new Boolean();
	}
}