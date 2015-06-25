#include "pe.h"



void TextSection::WriteBody(Writer & w) {
	w.Write(_code->GetEntryPoint()->GetCode(), _code->GetEntryPoint()->GetCodeSize());
}

void PESection::WriteBody(Writer::Slot<PESectionHeader> header, Writer & w) {
	int start = w.GetPosition();
	WriteBody(w);

	header->VirtualSize = w.GetPosition() - start;
	header->VirtualAddress = ((start - 0x200) / 0x200) * 0x1000;
	header->SizeOfRawData = 0x200;
	header->PointerToRawData = start;
	header->Characteristics = _characteristics;
}



void PE::Write(Writer & w) {
	WriteHeader(w);
	WriteSectionHeaders(w);
	WriteSections(w);



	//w.Write<DWORD>(0); // zeros
	//w.Write<DWORD>(4); // offset
	//w.Write<DWORD>(0); // value
	//w.Write<WORD>(6); // scnum
	//w.Write<WORD>(0); // type
	//w.Write<WORD>(0x103); // scclass & numaux

	//w.Write<DWORD>(0);
	//w.Write<DWORD>(0);
	//w.Write<DWORD>(0);
	//w.Write<DWORD>(0);
	//w.Write<WORD>(0);
	//


	//w.Write<DWORD>(0);
	//w.Write<DWORD>(18);
	//w.Write<DWORD>(0);
	//w.Write<WORD>(8);
	//w.Write<WORD>(0);
	//w.Write<WORD>(0x0103);

	//w.Write<DWORD>(0);
	//w.Write<DWORD>(0);
	//w.Write<DWORD>(0);
	//w.Write<DWORD>(0);
	//w.Write<WORD>(0);

	//w.Write<DWORD>(0);
	//w.Write<DWORD>(30);
	//w.Write<DWORD>(0);
	//w.Write<WORD>(7);
	//w.Write<WORD>(0);
	//w.Write<WORD>(0x0103);
	//
	//w.Write<DWORD>(0);
	//w.Write<DWORD>(0);
	//w.Write<DWORD>(0);
	//w.Write<DWORD>(0);
	//w.Write<WORD>(0);
	//

	//w.Write<DWORD>(0);
	//w.Write<DWORD>(42);
	//w.Write<DWORD>(0);
	//w.Write<WORD>(6);
	//w.Write<WORD>(0);
	//w.Write<WORD>(0x0103);

	//w.Write<DWORD>(0);
	//w.Write<DWORD>(0);
	//w.Write<DWORD>(0);
	//w.Write<DWORD>(0);
	//w.Write<WORD>(0);

	//w.Write<DWORD>(55);
	//w.WriteString(".debug_abbrev");
	//w.WriteString(".debug_line");
	//w.WriteString(".debug_info");
	//w.WriteString(".debug_frame");


}

void PE::WriteSections(Writer & w) {
	Writer::Slot<PESectionHeader> headerSlot = w.SlotAt<PESectionHeader>(0x178); //sizeof(PEHeader)

	for (int i = 0; i < GetSections()->GetSize(); i++) {
		GetSections()->Get(i)->WriteBody(headerSlot.At(i), w);
		w.Align(0x200);
	}
}

void PE::WriteSectionHeaders(Writer & w) {

	for (PESection * section : *_sections) {
		Writer::Slot<PESectionHeader> header = w.CreateSlot<PESectionHeader>();
		memcpy((void*)header->Name, (void*)section->GetName(), 8);
		header->VirtualSize = 0;
		header->VirtualAddress = 0;
		header->SizeOfRawData = 0;
		header->PointerToRawData = 0;
		header->PointerToRelocations = 0;
		header->PointerToLineNumbers = 0;
		header->NumberOfRelocations = 0;
		header->NumberOfLineNumbers = 0;
		header->Characteristics = 0;
	}

	w.Align(0x200);
}

