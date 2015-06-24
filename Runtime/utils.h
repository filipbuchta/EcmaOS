#pragma once



#define offsetof(s,m) ((size_t)&reinterpret_cast<char const volatile&>((((s*)0)->m)))


unsigned int StringLength(const char * string);