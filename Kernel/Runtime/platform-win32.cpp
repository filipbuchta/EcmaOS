//#ifdef WIN32
//
//#include "platform.h"
//#include <iostream>
//
//#include <string> 
//#include <sstream> 
//#include <stdarg.h>
//
//
//void __cdecl FatalFail(const char* file, int line, const char* message);
//
//namespace v8 {
//	
//	void Platform::Print(const char *value) {
//		std::cout << value;
//	}
//
//	void Platform::Fatal(const char* file, int line, const char* format, ...) {
//
//		int size = ((int)strlen(format)) * 2 + 50;   // Use a rubric appropriate for your code
//		std::string str;
//		va_list ap;
//		while (1) {     // Maximum two passes on a POSIX system...
//			str.resize(size);
//			va_start(ap, format);
//			int n = vsnprintf((char *)str.data(), size, format, ap);
//			va_end(ap);
//			if (n > -1 && n < size) {  // Everything worked
//				str.resize(n);
//				break;
//			}
//			if (n > -1)  // Needed size returned
//				size = n + 1;   // For null char
//			else
//				size *= 2;      // Guess at a larger size (OS specific)
//		}
//
//		FatalFail(file, line, str.c_str());
//	}
//}
//#endif