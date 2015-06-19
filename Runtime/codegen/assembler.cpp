
#include "assembler.h"
#include "../checks.h"

namespace r {

	void LineInfo::SetPosition(int pc, Location location) {
		_pcInfo->Push(PCInfo(pc, location));
	}

	void Operand::SetModRM(unsigned char mod, Register rm)
	{
		//ASSERT(_length == 1);
		_buffer[0] = mod << 6 | rm;
		_length = 1;
	}

	void Operand::SetDisplacement(char displacement)
	{
		_buffer[_length] = displacement;
		_length++;
	}

	Operand::Operand(Register base)
	{
		SetModRM(3, base);
	}

	Operand::Operand(Register base, char displacement)
	{
		SetModRM(1, base);
		SetDisplacement(displacement);
	}

	Assembler::Assembler(unsigned char * buffer, int bufferSize) {
		_buffer = buffer;
		_bufferSize = bufferSize;
		_pc = buffer;
	}

	Assembler::Assembler() {
		_bufferSize = 1024;
		_buffer = new unsigned char[_bufferSize];
		_pc = _buffer;

		for (int i = 0; i < _bufferSize; i++) {
			_buffer[i] = 0xCC;
		}
	}

	void Assembler::Emit(unsigned char x)
	{
		//EnsureCapacity
		*_pc++ = x;
	}

	void Assembler::Emit4(unsigned int x)
	{
		//EnsureCapacity
		*((int *)_pc) = x;
		_pc += sizeof(int);
	}

	void Assembler::Mov(Register dst, Register src) {
		Emit(0x89);
		Emit(0xC0 | src << 3 | dst);
	}

	void Assembler::Mov(Register dst, unsigned int immediate) {
		Emit(0xB8 + dst);
		Emit4(immediate);
	}

	void Assembler::Mov(Operand & dst, unsigned int immediate) {
		Emit(0xC7);
		EmitOperand(EAX, dst);
		Emit4(immediate);
	}

	void Assembler::Mov(Operand & dst, Register src) {
		Emit(0x89);
		EmitOperand(src, dst);
	}

	void Assembler::Mov(Register dst, Operand & src) {
		Emit(0x8B);
		EmitOperand(dst, src);
	}

	void Assembler::Addsd(XMMRegister dst, XMMRegister src) {
		Emit(0xF2);
		Emit(0x0F);
		Emit(0x58);
		EmitSSEOperand(dst, src);
	}

	void Assembler::Subsd(XMMRegister dst, XMMRegister src) {
		Emit(0xF2);
		Emit(0x0F);
		Emit(0x5C);
		EmitSSEOperand(dst, src);
	}

	void Assembler::Mulsd(XMMRegister dst, XMMRegister src) {
		Emit(0xF2);
		Emit(0x0F);
		Emit(0x59);
		EmitSSEOperand(dst, src);
	}


	void Assembler::Movd(XMMRegister dst, Operand& src) {
		Emit(0x66);
		Emit(0x0F);
		Emit(0x6E);
		EmitOperand(dst, src);
	}

	void Assembler::Movd(Operand& dst, XMMRegister src) {
		Emit(0x66);
		Emit(0x0F);
		Emit(0x7E);
		EmitOperand(src, dst);
	}


	void Assembler::Cmpsd(SSECondition condition, XMMRegister dst, XMMRegister src) {
		Emit(0xF2);
		Emit(0x0F);
		Emit(0xC2);
		EmitSSEOperand(dst, src);
		Emit((unsigned char)condition);
	}


	void Assembler::Movsd(Operand & dst, XMMRegister src) {
		Emit(0xF2);
		Emit(0x0F);
		Emit(0x11);
		EmitOperand(src, dst);
	}


	void Assembler::Movsd(XMMRegister dst, Operand & src)
	{
		Emit(0xF2);
		Emit(0x0F);
		Emit(0x10);

		EmitOperand(dst, src);
	}

	void Assembler::EmitSSEOperand(XMMRegister dst, XMMRegister src) {
		Emit(0xC0 | dst << 3 | src);
	}

	void Assembler::Test(Register dst, Register src)
	{
		Emit(0x85);
		Emit(0xC0 | src << 3 | dst);
	}

	void Assembler::Nop()
	{
		Emit(0x90);
	}

	void Assembler::Bind(Label & label)
	{
		label.SetPosition((int)_pc);
		
		for (int value : *label.GetUnresolvedPositions()) {
			int offset = (int)_pc - value;
			
			*((int *)value) = (unsigned char)(offset - sizeof(int32_t));
		}
	}

