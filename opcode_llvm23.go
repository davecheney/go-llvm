//go:build llvm23

package llvm

/*
#include "llvm-c/Core.h"
*/
import "C"

// LLVM 23 also changed the operand order of conditional branches to match
// successor order: before LLVM 23, operand 1 was the false destination and
// operand 2 the true one. Code that reaches successors through Operand
// needs to account for this; nothing about it fails to compile.
const (
	UncondBr Opcode = C.LLVMUncondBr
	CondBr   Opcode = C.LLVMCondBr
)
