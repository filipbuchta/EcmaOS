#pragma once


#include "platform.h"



namespace r {


	class Runtime
	{
	public:
		static const unsigned int Undefined = 0xCCCCCCCC;
		static void __cdecl Runtime::DebugPrint(int value);

	};


}