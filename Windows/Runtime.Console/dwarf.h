#pragma once

#include "pe.h"
#include "../../Kernel/Runtime/codegen/codegenerator.h"
#include "../../Kernel/Runtime/global.h"

using namespace r;



// DWARF2 standard, figure 34.                       
enum DWARF2Opcodes {
	DW_LNS_COPY = 1,
	DW_LNS_ADVANCE_PC = 2,
	DW_LNS_ADVANCE_LINE = 3,
	DW_LNS_SET_FILE = 4,
	DW_LNS_SET_COLUMN = 5,
	DW_LNS_NEGATE_STMT = 6
};

// DWARF2 standard, figure 35.                       
enum DWARF2ExtendedOpcode {
	DW_LNE_END_SEQUENCE = 1,
	DW_LNE_SET_ADDRESS = 2,
	DW_LNE_DEFINE_FILE = 3
};



enum dwarf_call_frame_info
{
	DW_CFA_ADVANCE_LOC = 0x40,
	DW_CFA_OFFSET = 0x80,
	DW_CFA_RESTORE = 0xC0,
	DW_CFA_NOP = 0x00,
	DW_CFA_SET_LOC = 0x01,
	DW_CFA_ADVANCE_LOC1 = 0x02,
	DW_CFA_ADVANCE_LOC2 = 0x03,
	DW_CFA_ADVANCE_LOC4 = 0x04,
	DW_CFA_OFFSET_EXTENDED = 0x05,
	DW_CFA_RESTORE_EXTENDED = 0x06,
	DW_CFA_UNDEFINED = 0x07,
	DW_CFA_SAME_VALUE = 0x08,
	DW_CFA_REGISTER = 0x09,
	DW_CFA_REMEMBER_STATE = 0x0A,
	DW_CFA_RESTORE_STATE = 0x0B,
	DW_CFA_DEF_CFA = 0x0C,
	DW_CFA_DEF_CFA_REGISTER = 0x0D,
	DW_CFA_DEF_CFA_OFFSET = 0x0E,
	DW_CFA_DEF_CFA_ExPRESSION = 0x0F,
	DW_CFA_EXPRESSION = 0x10,
	/* DWARF 2.1 */
	DW_CFA_OFFSET_ExTENDED_SF = 0x11,
	DW_CFA_DEF_CFA_SF = 0x12,
	DW_CFA_DEF_CFA_OFFSET_SF = 0x13,

	/* SGI/MIPS SPECIFIC */
	DW_CFA_MIPS_ADVANCE_LOC8 = 0x1D,

	/* GNU ExTENSIONS */
	DW_CFA_GNU_WINDOW_SAVE = 0x2D,
	DW_CFA_GNU_ARGS_SIZE = 0x2E,
	DW_CFA_GNU_NEGATIVE_OFFSET_EXTENDED = 0x2F
};


// DWARF2 standard, figure 14.                     
enum DWARF2Tags {
	DW_TAG_ARRAY_TYPE = 0x01,
	DW_TAG_CLASS_TYPE = 0x02,
	DW_TAG_ENTRY_POINT = 0x03,
	DW_TAG_ENUMERATION_TYPE = 0x04,
	DW_TAG_FORMAL_PARAMETER = 0x05,
	DW_TAG_IMPORTED_DECLARATION = 0x08,
	DW_TAG_LABEL = 0x0A,
	DW_TAG_LEXICAL_BLOCK = 0x0B,
	DW_TAG_MEMBER = 0x0D,
	DW_TAG_POINTER_TYPE = 0x0F,
	DW_TAG_REFERENCE_TYPE = 0x10,
	DW_TAG_COMPILE_UNIT = 0x11,
	DW_TAG_STRING_TYPE = 0x12,
	DW_TAG_STRUCTURE_TYPE = 0x13,
	DW_TAG_SUBROUTINE_TYPE = 0x15,
	DW_TAG_TYPEDEF = 0x16,
	DW_TAG_UNION_TYPE = 0x17,
	DW_TAG_UNSPECIFIED_PARAMETERS = 0x18,
	DW_TAG_VARIANT = 0x19,
	DW_TAG_COMMON_BLOCK = 0x1A,
	DW_TAG_COMMON_INCLUSION = 0x1B,
	DW_TAG_INHERITANCE = 0x1C,
	DW_TAG_INLINED_SUBROUTINE = 0x1D,
	DW_TAG_MODULE = 0x1E,
	DW_TAG_PTR_TO_MEMBER_TYPE = 0x1F,
	DW_TAG_SET_TYPE = 0x20,
	DW_TAG_SUBRANGE_TYPE = 0x21,
	DW_TAG_WITH_STMT = 0x22,
	DW_TAG_ACCESS_DECLARATION = 0x23,
	DW_TAG_BASE_TYPE = 0x24,
	DW_TAG_CATCH_BLOCK = 0x25,
	DW_TAG_CONST_TYPE = 0x26,
	DW_TAG_CONSTANT = 0x27,
	DW_TAG_ENUMERATOR = 0x28,
	DW_TAG_FILE_TYPE = 0x29,
	DW_TAG_FRIEND = 0x2A,
	DW_TAG_NAMELIST = 0x2B,
	DW_TAG_NAMELIST_ITEM = 0x2C,
	DW_TAG_PACKED_TYPE = 0x2D,
	DW_TAG_SUBPROGRAM = 0x2E,
	DW_TAG_TEMPLATE_TYPE_PARAMETER = 0x2F,
	DW_TAG_TEMPLATE_VALUE_PARAMETER = 0x30,
	DW_TAG_THROWN_TYPE = 0x31,
	DW_TAG_TRY_BLOCK = 0x32,
	DW_TAG_VARIANT_PART = 0x33,
	DW_TAG_VARIABLE = 0x34,
	DW_TAG_VOLATILE_TYPE = 0x35,
	DW_TAG_DWARF_PROCEDURE = 0x36,
	DW_TAG_RESTRICT_TYPE = 0x37,
	DW_TAG_INTERFACE_TYPE = 0x38,
	DW_TAG_NAMESPACE = 0x39,
	DW_TAG_IMPORTED_MODULE = 0x3A,
	DW_TAG_UNSPECIFIED_TYPE = 0x3B,
	DW_TAG_PARTIAL_UNIT = 0x3C,
	DW_TAG_IMPORTED_UNIT = 0x3D,
	DW_TAG_CONDITION = 0x3F,
	DW_TAG_SHARED_TYPE = 0x40,
	DW_TAG_LO_USER = 0x4080,
	DW_TAG_HI_USER = 0xFFFF,
};

