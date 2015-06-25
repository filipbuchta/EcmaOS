#include "diagnostic.h"


namespace r {

	void Diagnostics::AddError(const char * message, Location location) {
		DiagnosticInfo * diagnostic = new DiagnosticInfo();
		diagnostic->SetSeverity(DiagnosticSeverity::Error);
		diagnostic->SetMessage(message);
		diagnostic->SetLocation(location);
		_infos->Push(diagnostic);
	}

}