
#include "CppUnitTest.h"
#include "../../Kernel/Runtime/platform.h"
#include <iostream>
#include <string> 
#include <sstream> 
#include <stdarg.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

wchar_t* toWChar(const char* value) {
	const size_t cSize = strlen(value) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, value, cSize);
	return wc;
}

namespace r {

	void Platform::Print(const char *value) {
		std::cout << value;
	}

	void Platform::Fatal(const char* file, int line, const char* format, ...) {

		int size = ((int)strlen(format)) * 2 + 50;   // Use a rubric appropriate for your code
		std::string str;
		va_list ap;
		while (1) {     // Maximum two passes on a POSIX system...
			str.resize(size);
			va_start(ap, format);
			int n = vsnprintf((char *)str.data(), size, format, ap);
			va_end(ap);
			if (n > -1 && n < size) {  // Everything worked
				str.resize(n);
				break;
			}
			if (n > -1)  // Needed size returned
				size = n + 1;   // For null char
			else
				size *= 2;      // Guess at a larger size (OS specific)
		}



		__LineInfo * lineInfo = new __LineInfo(toWChar(file), "", line);

		Assert::Fail(toWChar(str.c_str()), lineInfo);
	}
}