// DWARF2 standard, figure 16.                     
enum DWARF2ChildrenDetermination {
	DW_CHILDREN_NO = 0,
	DW_CHILDREN_YES = 1
};

// DWARF standard, figure 17.                      
enum DWARF2Attribute {
	DW_AT_SIBLING = 0x01,
	DW_AT_LOCATION = 0x02,
	DW_AT_NAME = 0x03,
	DW_AT_ORDERING = 0x09,
	DW_AT_BYTE_SIZE = 0x0B,
	DW_AT_BIT_OFFSET = 0x0C,
	DW_AT_BIT_SIZE = 0x0D,
	DW_AT_STMT_LIST = 0x10,
	DW_AT_LOW_PC = 0x11,
	DW_AT_HIGH_PC = 0x12,
	DW_AT_LANGUAGE = 0x13,
	DW_AT_DISCR = 0x15,
	DW_AT_DISCR_VALUE = 0x16,
	DW_AT_VISIBILITY = 0x17,
	DW_AT_IMPORT = 0x18,
	DW_AT_STRING_LENGTH = 0x19,
	DW_AT_COMMON_REFERENCE = 0x1A,
	DW_AT_COMP_DIR = 0x1B,
	DW_AT_CONST_VALUE = 0x1C,
	DW_AT_CONTAINING_TYPE = 0x1D,
	DW_AT_DEFAULT_VALUE = 0x1E,
	DW_AT_INLINE = 0x20,
	DW_AT_IS_OPTIONAL = 0x21,
	DW_AT_LOWER_BOUND = 0x22,
	DW_AT_PRODUCER = 0x25,
	DW_AT_PROTOTYPED = 0x27,
	DW_AT_RETURN_ADDR = 0x2A,
	DW_AT_START_SCOPE = 0x2C,
	DW_AT_BIT_STRIDE = 0x2E,
	DW_AT_UPPER_BOUND = 0x2F,
	DW_AT_ABSTRACT_ORIGIN = 0x31,
	DW_AT_ACCESSIBILITY = 0x32,
	DW_AT_ADDRESS_CLASS = 0x33,
	DW_AT_ARTIFICIAL = 0x34,
	DW_AT_BASE_TYPES = 0x35,
	DW_AT_CALLING_CONVENTION = 0x36,
	DW_AT_COUNT = 0x37,
	DW_AT_DATA_MEMBER_LOCATION = 0x38,
	DW_AT_DECL_COLUMN = 0x39,
	DW_AT_DECL_FILE = 0x3A,
	DW_AT_DECL_LINE = 0x3B,
	DW_AT_DECLARATION = 0x3C,
	DW_AT_DISCR_LIST = 0x3D,
	DW_AT_ENCODING = 0x3E,
	DW_AT_ExTERNAL = 0x3F,
	DW_AT_FRAME_BASE = 0x40,
	DW_AT_FRIEND = 0x41,
	DW_AT_IDENTIFIER_CASE = 0x42,
	DW_AT_MACRO_INFO = 0x43,
	DW_AT_NAMELIST_ITEM = 0x44,
	DW_AT_PRIORITY = 0x45,
	DW_AT_SEGMENT = 0x46,
	DW_AT_SPECIFICATION = 0x47,
	DW_AT_STATIC_LINK = 0x48,
	DW_AT_TYPE = 0x49,
	DW_AT_USE_LOCATION = 0x4A,
	DW_AT_VARIABLE_PARAMETER = 0x4B,
	DW_AT_VIRTUALITY = 0x4C,
	DW_AT_VTABLE_ELEM_LOCATION = 0x4D,
	DW_AT_ALLOCATED = 0x4E,
	DW_AT_ASSOCIATED = 0x4F,
	DW_AT_DATA_LOCATION = 0x50,
	DW_AT_BYTE_STRIDE = 0x51,
	DW_AT_ENTRY_PC = 0x52,
	DW_AT_USE_UTF8 = 0x53,
	DW_AT_ExTENSION = 0x54,
	DW_AT_RANGES = 0x55,
	DW_AT_TRAMPOLINE = 0x56,
	DW_AT_CALL_COLUMN = 0x57,
	DW_AT_CALL_FILE = 0x58,
	DW_AT_CALL_LINE = 0x59,
	DW_AT_DESCRIPTION = 0x5A,
	DW_AT_BINARY_SCALE = 0x5B,
	DW_AT_DECIMAL_SCALE = 0x5C,
	DW_AT_SMALL = 0x5D,
	DW_AT_DECIMAL_SIGN = 0x5E,
	DW_AT_DIGIT_COUNT = 0x5F,
	DW_AT_PICTURE_STRING = 0x60,
	DW_AT_MUTABLE = 0x61,
	DW_AT_THREADS_SCALED = 0x62,
	DW_AT_ExPLICIT = 0x63,
	DW_AT_OBJECT_POINTER = 0x64,
	DW_AT_ENDIANITY = 0x65,
	DW_AT_ELEMENTAL = 0x66,
	DW_AT_PURE = 0x67,
	DW_AT_RECURSIVE = 0x68,
	DW_AT_MIPS_LINKAGE_NAME = 0x2007,
	DW_AT_SF_NAMES = 0x2101,
	DW_AT_SRC_INFO = 0x2102,
	DW_AT_MAC_INFO = 0x2103,
	DW_AT_SRC_COORDS = 0x2104,
	DW_AT_BODY_BEGIN = 0x2105,
	DW_AT_BODY_END = 0x2106,
	DW_AT_GNU_VECTOR = 0x2107,
	DW_AT_LO_USER = 0x2000,
	DW_AT_HI_USER = 0x3FFF,
};

