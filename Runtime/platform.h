#pragma once

namespace r {

	class Platform {
	public:
		static void Fatal(const char* file, int line, const char* format, ...);
		static void Print(const char *value);
		static unsigned char * AllocateMemory(int size, bool executable);
	};

}