#include "dwarf.h"



void DebugLineSection::WriteExtendedOpcode(Writer & w, DWARF2ExtendedOpcode op, size_t operands_size) {
	w.Write<uint8_t>(0);
	w.WriteULEB128(operands_size + 1);
	w.Write<uint8_t>(op);
}

void DebugLineSection::WriteBody(Writer & w) {
	Writer::Slot<DWORD> totalLength = w.CreateSlot<DWORD>();
	int start = w.GetPosition();


	// Used for special opcodes                                                                    
	const int8_t line_base = 1;
	const uint8_t line_range = 7;
	const int8_t max_line_incr = (line_base + line_range - 1);
	const uint8_t opcode_base = DW_LNS_NEGATE_STMT + 1;

	w.Write<uint16_t>(2);  // Field version.                                                      
	Writer::Slot<uint32_t> prologueLength = w.CreateSlot<uint32_t>();
	uintptr_t prologue_start = w.GetPosition();
	w.Write<uint8_t>(1);  // Field minimum_instruction_length.                                    
	w.Write<uint8_t>(1);  // Field default_is_stmt.                                               
	w.Write<int8_t>(line_base);  // Field line_base.                                              
	w.Write<uint8_t>(line_range);  // Field line_range.                                           
	w.Write<uint8_t>(opcode_base);  // Field opcode_base.                                         
	w.Write<uint8_t>(0);  // DW_LNS_COPY operands count.                                          
	w.Write<uint8_t>(1);  // DW_LNS_ADVANCE_PC operands count.                                    
	w.Write<uint8_t>(1);  // DW_LNS_ADVANCE_LINE operands count.                                  
	w.Write<uint8_t>(1);  // DW_LNS_SET_FILE operands count.                                      
	w.Write<uint8_t>(1);  // DW_LNS_SET_COLUMN operands count.                                    
	w.Write<uint8_t>(0);  // DW_LNS_NEGATE_STMT operands count.                                   
	w.Write<uint8_t>(0);  // Empty include_directories sequence.                                  
	w.WriteString(_description->GetFilename());  // File name.                                          
	w.WriteULEB128(0);  // Current directory.                                                     
	w.WriteULEB128(0);  // Unknown modification time.                                             
	w.WriteULEB128(0);  // Unknown file size.                                                     
	w.Write<uint8_t>(0);
	prologueLength.Set(static_cast<uint32_t>(w.GetPosition() - prologue_start));

	WriteExtendedOpcode(w, DW_LNE_SET_ADDRESS, sizeof(intptr_t));
	//w.Write<intptr_t>(_description->GetCodeStart());
	w.Write<intptr_t>(0x401000);

	w.Write<uint8_t>(DW_LNS_COPY);

	intptr_t pc = 0;
	intptr_t line = 1;


	//bool is_statement = true;

	List<PCInfo> * pcInfo = _description->GetEntryPoint()->GetLineInfo()->GetPCInfo();

	//pc_info->Sort(&ComparePCInfo);

	int pcInfoSize = pcInfo->GetSize();
	for (int i = 0; i < pcInfoSize; i++) {
		PCInfo* info = &pcInfo->Get(i);

		// Reduce bloating in the debug line table by removing duplicate line                        
		// entries (per DWARF2 standard).                                                            
		intptr_t  new_line = info->Location.Line;
		/*if (new_line == line) {
			continue;
		}*/

	//	// Mark statement boundaries.  For a better debugging experience, mark                       
	//	// the last pc address in the function as a statement (e.g. "}"), so that                    
	//	// a user can see the result of the last line executed in the function,                      
	//	// should control reach the end.                                                             
	//	if ((i + 1) == pc_info_length) {
	//		if (!is_statement) {
	//			w.Write<uint8_t>(DW_LNS_NEGATE_STMT);
	//		}
	//	}
	//	else if (is_statement != info->is_statement_) {
	//		w.Write<uint8_t>(DW_LNS_NEGATE_STMT);
	//		is_statement = !is_statement;
	//	}

		// Generate special opcodes, if possible.  This results in more compact                      
		// debug line tables.  See the DWARF 2.0 standard to learn more about                        
		// special opcodes.                                                                          
		uintptr_t pc_diff = info->PC - pc;
		intptr_t line_diff = new_line - line;

		// Compute special opcode (see DWARF 2.0 standard)                                           
		intptr_t special_opcode = (line_diff - line_base) + (line_range * pc_diff) + opcode_base;

		// If special_opcode is less than or equal to 255, it can be used as a                       
		// special opcode.  If line_diff is larger than the max line increment                       
		// allowed for a special opcode, or if line_diff is less than the minimum                    
		// line that can be added to the line register (i.e. line_base), then                        
		// special_opcode can't be used.                                                             
		if ((special_opcode >= opcode_base) && (special_opcode <= 255) &&
			(line_diff <= max_line_incr) && (line_diff >= line_base)) {
			w.Write<uint8_t>(special_opcode);
		}
		else {
			w.Write<uint8_t>(DW_LNS_ADVANCE_PC);
			w.WriteSLEB128(pc_diff);
			w.Write<uint8_t>(DW_LNS_ADVANCE_LINE);
			w.WriteSLEB128(line_diff);
			w.Write<uint8_t>(DW_LNS_COPY);
		}

		// Increment the pc and line operands.                                                       
		pc += pc_diff;
		line += line_diff;
	}
	// Advance the pc to the end of the routine, since the end sequence opcode                     
	// requires this.                                                                              
	w.Write<uint8_t>(DW_LNS_ADVANCE_PC);
	//w.WriteSLEB128(_description->GetCodeSize() - pc);
	w.WriteSLEB128(11);
	WriteExtendedOpcode(w, DW_LNE_END_SEQUENCE, 0);


	totalLength.Set((DWORD)(w.GetPosition() - start));
}


