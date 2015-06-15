#pragma once

#include "../../Kernel/Runtime/list.h"
#include "writer.h"
#include "../../Kernel/Runtime/codegen/codegenerator.h"

using namespace r;

struct PESectionHeader {
	const char Name[8];
	DWORD VirtualSize;
	DWORD VirtualAddress;
	DWORD SizeOfRawData;
	DWORD PointerToRawData;
	DWORD PointerToRelocations;
	DWORD PointerToLineNumbers;
	WORD NumberOfRelocations;
	WORD NumberOfLineNumbers;
	DWORD Characteristics;
};

class PESection {
public:
	PESection(const char * name, DWORD characteristics) : _name(name), _characteristics(characteristics) { }
	const char * GetName() { return _name; }
	void WriteBody(Writer::Slot<PESectionHeader> header, Writer & w);

protected:
	virtual void WriteBody(Writer & w) = 0;

private:
	const char * _name;
	DWORD _characteristics;
};


class TextSection : public PESection {
public:
	TextSection(AssemblySymbol * code) : _code(code), PESection(".text", 0x60300020) { }

	void TextSection::WriteBody(Writer & w) override;
private:
	AssemblySymbol * _code;
};

class PE {

public:
	void Write(Writer & write);

	List<PESection *> * GetSections() { return _sections; }

private:
	void WriteHeader(Writer & writer);
	void WriteBody(Writer::Slot<PESectionHeader> header, Writer & w);
	void WriteSections(Writer & w);
	void WriteSectionHeaders(Writer & w);

	List<PESection *> * _sections = new List<PESection *>();
};