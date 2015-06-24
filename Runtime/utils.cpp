#include "utils.h"
#include "list.h"
#include "runtime.h"
#include "checks.h"



unsigned int StringLength(const char * string) {
	int length = 0;
	while (string != 0) {
		string++;
		length++;
	}
	return length;
}

