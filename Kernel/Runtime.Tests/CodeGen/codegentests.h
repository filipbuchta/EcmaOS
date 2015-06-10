#pragma once

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../Runtime/syntax/syntaxkind.h"
#include "../../Runtime/syntax/syntaxnode.h"
#include "../../Runtime/parser.h"
#include "../../Runtime/codegen/codegenerator.h"
#include "../../Runtime/syntax/syntaxnodevisitor.h"

void CompileAndVerify(const char * code, const char *expectedOutput);