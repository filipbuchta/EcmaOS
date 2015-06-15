#include "scope.h"


namespace r {

	Symbol * Scope::LookupSymbol(const char * name) {
		for (Symbol* child : *_locals) {
			if (strcmp(child->GetName(), name) == 0) {
				return child;
			}
		}
		if (_outerScope != nullptr) {
			return _outerScope->LookupSymbol(name);
		}
		return nullptr;
	}


}