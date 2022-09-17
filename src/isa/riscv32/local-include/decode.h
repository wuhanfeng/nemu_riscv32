#include "cpu/exec.h"
#include "rtl.h"

// decode operand helper
#define def_DopHelper(name) \
	void concat(decode_op_, name)(DecodeExecState * s, Operand * op, word_t val, bool load_val)

// static inline def_DopHelper(i)
static inline void decode_op_i (DecodeExecState *s, Operand *op, word_t val, bool load_val)
{
	op->type = OP_TYPE_IMM;
	op->imm = val;

	print_Dop(op->str, OP_STR_SIZE, "%d", op->imm);
}

// static inline def_DopHelper(r)
static inline void decode_op_r (DecodeExecState *s, Operand *op, word_t val, bool load_val)
{
	op->type = OP_TYPE_REG;
	op->reg = val;
	// op->preg = &reg_l(val);
	op->preg = &(cpu.gpr[check_reg_index(val)]._32);

	print_Dop(op->str, OP_STR_SIZE, "%s", reg_name(op->reg));
}

// static inline def_DHelper(I)
static inline void decode_I(DecodeExecState * s)
{
	decode_op_r(s, id_src1, s->isa.instr.i.rs1, true);
	decode_op_i(s, id_src2, s->isa.instr.i.simm11_0, true);
	decode_op_r(s, id_dest, s->isa.instr.i.rd, false);
}

// static inline def_DHelper(U)
static inline void decode_U(DecodeExecState * s)
{
	decode_op_i(s, id_src1, s->isa.instr.u.imm31_12 << 12, true);
	decode_op_r(s, id_dest, s->isa.instr.u.rd, false);

	print_Dop(id_src1->str, OP_STR_SIZE, "0x%x", s->isa.instr.u.imm31_12);
}

// static inline def_DHelper(S)
static inline void decode_S(DecodeExecState * s)
{
	decode_op_r(s, id_src1, s->isa.instr.s.rs1, true);
	sword_t simm = (s->isa.instr.s.simm11_5 << 5) | s->isa.instr.s.imm4_0;
	decode_op_i(s, id_src2, simm, true);
	decode_op_r(s, id_dest, s->isa.instr.s.rs2, true);
}