void DebugAbbrevSection::WriteBody(Writer & w) {

	
	w.WriteULEB128(1);
	w.WriteULEB128(DW_TAG_COMPILE_UNIT);
	w.Write<uint8_t>(DW_CHILDREN_YES);

	w.WriteULEB128(DW_AT_NAME);
	w.WriteULEB128(DW_FORM_STRING);

	w.WriteULEB128(DW_AT_LOW_PC);
	w.WriteULEB128(DW_FORM_ADDR);

	w.WriteULEB128(DW_AT_HIGH_PC);
	w.WriteULEB128(DW_FORM_ADDR);

	w.WriteULEB128(DW_AT_STMT_LIST);
	w.WriteULEB128(DW_FORM_DATA4);

	w.WriteULEB128(0);
	w.WriteULEB128(0);




	w.WriteULEB128(2);
	w.WriteULEB128(DW_TAG_SUBPROGRAM);
	w.Write<uint8_t>(DW_CHILDREN_YES);

	w.WriteULEB128(DW_AT_NAME);
	w.WriteULEB128(DW_FORM_STRING);

	w.WriteULEB128(DW_AT_LOW_PC);
	w.WriteULEB128(DW_FORM_ADDR);

	w.WriteULEB128(DW_AT_HIGH_PC);
	w.WriteULEB128(DW_FORM_ADDR);

	w.WriteULEB128(DW_AT_FRAME_BASE);
	w.WriteULEB128(DW_FORM_DATA4);

	w.WriteULEB128(DW_AT_SIBLING);
	w.WriteULEB128(DW_FORM_REF4);


	w.WriteULEB128(0);
	w.WriteULEB128(0);



	w.WriteULEB128(3);
	w.WriteULEB128(DW_TAG_LEXICAL_BLOCK);
	w.Write<uint8_t>(DW_CHILDREN_YES);

	w.WriteULEB128(DW_AT_LOW_PC);
	w.WriteULEB128(DW_FORM_ADDR);

	w.WriteULEB128(DW_AT_HIGH_PC);
	w.WriteULEB128(DW_FORM_ADDR);

	w.WriteULEB128(0);
	w.WriteULEB128(0);



	w.WriteULEB128(4);
	w.WriteULEB128(DW_TAG_VARIABLE);
	w.Write<uint8_t>(DW_CHILDREN_NO);

	w.WriteULEB128(DW_AT_NAME);
	w.WriteULEB128(DW_FORM_STRING);

	w.WriteULEB128(DW_AT_TYPE);
	w.WriteULEB128(DW_FORM_REF4);

	w.WriteULEB128(DW_AT_LOCATION);
	w.WriteULEB128(DW_FORM_BLOCK1);

	w.WriteULEB128(0);
	w.WriteULEB128(0);




	w.WriteULEB128(5);
	w.WriteULEB128(DW_TAG_BASE_TYPE);
	w.Write<uint8_t>(DW_CHILDREN_NO);

	w.WriteULEB128(DW_AT_BYTE_SIZE);
	w.WriteULEB128(DW_FORM_DATA1);

	w.WriteULEB128(DW_AT_ENCODING);
	w.WriteULEB128(DW_FORM_DATA1);

	w.WriteULEB128(DW_AT_NAME);
	w.WriteULEB128(DW_FORM_STRING);


	w.WriteULEB128(0);
	w.WriteULEB128(0);




	w.WriteULEB128(0);

}

