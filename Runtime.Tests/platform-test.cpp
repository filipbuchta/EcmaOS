
#include "CppUnitTest.h"
#include "../../Kernel/Runtime/platform.h"
#include <iostream>
#include <string> 
#include <sstream> 
#include <stdarg.h>
#include <iostream>
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

wchar_t* toWChar(const char* value) {
	const size_t cSize = strlen(value) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, value, cSize);
	return wc;
}


namespace r {


	void Platform::Fatal(const char* file, int line, const char* format, ...) {

		int size = ((int)strlen(format)) * 2 + 50;
		char * buffer = new char[size];

		va_list args;
		va_start(args, format);
		vsnprintf(buffer, size, format, args);
		va_end(args);


		__LineInfo * lineInfo = new __LineInfo(toWChar(file), "", line);

		Assert::Fail(toWChar(buffer), lineInfo);
	}
}