// DWARF2 standard, figure 19.                     
enum DWARF2AttributeForm {
	DW_FORM_ADDR = 0x01,
	DW_FORM_BLOCK2 = 0x03,
	DW_FORM_BLOCK4 = 0x04,
	DW_FORM_DATA2 = 0x05,
	DW_FORM_DATA4 = 0x06,
	DW_FORM_DATA8 = 0x07,
	DW_FORM_STRING = 0x08,
	DW_FORM_BLOCK = 0x09,
	DW_FORM_BLOCK1 = 0x0A,
	DW_FORM_DATA1 = 0x0B,
	DW_FORM_FLAG = 0x0C,
	DW_FORM_SDATA = 0x0D,
	DW_FORM_STRP = 0x0E,
	DW_FORM_UDATA = 0x0F,
	DW_FORM_REF_ADDR = 0x10,
	DW_FORM_REF1 = 0x11,
	DW_FORM_REF2 = 0x12,
	DW_FORM_REF4 = 0x13,
	DW_FORM_REF8 = 0x14,
	DW_FORM_REF_UDATA = 0x15,
	DW_FORM_INDIRECT = 0x16,
	DW_FORM_EXPRLOC = 0x18,

};




class DebugAbbrevSection : public PESection {
public:
	DebugAbbrevSection() : PESection("/4", 0x42100040) {

	}

	void PESection::WriteBody(Writer & w) override;
};

class DebugFrameSection : public PESection {
public:
	DebugFrameSection(AssemblySymbol * code) : PESection("/42", 0x42100040), _code(code) {

	}

	void PESection::WriteBody(Writer & w) override;
private:
	AssemblySymbol * _code;
};


class DebugLineSection : public PESection {
public:

	DebugLineSection(AssemblySymbol * description) : PESection("/18", 0x42100040), _description(description) {
	}

	void WriteExtendedOpcode(Writer & w, DWARF2ExtendedOpcode op, size_t operands_size);
	void PESection::WriteBody(Writer & w) override;
private:
	AssemblySymbol * _description;
};

class DebugInfoSection : public PESection {
public:
	DebugInfoSection(AssemblySymbol * description) : PESection("/30", 0x42100040), _description(description) {

	}

	void PESection::WriteBody(Writer & w) override;
private:
	AssemblySymbol * _description;
};
