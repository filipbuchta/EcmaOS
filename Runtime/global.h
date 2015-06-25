#pragma once


typedef signed char        int8;
typedef short              int16;
typedef int                int32;
typedef long long          int64;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

typedef int              intptr;
typedef unsigned int     uintptr;


extern "C" void *__cdecl memcpy(void *dest, const void *src, size_t count);
extern "C" int __cdecl strcmp(const char * str1, const char * str2);

#ifdef WIN32
	#include <memory>
#else
	extern "C" void * __cdecl malloc(size_t _Size);
	extern "C" void * __cdecl realloc(void * block, size_t size);
#endif


