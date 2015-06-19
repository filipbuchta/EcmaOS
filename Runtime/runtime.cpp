#include "runtime.h"
#include "list.h"
#include "utils.h"

namespace r {

	//TODO: remove heap argument and create executioncontext
	//JSObject * Runtime::Allocate(Heap * heap, TypeSymbol * type) {
	//	heap->GetAllocationTop();
	//	
	//	/*unsigned int top = _heap->GetAllocationTop();
	//	_assembler->Mov(result, top);
	//	_assembler->Add(Operand(result, 0), size);
	//	_assembler->Mov(result, Operand(result, 0));*/

	//	return nullptr;
	//}

	void Runtime::Console_log(HeapObject * number)
	{

		double value = *reinterpret_cast<double*>(reinterpret_cast<char*>(number) + HeapObject::PropertyTableOffset + 0); //TODO: use proper offset from propertysymbol
		const char * str = dtoa(value);

		Platform::Print(str);

		return;
	}

}