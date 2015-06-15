#pragma once


#include "platform.h"
#include "heap.h"


namespace r {


	class Runtime
	{
	public:
		static HeapObject * Runtime::Console_log(Number * value);
	};


}