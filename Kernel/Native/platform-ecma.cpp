

#include "../Runtime/platform.h"
#include "video.h"
#include "memory.h"


namespace r {

	unsigned char * Platform::AllocateMemory(int size, bool executable) {
		return (unsigned char *)malloc(size);
	}

	void Platform::Print(const char *value) {
		ConsoleWrite(value);
	}

	void Platform::Fatal(const char* file, int line, const char* format, ...) {
		Print("Fatal error");
	}
}