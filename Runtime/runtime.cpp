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

	HeapObject * Runtime::Console_log(Number * number)
	{
		double value = *reinterpret_cast<double*>(reinterpret_cast<char*>(number) + Number::ValueOffset);
		List<char> * result = new List<char>();


		if (value < 0) {
			result->Push('-');
			value *= -1;
		}


		int64 temp = (int64)value;

		int integerNumbers = 0;
		{
			while (temp > 0) {
				temp /= 10;
				integerNumbers++;
			}

			const char table[] = "0123456789";

			int position = result->GetSize();

			temp = (int64)(value * 100000000);

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

		//delete result;

		Platform::Print(str);

		return 0;
	}

}