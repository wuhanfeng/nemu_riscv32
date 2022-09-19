// #define def_EHelper(name) void concat(exec_, name)(DecodeExecState * s)
static inline void exec_lui(DecodeExecState * s)
{
	rtl_li(s, ddest, id_src1->imm);
	print_asm_template2(lui);
}
