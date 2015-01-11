#ifdef ECMA

#include "platform.h"

void __cdecl ConsoleWrite(const char* text);


namespace v8 {

	void Platform::Print(const char *value) {
		ConsoleWrite(value);
	}


}
#endif