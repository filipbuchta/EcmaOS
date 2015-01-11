#pragma once

namespace r {

	class Platform {
	public:
		static void __cdecl Fatal(const char* file, int line, const char* format, ...);
		static void __cdecl Print(const char *value);
	};

}