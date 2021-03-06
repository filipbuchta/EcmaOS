#pragma once

#include "../list.h"
#include "../location.h"

namespace r {
	
	struct PCInfo {
		PCInfo(int pc, Location location) : PC(pc), Location(location) { }
		int PC;
		Location Location;
	};
	struct LineInfo {
	public:
		void SetPosition(int pc, Location location);
		List<PCInfo> * GetPCInfo() { return _pcInfo; }
	private:
		List<PCInfo> * _pcInfo = new List<PCInfo>();
	};

	class Label {
	public:
		bool IsBound() { return _position != 0; }
		void SetPosition(int value) { _position = value; }
		int GetPosition() { return _position; }
		List<int> * GetUnresolvedPositions() { return _unresolvedPositions; }
	private:
		int _position = 0;
		List<int> * _unresolvedPositions = new List<int>();
	};

	enum class Condition {
		None = -1,
		Equal = 4,
		NotEqual = 5,
		Less = 12,
		GreaterEqual = 13,
		LessEqual = 14,
		Greater = 15,
	};

	enum class SSECondition {
		Equal = 0,
		Less = 1,
		LessEqual = 2,
		NotEqual = 4,
		GreaterEqual = 5,
		Greater = 6
	};

	enum class Register {
		EAX = 0,
		ECX = 1,
		EDX = 2,
		EBX = 3,
		ESP = 4,
		EBP = 5,
		ESI = 6,
		EDI = 7,
	};

	enum class XMMRegister {
		XMM0 = 0,
		XMM1 = 1,
	};

	class Assembler;

	class Operand {
	public:
		Operand::Operand(Register base);
		Operand(Register base, char displacement);
		void SetModRM(unsigned char mod, Register rm);
		void SetDisplacement(char displacement);
		bool IsRegister(Register reg) { return ((_buffer[0] & 0xF8) == 0xC0) && ((_buffer[0] & 0x07) == (int8)reg);  }
	private:
		unsigned char * _buffer = new unsigned char[6];
		int _length = 0;

		friend class Assembler;
	};


	class Assembler {


	public:
		Assembler(unsigned char * buffer, int bufferSize);
		Assembler();

		void StartLineRecording();
		LineInfo * EndLineRecording();
		void RecordPosition(Location location);


		void Emit(unsigned char x);
		void Emit4(unsigned int x);

		void EmitOperand(int reg, Operand & operand) {

			_pc[0] = (operand._buffer[0] & ~0x38) | (reg << 3);

			for (int i = 1; i < operand._length; i++) {
				_pc[i] = operand._buffer[i];
			}

			_pc += 1;
			_pc += operand._length - 1;

		}

		void Bind(Label & label);

		void Xchg(Register dst, Register src);

		void Cmp(Register dst, Register src);
		void Shr(Register dst, unsigned char immediate);
		void Setcc(Condition cc, Register dst);
		void Dec(Register dst);
		void Inc(Register dst);

		void Sub(Register dst, Register src);
		void Sub(Register dst, unsigned int immediate);

		void Div(Register src);

		void Mul(Register src);

		void Pinsrd(XMMRegister dst, Operand& src, int8 offset);

		void Add(Register dst, Register src);
		void Add(Operand dst, unsigned int immediate);
		void Add(Operand dst, Register src);

		void Movzx(Register dst, Operand src);
		void Mov(Register dst, Register src);
		void Mov(Operand & dst, unsigned int immediate);
		void Mov(Register dst, unsigned int immediate);
		void Movd(Operand& dst, XMMRegister src);
		void Movd(XMMRegister dst, Operand& src);

		void Mov(Operand & dst, Register src);
		void Mov(Register dst, Operand & src);
		void Movsd(XMMRegister dst, Operand & src);
		
		void Movsd(Operand & dst, XMMRegister src);

		void Addsd(XMMRegister dst, XMMRegister src);
		void Subsd(XMMRegister dst, XMMRegister src);
		void Mulsd(XMMRegister dst, XMMRegister src);
		void Cmpsd(SSECondition condition, XMMRegister dst, XMMRegister src);
		void Cmp(Operand & operand, unsigned int immediate);

		void Test(Register dst, Register src);

		void Nop();

		void J(Condition condition, Label & label);
		void Jne(Label & label);
		void Jl(Label & label);
		void Jg(Label & label);
		void Jle(Label & label);
		void Jge(Label & label);
		void Je(Label & label);

		void Jmp(Label & label);

		void Ret();

		void Pop(Register reg);

		void Push(int value);
		void Push(Register reg);
		void Push(Operand operand);

		void Call(Operand & address);
		void Call(unsigned char* address);


		void EmitSSEOperand(XMMRegister dst, XMMRegister src);
		void EmitArithmetic(int operation, Operand dst, unsigned int immediate);

		void MagicBreakpoint();



		unsigned char *GetBuffer() const { return _buffer; }
		unsigned char *GetPC() const { return _pc; }
		int GetBufferSize() { return _bufferSize; }

	private:
		unsigned char* _pc;
		unsigned char* _buffer = nullptr;
		int _bufferSize = 0;


		LineInfo * _lineInfo;

	};

}

