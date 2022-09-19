#include "../local-include/rtl.h"

#include "compute.h"
#include "control.h"
#include "ldst.h"
#include "muldiv.h"
#include "system.h"

// def_EHelper(inv);
// def_EHelper(nemu_trap);

void exec_inv(DecodeExecState * s);
void exec_nemu_trap(DecodeExecState * s);
