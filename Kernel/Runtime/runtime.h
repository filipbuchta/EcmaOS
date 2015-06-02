#pragma once


#include "platform.h"
#include "heap.h"


namespace r {


	class Runtime
	{
	public:
		static HeapObject * Runtime::DebugPrint(Number * value);
	};


}