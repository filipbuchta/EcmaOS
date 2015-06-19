#include "memory.h"


unsigned char *heap = (unsigned char*)0x100000;

void *__cdecl operator new[](size_t count) {
	return malloc(count);
}

void *__cdecl operator new(size_t count){
	return malloc(count);
}

void * __cdecl malloc(size_t size) {
	char * allocated = (char*)heap;
	heap += size;
	return allocated;
}

void __cdecl operator delete(void *) {

}

extern "C" void *__cdecl memset(void * ptr, int value, size_t num) {
	unsigned char *memory = (unsigned char *)ptr;

	int c = num;
	while (c > 0) {
		*memory = value;
		memory++;
		c--;
	}

	return ptr;
}

extern "C" void *__cdecl memcpy(void *dest, const void *src, size_t count) {
	int c = count;

	char * source = (char*)src;
	char * destionation = (char*)dest;

	while (c > 0) {
		*destionation++ = *source++;
		c--;
	}
	return dest;
}

extern "C" int __cdecl _purecall()
{
	// Do nothing or print an error message.
	return 0;
}

extern "C" int __cdecl strcmp(const char * str1, const char * str2) {
	const unsigned char *p1 = (const unsigned char *)str1;
	const unsigned char *p2 = (const unsigned char *)str2;

	while (*p1 != '\0') {
		if (*p2 == '\0') return  1;
		if (*p2 > *p1)   return -1;
		if (*p1 > *p2)   return  1;

		p1++;
		p2++;
	}

	if (*p2 != '\0') return -1;

	return 0;
}