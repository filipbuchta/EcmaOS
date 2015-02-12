#pragma once


typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef unsigned int     size_t;
typedef int              ptrdiff_t;
typedef int              intptr_t;
typedef unsigned int     uintptr_t;


extern "C" void *__cdecl memcpy(void *dest, const void *src, size_t count);
extern "C" int __cdecl strcmp(const char * str1, const char * str2);

#ifdef WIN32
	#include <memory>
#else
	extern "C" void * __cdecl malloc(size_t _Size);
	extern "C" void * __cdecl realloc(void * block, size_t size);
#endif


