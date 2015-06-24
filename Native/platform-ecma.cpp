

#include "../Runtime/platform.h"
#include "video.h"
#include "memory.h"

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


	const char * Int32ToString(int value) {

		const int sizes[] = { 9, 99, 999, 9999, 99999, 999999, 9999999, 99999999, 999999999 };
		const char table[] = "0123456789";

		if (value < 0) {
			value *= -1;
		}

		int size = 0;
		for (int i = 0; i < 10; i++) {
			if (value < sizes[i]) {
				size = i + 1;
				break;
			}
		}

		char * result = new char[size + 1];

		if (value == 0) {
			result[0] = '0';
		}
		else {

			int i = size - 1;
			while (value > 0) {
				result[i] = table[value % 10];
				value /= 10;
				i--;
			}
		}

		result[size] = '\0';

		return result;
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