#pragma once


#include "platform.h"
#include "heap.h"


namespace r {


	class Runtime
	{
	public:
		static const unsigned int Undefined = 0xCCCCCCCC;
		static void Runtime::DebugPrint(Number * value);
	};


}