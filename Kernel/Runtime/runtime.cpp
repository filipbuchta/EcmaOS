#include "runtime.h"

namespace r {

	void Runtime::DebugPrint(int value)
	{
		int temp = value;

		int digits = 0;
		while (temp > 0) {
			digits++;
			temp /= 10;
		}

		char* result = new char[digits + 1];

		static char table[] = "0123456789";

		result[digits] = '\0';
		digits--;

		temp = value;
		while (temp > 0) {
			result[digits] = table[temp % 10];
			digits--;
			temp /= 10;
		}

		Platform::Print(result);
	}

}