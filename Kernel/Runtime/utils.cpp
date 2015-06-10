#include "utils.h"
#include "list.h"

extern "C" int __cdecl strcmp(const char * str1, const char * str2);


int pow(int number, int power) {
	int result = 1;
	for (int i = 0; i < power; i++) {
		result *= number;
	}
	return result;
}

//unsigned int strlen(const char * string) {
//	int length = 0;
//	while (string != 0) {
//		string++;
//		length++;
//	}
//	return length;
//}

double atod(const char * ch) {
	double result = 0;
	while (*ch >= '0' && *ch <= '9') {
		result = (result*10.0) + (*ch - '0');
		++ch;
	}
	if (*ch == '.') {
		double f = 0.0;
		int n = 0;
		++ch;
		while (*ch >= '0' && *ch <= '9') {
			f = (f*10.0) + (*ch - '0');
			++ch;
			++n;
		}
		double pow = 1;
		for (int i = 0; i < n; i++) {
			pow *= 10.0;
		}
		result += f / pow;
	}
	return result;
}

char * dtoa(double n) {

	int sign = ((unsigned long)n) >> 63;
	int exponent = (((unsigned long)n) & 0x7FF0000000000000) >> 52;
	int fraction = ((unsigned long)n) & 0xFFFFFFFFFFFFF;
	

	if (n == 0.0) {
		return "0";
	} else {

		List<char> *cb = new List<char>();

		if (n < 0) {
			cb->Push('-');
		}

		cb->Push('\0');

		char * value = new char[cb->GetSize()];
		memcpy(value, cb->GetBuffer(), cb->GetSize());

		//delete cb;

		return value;
	}
}