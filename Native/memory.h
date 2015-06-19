#pragma once

void *__cdecl operator new[](size_t count);
void *__cdecl operator new(size_t count);
void __cdecl operator delete(void *);

extern "C" void *__cdecl memcpy(void *dest, const void *src, size_t count);
extern "C" int __cdecl strcmp(const char * str1, const char * str2);
extern "C" void *__cdecl memset(void * ptr, int value, size_t num);
extern "C" void * __cdecl malloc(size_t size);

extern "C" int __cdecl _purecall();