#pragma once


#include "platform.h"
#include "heap.h"
#include "symbol.h"


namespace r {


	class Runtime
	{
	public:		
		//static * Runtime::Allocate(Heap * heap, TypeSymbol * type);
		static void Runtime::Console_log(HeapObject * value);
	};


}