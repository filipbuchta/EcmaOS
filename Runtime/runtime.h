#pragma once


#include "platform.h"
#include "heap.h"
#include "symbol.h"


namespace r {


	class Runtime
	{
	public:		
		void Runtime_YieldTo(HeapObject * stackId);

		//static * Runtime::Allocate(Heap * heap, TypeSymbol * type);
		static void Console_log(HeapString * value);
	};


}