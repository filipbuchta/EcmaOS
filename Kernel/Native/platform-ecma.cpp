

#include "../Runtime/platform.h"
#include "video.h"


namespace r {

	void Platform::Print(const char *value) {
		ConsoleWrite(value);
	}

	void Platform::Fatal(const char* file, int line, const char* format, ...) {
		Print("Fatal error");
	}
}