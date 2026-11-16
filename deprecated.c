//===- deprecated.c - Wrappers for deprecated LLVM C API --------*- C -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file wraps LLVM C API functions that are deprecated in newer LLVM
// releases but whose replacements are not available in every LLVM version
// these bindings support:
//
//   - LLVMGetGlobalContext, deprecated in LLVM 22 with no replacement.
//   - LLVMAlignOf, LLVMSizeOf, LLVMConstGEP2, LLVMConstInBoundsGEP2 and
//     LLVMCreateBuilderInContext, deprecated in LLVM 24.
//
// Other deprecated functions are handled by calling their context-aware
// counterparts from Go.
//
//===----------------------------------------------------------------------===//

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include "deprecated.h"

LLVMContextRef LLVMGetGlobalContext_wrap(void) {
  return LLVMGetGlobalContext();
}

LLVMValueRef LLVMAlignOf_wrap(LLVMTypeRef Ty) { return LLVMAlignOf(Ty); }

LLVMValueRef LLVMSizeOf_wrap(LLVMTypeRef Ty) { return LLVMSizeOf(Ty); }

LLVMValueRef LLVMConstGEP2_wrap(LLVMTypeRef Ty, LLVMValueRef ConstantVal,
                                LLVMValueRef *ConstantIndices,
                                unsigned NumIndices) {
  return LLVMConstGEP2(Ty, ConstantVal, ConstantIndices, NumIndices);
}

LLVMValueRef LLVMConstInBoundsGEP2_wrap(LLVMTypeRef Ty,
                                        LLVMValueRef ConstantVal,
                                        LLVMValueRef *ConstantIndices,
                                        unsigned NumIndices) {
  return LLVMConstInBoundsGEP2(Ty, ConstantVal, ConstantIndices, NumIndices);
}

LLVMBuilderRef LLVMCreateBuilderInContext_wrap(LLVMContextRef C) {
  return LLVMCreateBuilderInContext(C);
}

#pragma GCC diagnostic pop