	void Assembler::Jne(Label & label)
	{
		Emit(0x0F);
		Emit(0x85);
		if (label.IsBound()) {
			int offset = label.GetPosition() - (int)_pc;
			Emit4(offset - 1);
		}
		else {
			label.GetUnresolvedPositions()->Push((int)_pc);
			_pc += sizeof(int);
		}
	}


	void Assembler::Je(Label & label)
	{
		Emit(0x0F);
		Emit(0x84);
		if (label.IsBound()) {
			int offset = label.GetPosition() - (int)_pc;
			Emit4(offset - 1);
		} else {
			label.GetUnresolvedPositions()->Push((int)_pc);
			_pc += sizeof(int);
		}
	}

	void Assembler::Jmp(Label & label)
	{
		Emit(0xE9);
		if (label.IsBound()) {
			int offset = label.GetPosition() - (int)_pc;
			//ASSERT(offset < 0);
			Emit4(offset - 1);
		} else {
			label.GetUnresolvedPositions()->Push((int)_pc);
			_pc += sizeof(int);
		}
	}

	void Assembler::Dec(Register dst) {
		Emit(0x48 | dst);
	}

	void Assembler::Inc(Register dst) {
		Emit(0x40 | dst);
	}

	void Assembler::Ret() {
		Emit(0xC3);
	}

	void Assembler::MagicBreakpoint() {
		Emit(0x66); // 16 bit
		Emit(0x87);
		Emit(0xDB); // bx, bx
	}

	void Assembler::Xchg(Register dst, Register src) {
		NOT_IMPLEMENTED();
	}

	void Assembler::Cmp(Register dst, Register src) {
		Emit(0x39);
		Emit(0xC0 | src << 3 | dst);
	}
	
	void Assembler::Cmp(Operand & operand, unsigned int immediate) {
		EmitArithmetic(7, operand, immediate);
	}

	void Assembler::Setcc(Condition cc, Register dst) {
		Emit(0x0F);
		Emit(0x90 | (unsigned char)cc);
		Emit(0xC0 | dst);
	}

	void Assembler::Shr(Register dst, unsigned char immediate) {
		
		if (immediate == 1) {
			Emit(0xD1);
			Emit(0xE8 | dst);
		}
		else {
			Emit(0xC1);
			Emit(0xE8 | dst);
			Emit(immediate);
		}
	}

	void Assembler::Pinsrd(XMMRegister dst, Operand& src, int8_t offset) {
		Emit(0x66);
		Emit(0x0F);
		Emit(0x3A);
		Emit(0x22);
		EmitOperand(dst, src);
		Emit(offset);
	}

	void Assembler::Sub(Register dst, Register src) {
		Emit(0x29);
		Emit(0xC0 | src << 3 | dst);
	}

	void Assembler::Sub(Register dst, unsigned int immediate) {
		Emit(0x81);
		Emit(0xEC | dst);
		*((int *)_pc) = immediate;
		_pc += sizeof(int);
	}

	void Assembler::Mul(Register src) {
		Emit(0xF7);
		Emit(0xE0 | src);
	}

	void Assembler::Add(Register dst, Register src) {
		Emit(0x01);
		Emit(0xC0 | src << 3 | dst);
	}


	void Assembler::Add(Operand dst, unsigned int immediate) {
		
		EmitArithmetic(0, dst, immediate);
	}

	void Assembler::EmitArithmetic(int operation, Operand dst, unsigned int immediate) {

		if (dst.IsRegister(EAX)) {

			Emit((operation << 3) | 0x05);

			Emit4(immediate);
		}
		else {
			Emit(0x81);
			EmitOperand(operation, dst);

			Emit4(immediate);
		}
	}

	void Assembler::Pop(Register reg) {
		Emit(0x58 + reg);
	}

	void Assembler::Push(int value) {
		Emit(0x68);

		*((int *)_pc) = value;
		_pc += sizeof(int);
	}

	void Assembler::Push(Register reg) {
		Emit(0x50 + reg);
	}

	void Assembler::Push(Operand src) {
		Emit(0xFF);
		EmitOperand(ESI, src);
	}

	void Assembler::Call(Operand & address) {
		Emit(0xFF);
		EmitOperand(EDX, address);
	}

	void Assembler::Call(unsigned char* address) {
		Emit(0xE8);

		*((int *)_pc) = (int)(address - (_pc + sizeof(int)));
		_pc += sizeof(int);
	}

	void Assembler::StartLineRecording() {
		_lineInfo = new LineInfo();
	}

	LineInfo * Assembler::EndLineRecording() {
		LineInfo * result = _lineInfo;
		_lineInfo = nullptr;
		return result;
	}

	void Assembler::RecordPosition(Location location) {
		if (_lineInfo != nullptr) {
			_lineInfo->SetPosition(_pc - _buffer, location);
		}
	}

}