void DebugInfoSection::WriteBody(Writer & w) {

	Writer::Slot<DWORD> size = w.CreateSlot<DWORD>();

	int start = w.GetPosition();

	w.Write<WORD>(2); // Dwarf version
	w.Write((DWORD)0); // offset into the .debug_abbrev section
	w.Write<BYTE>(sizeof(intptr_t)); // Size of pointer

	w.WriteULEB128(1); // compilation unit
	w.WriteString(_description->GetFilename());

	/*w.Write((intptr_t)_description->GetCodeStart());
	w.Write((intptr_t)_description->GetCodeStart() + _description->GetCodeSize());*/
	w.Write((intptr_t)0x401000);
	w.Write((intptr_t)_description->GetCodeSize());

	w.Write((DWORD)0); // statement list



	// subprogram
	w.WriteULEB128(2); 
	w.WriteString("entry");

	w.Write((intptr_t)0x401000);
	w.Write((intptr_t)0x401000 + _description->GetCodeSize());
	w.Write((DWORD)0x0);
	w.Write((DWORD)0x4c); // sibling

	{
		w.WriteULEB128(3); // lexical block
		w.Write((intptr_t)0x401000);
		w.Write((intptr_t)0x401000 + _description->GetCodeSize());

		{
			w.WriteULEB128(4); // variable
			w.WriteString("x");

			w.Write((DWORD)0x4c); // type
			w.WriteULEB128(2);
			w.Write<BYTE>(0x91);
			w.Write<BYTE>(0x74);
		}
		w.Write((BYTE)0);
	}
	w.Write((BYTE)0);

	w.WriteULEB128(5); // type

	w.Write<BYTE>(0x04);
	w.Write<BYTE>(0x05);

	w.WriteString("int");

	w.Write((BYTE)0);
	


	size.Set((DWORD)(w.GetPosition() - start));
}



void DebugFrameSection::WriteBody(Writer & w) {
	{
		Writer::Slot<DWORD> size = w.CreateSlot<DWORD>();
		int start = w.GetPosition();

		w.Write((DWORD)0xffffffff);
		w.Write((BYTE)1);
		w.WriteString("");
		w.WriteULEB128(1); // code align factor
		w.WriteSLEB128(-4); // data align factor
		w.Write<BYTE>(8); // return address column

		w.Write<BYTE>(DW_CFA_DEF_CFA);
		w.WriteULEB128(4);
		w.WriteULEB128(4);
		w.Write<BYTE>(DW_CFA_OFFSET | 8);
		w.WriteULEB128(1);
		w.Write<BYTE>(DW_CFA_NOP);
		w.Write<BYTE>(DW_CFA_NOP);

		size.Set((WORD)(w.GetPosition() - start));
	}
	{
		Writer::Slot<DWORD> size = w.CreateSlot<DWORD>();
		int start = w.GetPosition();
		w.Write<DWORD>(0x00);
		w.Write<DWORD>(0x401000);
		w.Write<DWORD>(0x1C);

		w.Write<BYTE>(DW_CFA_ADVANCE_LOC | 1);
		w.Write<BYTE>(DW_CFA_DEF_CFA_OFFSET);
		w.WriteULEB128(8);
		w.Write<BYTE>(DW_CFA_OFFSET | 5);
		w.WriteULEB128(2);
		w.Write<BYTE>(DW_CFA_ADVANCE_LOC | 2);
		w.Write<BYTE>(DW_CFA_DEF_CFA_REGISTER);
		w.WriteULEB128(5);
		w.Write<BYTE>(DW_CFA_ADVANCE_LOC | 24);
		w.Write<BYTE>(DW_CFA_RESTORE | 5);
		w.Write<BYTE>(DW_CFA_DEF_CFA);
		w.WriteULEB128(4);
		w.WriteULEB128(4);
		w.Write<BYTE>(DW_CFA_NOP);
		w.Write<BYTE>(DW_CFA_NOP);
		w.Write<BYTE>(DW_CFA_NOP);

		size.Set((WORD)(w.GetPosition() - start));
	}
}
	