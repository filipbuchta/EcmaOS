
#include "assembler.h"
#include "checks.h"

namespace r {

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

	Operand::Operand(Register base, char displacement)
	{
		SetModRM(1, base);
		SetDisplacement(displacement);
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
		*_pc++ = x;
	}

	void Assembler::Mov(Register dst, Register src) {
		Emit(0x89);
		Emit(0xC0 | src << 3 | dst);
	}

	void Assembler::Mov(Register dst, unsigned int immediate) {
		Emit(0xB8 + dst);
		*((int *)_pc) = immediate;
		_pc += sizeof(int);
	}

	void Assembler::EmitOperand(Operand & operand, Register reg) {
		_pc[0] = (operand._buffer[0] & ~0x38) | (reg << 3);
		for (int i = 1; i < operand._length; i++) {
			_pc[i] = operand._buffer[i];
		}
		_pc += 1;
		_pc += operand._length - 1;
	}

	void Assembler::Mov(Operand & dst, Register src) {
		Emit(0x89);
		EmitOperand(dst, src);
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
			
			*((int *)value) = (char)(offset - 2);
		}
	}

	void Assembler::Jne(Label & label)
	{
		if (label.IsBound()) {
			int offset = label.GetPosition() - (int)_pc;
			Emit(0x75);
			Emit((char)(offset - 2));
		}
		else {
			Emit(0x0F);
			Emit(0x85);
			label.GetUnresolvedPositions()->Push((int)_pc);
			_pc += sizeof(int);
		}
	}


	void Assembler::Je(Label & label)
	{
		if (label.IsBound()) {
			int offset = label.GetPosition() - (int)_pc;
			Emit(0x74);
			Emit((char)(offset - 2));
		} else {
			Emit(0x0F);
			Emit(0x84);
			label.GetUnresolvedPositions()->Push((int)_pc);
			_pc += sizeof(int);
		}
	}

	void Assembler::Jmp(Label & label)
	{
		if (label.IsBound()) {
			int offset = label.GetPosition() - (int)_pc;
			//ASSERT(offset < 0);
			Emit(0xEB);
			Emit((char)(offset - 2));
		} else {
			Emit(0xE9);
			label.GetUnresolvedPositions()->Push((int)_pc);
			_pc += sizeof(int);
		}
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

	void Assembler::Sub(Register dst, Register src) {
		Emit(0x29);
		Emit(0xC0 | src << 3 | dst);
	}

	void Assembler::Add(Register dst, Register src) {
		Emit(0x01);
		Emit(0xC0 | src << 3 | dst);
	}

	void Assembler::Pop(Register reg) {
		Emit(0x58 + reg);
	}

	void Assembler::Push(int value) {
		Emit(0x68);

		*((int *)_pc) = value;
		_pc += sizeof(int);
	}

	void Assembler::Call(unsigned char* address) {
		Emit(0xE8);

		*((int *)_pc) = (int)(address - (_pc + sizeof(int)));
		_pc += sizeof(int);
	}

	void Assembler::Push(Register reg) {
		Emit(0x50 + reg);
	}

}
