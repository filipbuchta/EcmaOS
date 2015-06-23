#pragma once



#define offsetof(s,m) ((size_t)&reinterpret_cast<char const volatile&>((((s*)0)->m)))


double atod(const char * ch);
const char * itoa(int value);
const char * dtoa(double value);