void PE::WriteHeader(Writer & w) {

	const char dosHeader[] =
	{
		0x4d, 0x5a, 0x90, 0x00, 0x03, 0x00, 0x00, 0x00,
		0x04, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
		0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
		0x0e, 0x1f, 0xba, 0x0e, 0x00, 0xb4, 0x09, 0xcd,
		0x21, 0xb8, 0x01, 0x4c, 0xcd, 0x21, 0x54, 0x68,
		0x69, 0x73, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72,
		0x61, 0x6d, 0x20, 0x63, 0x61, 0x6e, 0x6e, 0x6f,
		0x74, 0x20, 0x62, 0x65, 0x20, 0x72, 0x75, 0x6e,
		0x20, 0x69, 0x6e, 0x20, 0x44, 0x4f, 0x53, 0x20,
		0x6d, 0x6f, 0x64, 0x65, 0x2e, 0x0d, 0x0d, 0x0a,
		0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};


	w.Write((unsigned char *)dosHeader, sizeof(dosHeader));

	// PE Signature
	w.Write<DWORD>(0x00004550); // PE\0\0

	// COFF Header

	w.Write((WORD)0x014C); // machine
	w.Write((WORD)GetSections()->GetSize()); // number of sections
	w.Write((DWORD)0x00); // timestmap
	w.Write((DWORD)0xE00); // pointer to symbol table
	w.Write((DWORD)8); // number of symbols
	w.Write((WORD)0xE0); // SizeOfOptionalHeader 

	w.Write((WORD)0x0107); // characteristics 0x2000 for dll

	w.Write((WORD)0x10B); // PE32

	w.Write((BYTE)0x02); // Linker major version
	w.Write((BYTE)0x18); // Linker minor version
	w.Write((DWORD)0x200); // size of code
	w.Write((DWORD)0x00); // size of initialized data
	w.Write((DWORD)0); // size of uninitialized data
	w.Write((DWORD)0x001000); // address of entry point
	w.Write((DWORD)0x001000); // base of code

	w.Write((DWORD)0x00000000); // base of data
	w.Write((DWORD)0x00400000); // base address

	w.Write((DWORD)0x1000); // section alignment
	w.Write((DWORD)0x200); // file alignment


	w.Write((WORD)4); // OS Major version
	w.Write((WORD)0);
	w.Write((WORD)0); // Image Major version
	w.Write((WORD)0);
	w.Write((WORD)4); // Subsystem Major version
	w.Write((WORD)0);

	w.Write((DWORD)0); // Win32VersionValue
	//THIS IS IMPORANT AND CAN CAUSE EXE TO NOT WORK
	w.Write((DWORD)0x6000); // Size of image 
	w.Write((DWORD)0x400); // Size of headers
	w.Write((DWORD)0); // checksum

	w.Write((WORD)0x3); // Console application

	w.Write((WORD)0x0000);

	w.Write((DWORD)0x200000); // size of stack reserve
	w.Write((DWORD)0x1000); // size of stack commit
	w.Write((DWORD)0x100000); // size of heap reserve
	w.Write((DWORD)0x1000); // size of heap commit

	w.Write((DWORD)0); // loader flags
	w.Write((DWORD)0x10); // number of data directories

	w.Write((DWORD)0); // export table
	w.Write((DWORD)0);

	w.Write((DWORD)0); // import table
	w.Write((DWORD)0);

	w.Write((DWORD)0); // resource table
	w.Write((DWORD)0);

	w.Write((DWORD)0); // exception table
	w.Write((DWORD)0);

	w.Write((DWORD)0); // certifiate table
	w.Write((DWORD)0);

	w.Write((DWORD)0); // base relocation table
	w.Write((DWORD)0);

	w.Write((DWORD)0); // debug data
	w.Write((DWORD)0);

	w.Write((DWORD)0); // copyright table
	w.Write((DWORD)0);

	w.Write((DWORD)0); // machine value
	w.Write((DWORD)0);

	w.Write((DWORD)0); // thread local storage table
	w.Write((DWORD)0);

	w.Write((DWORD)0); // load configuration table
	w.Write((DWORD)0);

	w.Write((DWORD)0); // bound import table
	w.Write((DWORD)0);

	w.Write((DWORD)0); // import address table 
	w.Write((DWORD)0);

	w.Write((DWORD)0); // delay import table
	w.Write((DWORD)0);

	w.Write((DWORD)0); // cli header table
	w.Write((DWORD)0);

	w.Write((DWORD)0);
	w.Write((DWORD)0);


}