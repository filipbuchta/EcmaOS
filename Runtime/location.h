#pragma once

namespace r {
	struct Location {
		const char * Path = "";
		int Position = 0;
		int Line = 1;
		int Column = 1;
	};

}