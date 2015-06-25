

#include "../Runtime/platform.h"
#include "video.h"
#include "memory.h"
#include "debug.h"

typedef char* va_list;
#define va_start(ap,v)  (ap = (va_list)&v + sizeof(v))
#define va_arg(ap,t)    (*(t *)((ap += sizeof(t)) - sizeof(t)))
#define va_end(ap)      (ap = (va_list)0)


namespace r {

	unsigned char * Platform::AllocateMemory(int size, bool executable) {
		return (unsigned char *)malloc(size);
	}

	void Platform::Print(const char *value) {
		ConsoleWrite(value);
	}


	void Platform::Fatal(const char* file, int line, const char* format, ...) {

		Print(file);
		Print("\n");

		Print(Int32ToString(line));
		Print("\n");

		Print(format);
		Print("\n");

		va_list ap;

		va_start(ap, format);
		if (ap != nullptr) {
			Print(va_arg(ap, const char*));
		}
		va_end(ap);

		Print("\n");

		_asm { hlt }
	}
}