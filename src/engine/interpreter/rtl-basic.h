#ifndef __RTL_BASIC_H__
#define __RTL_BASIC_H__

#include "c_op.h"
#include <memory/vaddr.h>

/* RTL basic instructions */

#define def_rtl_compute_reg(name)                                                           \
	static inline def_rtl(name, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) \
	{                                                                                       \
		*dest = concat(c_, name)(*src1, *src2);                                             \
	}

#define def_rtl_compute_imm(name)                                                           \
	static inline def_rtl(name##i, rtlreg_t *dest, const rtlreg_t *src1, const sword_t imm) \
	{                                                                                       \
		*dest = concat(c_, name)(*src1, imm);                                               \
	}

#define def_rtl_compute_reg_imm(name) \
	def_rtl_compute_reg(name)         \
		def_rtl_compute_imm(name)

// compute

static inline void rtl_add(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) { *dest = ((*src1) + (*src2)); }
static inline void rtl_addi(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const sword_t imm) { *dest = ((*src1) + (imm)); }
static inline void rtl_sub(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) { *dest = ((*src1) - (*src2)); }
static inline void rtl_subi(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const sword_t imm) { *dest = ((*src1) - (imm)); }
static inline void rtl_and(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) { *dest = ((*src1) & (*src2)); }
static inline void rtl_andi(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const sword_t imm) { *dest = ((*src1) & (imm)); }
static inline void rtl_or(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) { *dest = ((*src1) | (*src2)); }
static inline void rtl_ori(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const sword_t imm) { *dest = ((*src1) | (imm)); }
static inline void rtl_xor(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) { *dest = ((*src1) ^ (*src2)); }
static inline void rtl_xori(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const sword_t imm) { *dest = ((*src1) ^ (imm)); }
static inline void rtl_shl(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) { *dest = ((*src1) << ((*src2) & 0x1f)); }
static inline void rtl_shli(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const sword_t imm) { *dest = ((*src1) << ((imm)&0x1f)); }
static inline void rtl_shr(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) { *dest = ((*src1) >> ((*src2) & 0x1f)); }
static inline void rtl_shri(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const sword_t imm) { *dest = ((*src1) >> ((imm)&0x1f)); }
static inline void rtl_sar(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) { *dest = ((sword_t)(*src1) >> ((*src2) & 0x1f)); }
static inline void rtl_sari(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const sword_t imm) { *dest = ((sword_t)(*src1) >> ((imm)&0x1f)); }

#ifdef ISA64
def_rtl_compute_reg_imm(addw)
	def_rtl_compute_reg_imm(subw)
		def_rtl_compute_reg_imm(shlw)
			def_rtl_compute_reg_imm(shrw)
				def_rtl_compute_reg_imm(sarw)
#define rtl_addiw rtl_addwi
#define rtl_shliw rtl_shlwi
#define rtl_shriw rtl_shrwi
#define rtl_sariw rtl_sarwi
#endif

	// static inline def_rtl(setrelop, uint32_t relop, rtlreg_t *dest,
	// 																		  const rtlreg_t *src1, const rtlreg_t *src2)
static inline void rtl_setrelop(DecodeExecState *s, uint32_t relop, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2)
{
	*dest = interpret_relop(relop, *src1, *src2);
}

// static inline def_rtl(setrelopi, uint32_t relop, rtlreg_t *dest,
// 					  const rtlreg_t *src1, sword_t imm)
static inline void rtl_setrelopi(DecodeExecState *s, uint32_t relop, rtlreg_t *dest, const rtlreg_t *src1, sword_t imm)
{
	*dest = interpret_relop(relop, *src1, imm);
}

// mul/div
static inline void rtl_mul_lo(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) { *dest = ((*src1) * (*src2)); }
static inline void rtl_mul_hi(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) { *dest = (((uint64_t)(*src1) * (uint64_t)(*src2)) >> 32); }
static inline void rtl_imul_lo(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) { *dest = ((sword_t)(*src1) * (sword_t)(*src2)); }
static inline void rtl_imul_hi(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) { *dest = (((int64_t)(sword_t)(*src1) * (int64_t)(sword_t)(*src2)) >> 32); }
static inline void rtl_div_q(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) { *dest = ((*src1) / (*src2)); }
static inline void rtl_div_r(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) { *dest = ((*src1) % (*src2)); }
static inline void rtl_idiv_q(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) { *dest = ((sword_t)(*src1) / (sword_t)(*src2)); }
static inline void rtl_idiv_r(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const rtlreg_t *src2) { *dest = ((sword_t)(*src1) % (sword_t)(*src2)); }
// def_rtl_compute_reg(mul_lo)
// 	def_rtl_compute_reg(mul_hi)
// 		def_rtl_compute_reg(imul_lo)
// 			def_rtl_compute_reg(imul_hi)
// 				def_rtl_compute_reg(div_q)
// 					def_rtl_compute_reg(div_r)
// 						def_rtl_compute_reg(idiv_q)
// 							def_rtl_compute_reg(idiv_r)

#ifdef ISA64
def_rtl_compute_reg(mulw)
	def_rtl_compute_reg(divw)
		def_rtl_compute_reg(divuw)
			def_rtl_compute_reg(remw)
				def_rtl_compute_reg(remuw)
#endif

	// static inline def_rtl(div64_q, rtlreg_t *dest,
	// 																		  const rtlreg_t *src1_hi, const rtlreg_t *src1_lo, const rtlreg_t *src2)
static inline void rtl_div64_q(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1_hi, const rtlreg_t *src1_lo, const rtlreg_t *src2)
{
	uint64_t dividend = ((uint64_t)(*src1_hi) << 32) | (*src1_lo);
	uint32_t divisor = (*src2);
	*dest = dividend / divisor;
}

// static inline def_rtl(div64_r, rtlreg_t *dest,
// 					  const rtlreg_t *src1_hi, const rtlreg_t *src1_lo, const rtlreg_t *src2)
static inline void rtl_div64_r(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1_hi, const rtlreg_t *src1_lo, const rtlreg_t *src2)
{
	uint64_t dividend = ((uint64_t)(*src1_hi) << 32) | (*src1_lo);
	uint32_t divisor = (*src2);
	*dest = dividend % divisor;
}

// static inline def_rtl(idiv64_q, rtlreg_t *dest,
// 					  const rtlreg_t *src1_hi, const rtlreg_t *src1_lo, const rtlreg_t *src2)
static inline void rtl_idiv64_q(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1_hi, const rtlreg_t *src1_lo, const rtlreg_t *src2)
{
	int64_t dividend = ((uint64_t)(*src1_hi) << 32) | (*src1_lo);
	int32_t divisor = (*src2);
	*dest = dividend / divisor;
}

// static inline def_rtl(idiv64_r, rtlreg_t *dest,
// 					  const rtlreg_t *src1_hi, const rtlreg_t *src1_lo, const rtlreg_t *src2)
static inline void rtl_idiv64_r(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1_hi, const rtlreg_t *src1_lo, const rtlreg_t *src2)
{
	int64_t dividend = ((uint64_t)(*src1_hi) << 32) | (*src1_lo);
	int32_t divisor = (*src2);
	*dest = dividend % divisor;
}

// memory

static inline void rtl_lm(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *addr, word_t offset, int len)
{
	*dest = vaddr_read(*addr + offset, len);
}

static inline void rtl_sm(DecodeExecState *s, const rtlreg_t *addr, word_t offset, const rtlreg_t *src1, int len)
{
	vaddr_write(*addr + offset, *src1, len);
}

// static inline def_rtl(lms, rtlreg_t *dest, const rtlreg_t *addr, word_t offset, int len)
static inline void rtl_lms(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *addr, word_t offset, int len)
{
	word_t val = vaddr_read(*addr + offset, len);
	switch (len)
	{
	case 4:
		*dest = (sword_t)(int32_t)val;
		return;
	case 1:
		*dest = (sword_t)(int8_t)val;
		return;
	case 2:
		*dest = (sword_t)(int16_t)val;
		return;
	default:
		assert(0);
	}
}

// static inline def_rtl(host_lm, rtlreg_t *dest, const void *addr, int len)
static inline void rtl_host_lm(DecodeExecState *s, rtlreg_t *dest, const void *addr, int len)
{
	switch (len)
	{
	case 4:
		*dest = *(uint32_t *)addr;
		return;
	case 1:
		*dest = *(uint8_t *)addr;
		return;
	case 2:
		*dest = *(uint16_t *)addr;
		return;
	default:
		assert(0);
	}
}

// static inline def_rtl(host_sm, void *addr, const rtlreg_t *src1, int len)
static inline void rtl_host_sm(DecodeExecState *s, void *addr, const rtlreg_t *src1, int len)
{
	switch (len)
	{
	case 4:
		*(uint32_t *)addr = *src1;
		return;
	case 1:
		*(uint8_t *)addr = *src1;
		return;
	case 2:
		*(uint16_t *)addr = *src1;
		return;
	default:
		assert(0);
	}
}

// control

// static inline def_rtl(j, vaddr_t target)
static inline void rtl_j(DecodeExecState *s, vaddr_t target)
{
	s->jmp_pc = target;
	s->is_jmp = true;
}

// static inline def_rtl(jr, rtlreg_t *target)
static inline void rtl_jr(DecodeExecState *s, rtlreg_t *target)
{
	s->jmp_pc = *target;
	s->is_jmp = true;
}

// static inline def_rtl(jrelop, uint32_t relop,
// 					  const rtlreg_t *src1, const rtlreg_t *src2, vaddr_t target)
static inline void rtl_jrelop(DecodeExecState *s, uint32_t relop, const rtlreg_t *src1, const rtlreg_t *src2, vaddr_t target)
{
	bool is_jmp = interpret_relop(relop, *src1, *src2);
	if (is_jmp)
		rtl_j(s, target);
}
#endif
