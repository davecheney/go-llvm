//go:build !llvm23

package llvm

/*
#include "llvm-c/Core.h"
*/
import "C"

// LLVM 23 split Br into UncondBr and CondBr and removed it, so code that
// matches on branch opcodes must handle both forms to build against LLVM
// 23 as well as earlier releases.
const Br Opcode = C.LLVMBr
