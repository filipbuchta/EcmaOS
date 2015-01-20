#pragma once

#include "vector.h"

namespace r {

	class Label {
	public:
		bool IsBound() { return _position != 0; }
		void SetPosition(int value) { _position = value; }
		int GetPosition() { return _position; }
		Vector<int> * GetUnresolvedPositions() { return _unresolvedPositions; }
	private:
		int _position = 0;
		Vector<int> * _unresolvedPositions = new Vector<int>();
	};

	enum Register {
		EAX = 0,
		ECX = 1,
		EDX = 2,
		EBX = 3,
		ESP = 4,
		EBP = 5,
		ESI = 6,
		EDI = 7,
	};

	class Assembler;

	class Operand {
	public:
		Operand(Register base, char displacement);
		void SetModRM(unsigned char mod, Register rm);
		void SetDisplacement(char displacement);
	private:
		unsigned char * _buffer = new unsigned char[6];
		int _length = 0;

		friend class Assembler;
	};


	class Assembler {


	public:
		Assembler();

		void Emit(unsigned char x);

		void EmitOperand(Operand & operand, Register reg);

		void Bind(Label & label);

		void Xchg(Register dst, Register src);

		void Sub(Register dst, Register src);

		void Add(Register dst, Register src);

		void Mov(Register dst, Register src);

		void Mov(Register dst, unsigned int operand);

		void Mov(Operand & dst, Register src);

		void Test(Register dst, Register src);

		void Nop();

		void Je(Label & label);

		void Jmp(Label & label);

		void Ret();

		void Pop(Register reg);

		void Push(int value);

		void Call(unsigned char* address);

		void Push(Register reg);


		void MagicBreakpoint();


		unsigned char *GetBuffer() const { return _buffer; }
		unsigned char *GetPC() const { return _pc; }

	private:
		unsigned char* _pc;
		unsigned char* _buffer = nullptr;
		int _bufferSize = 0;

	};

}

