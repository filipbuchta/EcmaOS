#include "runtime.h"
#include "list.h"

namespace r {

	void PrintNumber(__int64 number, List<char> * buffer) {
		const char table[] = "0123456789";

		int position = buffer->GetSize();

		__int64 temp = number;

		while (temp > 0) {
			buffer->Insert(position, table[temp % 10]);
			temp /= 10;
		}
	}

	void Runtime::DebugPrint(Number * number)
	{
		__int64 value = *(__int64*)(number + Number::ValueOffset);

		//__int64 mantissa = value & 0xFFFFFFFFFFFFF;

		__int64 fraction = value & 0xFFFFFFFFFFFFF;
		fraction |= 0x10000000000000;

		const int exponentBias = 1023;

		__int64 exponent = ((value & (0x7FF0000000000000)) >> 52) - exponentBias;

		bool sign = (value & 0x8000000000000000) != 0;
			
		List<char> * result = new List<char>();

		if (sign) {
			result->Push('-');
		}

		__int64 temp;
		if (52 - exponent > 0) {
			temp = fraction >> __int64(52 - exponent);
		}
		else {
			temp = fraction << __int64(exponent - 52);
		}
		PrintNumber(temp, result);


		result->Push('\0');

		char * str = new char[result->GetSize()];
		memcpy(str, result->GetBuffer(), result->GetSize());

		//delete result;

		Platform::Print(str);
	}

}