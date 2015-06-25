#include "utils.h"
#include "list.h"
#include "runtime.h"
#include "checks.h"



unsigned int StringLength(const char * string) {
	const char *s;

	for (s = string; *s; ++s)
	{
	}
	return (s - string);
}

