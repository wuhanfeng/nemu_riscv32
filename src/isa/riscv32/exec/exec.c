#include "cpu/exec.h"
#include "../local-include/decode.h"
#include "all-instr.h"

static inline void set_width(DecodeExecState *s, int width)
{
	if (width != 0)
		s->width = width;
}

// static inline def_EHelper(load)
static inline void exec_load (DecodeExecState *s)
{
	switch (s->isa.instr.i.funct3)
	{
		case 2: set_width(s, 4); ; exec_ld(s); break;
	default:
		exec_inv(s);
	}
}

// static inline def_EHelper(store)
static inline void exec_store (DecodeExecState *s)
{
	switch (s->isa.instr.s.funct3)
	{
		case 2: set_width(s, 4); ; exec_st(s); break;
	default:
		exec_inv(s);
	}
}

static inline void fetch_decode_exec(DecodeExecState *s)
{
	s->isa.instr.val = instr_fetch(&s->seq_pc, 4);
	Assert(s->isa.instr.i.opcode1_0 == 0x3, "Invalid instruction");
	switch (s->isa.instr.i.opcode6_2)
	{
		// IDEX(0b00000, I, load)
		// IDEX(0b01000, S, store)
		// IDEX(0b01101, U, lui)
		// EX(0b11010, nemu_trap)
		case 0b00000: set_width(s, 0); decode_I(s); exec_load(s); break;
		case 0b01000: set_width(s, 0); decode_S(s); exec_store(s); break;
		case 0b01101: set_width(s, 0); decode_U(s); exec_lui(s); break;
		case 0b11010: set_width(s, 0); ; exec_nemu_trap(s); break;
	default:
		exec_inv(s);
	}
}

static inline void reset_zero()
{
	reg_l(0) = 0;
}

vaddr_t isa_exec_once()
{
	DecodeExecState s;
	s.is_jmp = 0;
	s.seq_pc = cpu.pc;

	fetch_decode_exec(&s);
	update_pc(&s);

	reset_zero();

	return s.seq_pc;
}
