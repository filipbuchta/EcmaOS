#pragma once


namespace r {
	class SourceFile {
	public:
		void SetFileName(const char * value) { _fileName = value; }
		const char * GetFileName() { return _fileName; }
		void SetCode(const char * value) { _code = value; }
		const char * GetCode() { return _code; }
	private:
		const char * _fileName;
		const char * _code;
	};

}