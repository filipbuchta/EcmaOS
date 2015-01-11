#pragma once

#include "platform.h"

#define FATAL(format, ...) \
	r::Platform::Fatal(__FILE__, __LINE__, format, __VA_ARGS__);

#define NOT_IMPLEMENTED() \
	FATAL("%s", "Not implemented");

//#define ASSERT(condition)
