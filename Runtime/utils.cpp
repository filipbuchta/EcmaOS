#include "utils.h"
#include "list.h"
#include "runtime.h"
#include "checks.h"



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

const char * itoa(int value) {
	List<char> * result = new List<char>();

	if (value < 0) {
		value *= -1;
	}

	int32_t temp = (int32_t)value;

	const char table[] = "0123456789";

	while (temp > 0) {
		result->Insert(0, table[temp % 10]);
		temp /= 10;
	}

	if (value < 0) {
		result->Insert(0, '-');
	}
	result->Push('\0');

	char * str = new char[result->GetSize()];
	memcpy(str, result->GetBuffer(), result->GetSize());
	return str;
}

const char * dtoa(double value) {

	List<char> * result = new List<char>();

	if (value < 0) {
		result->Push('-');
		value *= -1;
	}

	int32_t temp = (int32_t)value;

	int integerNumbers = 0;
	{
		while (temp > 0) {
			temp /= 10;
			integerNumbers++;
		}

		const char table[] = "0123456789";

		int position = result->GetSize();

		temp = (int32_t)(value * 1000000);

		while (temp > 0) {
			result->Insert(position, table[temp % 10]);
			temp /= 10;
		}
	}


	result->Insert(integerNumbers, '.');
	if (integerNumbers == 0) {
		result->Insert(0, '0');
	}
	{
		int position = result->GetSize() - 1;
		while (true) {
			if (result->Get(position) == '0') {
				result->Pop();
			}
			else if (result->Get(position) != '.') {
				break;
			}
			else {
				result->Pop();
				break;
			}
			position--;
		}
	}


	result->Push('\0');

	char * str = new char[result->GetSize()];
	memcpy(str, result->GetBuffer(), result->GetSize());
	return str;
	//return "123\0";
}

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
