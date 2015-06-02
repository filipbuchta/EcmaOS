#include "heap.h"
#include "platform.h"
#include "runtime.h"


namespace r {

	Heap::Heap() {
		_space = Platform::AllocateMemory(0x2000, false);
		_allocationTop = reinterpret_cast<unsigned int>(&_space[0]);

		CreateInitialObjects();
	}
	

	HeapObject * Heap::Allocate(int size) {

		unsigned int top = _allocationTop;
		_allocationTop += size;

		return (HeapObject *)top;
	}

	void Heap::CreateInitialObjects() {

		_trueValue = (Boolean *)Allocate(Boolean::Size);
		_falseValue = (Boolean *)Allocate(Boolean::Size);

		_undefinedValue = new HeapObject();


		_runtime_log = (JSFunction *)Allocate(JSFunction::Size);
		
		unsigned int * address = reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(_runtime_log) + JSFunction::EntryOffset);
		unsigned int fun = (unsigned int)&Runtime::DebugPrint;

		*address = fun;

	}
}