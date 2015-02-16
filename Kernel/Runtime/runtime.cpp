#include "runtime.h"
#include "list.h"

typedef long long int64;

namespace r {

	void PrintNumber(int64 number, List<char> * buffer) {
		const char table[] = "0123456789";

		int position = buffer->GetSize();

		int64 temp = number;

		while (temp > 0) {
			buffer->Insert(position, table[temp % 10]);
			temp /= 10;
		}
	}

	Object * Runtime::DebugPrint(Number * number)
	{
		int64 value = *(int64*)(number + Number::ValueOffset);

		//int64 mantissa = value & 0xFFFFFFFFFFFFF;

		int64 fraction = value & 0xFFFFFFFFFFFFF;
		fraction |= 0x10000000000000;

		const int exponentBias = 1023;

		int64 exponent = ((value & (0x7FF0000000000000)) >> 52) - exponentBias;

		bool sign = (value & 0x8000000000000000) != 0;
			
		List<char> * result = new List<char>();

		if (sign) {
			result->Push('-');
		}

		int64 temp;
		if (52 - exponent > 0) {
			temp = fraction >> int64(52 - exponent);
		}
		else {
			temp = fraction << int64(exponent - 52);
		}
		PrintNumber(temp, result);


		result->Push('\0');

		char * str = new char[result->GetSize()];
		memcpy(str, result->GetBuffer(), result->GetSize());

		//delete result;

		Platform::Print(str);

		return 0;
	}

}