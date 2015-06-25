#pragma once

#include "syntaxkind.h"

namespace r {
	struct SyntaxToken {

		SyntaxToken(SyntaxKind kind, const char *value);

		SyntaxKind Kind;
		char const *Value; //TODO: change to void *
	};
}
