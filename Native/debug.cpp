#include "debug.h"


const char * Int32ToString(int value) {

	const int sizes[] = { 9, 99, 999, 9999, 99999, 999999, 9999999, 99999999, 999999999 };
	const char table[] = "0123456789";

	if (value < 0) {
		value *= -1;
	}

	int size = 0;
	for (int i = 0; i < 10; i++) {
		if (value < sizes[i]) {
			size = i + 1;
			break;
		}
	}

	char * result = new char[size + 1];

	if (value == 0) {
		result[0] = '0';
	}
	else {

		int i = size - 1;
		while (value > 0) {
			result[i] = table[value % 10];
			value /= 10;
			i--;
		}
	}

	result[size] = '\0';

	return result;
}


void Log(const char * message) {
	ConsoleWrite(message);
	ConsoleWrite("\n");
};

void Log(uint32 message) {
	Log(Int32ToString(message));
};





void BochsLog(const char * text) {
	for (int i = 0; (text)[i] != '\0'; i++) {

		char c = text[i];
		_asm {
			mov al, c
				out 0xe9, al
		}

	}
}

