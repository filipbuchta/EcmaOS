#pragma once

#include "location.h"
#include "list.h"

namespace r {
	enum class DiagnosticSeverity {
		Info,
		Warrning,
		Error,
	};

	class DiagnosticInfo {
	public:
		void SetSeverity(DiagnosticSeverity value) { _severity = value; }
		void SetMessage(const char * value) { _message = value; }
		void SetLocation(Location value) { _location = value; }

		DiagnosticSeverity GetSeverity() { return _severity; }
		const char * GetMessage() { return _message; }
		Location GetLocation() { return _location; }
	private:
		DiagnosticSeverity _severity;
		const char * _message;
		Location _location;
	};

	class Diagnostics {
	public:
		void AddError(const char * message, Location location);
		List<DiagnosticInfo *> * GetInfos() { return _infos; }
	private:
		List<DiagnosticInfo*> * _infos = new List<DiagnosticInfo*>();
	};
}