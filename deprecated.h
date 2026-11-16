//===- deprecated.h - Wrappers for deprecated LLVM C API --------*- C -*-===//
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

#ifndef LLVM_BINDINGS_GO_LLVM_DEPRECATED_H
#define LLVM_BINDINGS_GO_LLVM_DEPRECATED_H

#include "llvm-c/Core.h"

#ifdef __cplusplus
extern "C" {
#endif

LLVMContextRef LLVMGetGlobalContext_wrap(void);
LLVMValueRef LLVMAlignOf_wrap(LLVMTypeRef Ty);
LLVMValueRef LLVMSizeOf_wrap(LLVMTypeRef Ty);
LLVMValueRef LLVMConstGEP2_wrap(LLVMTypeRef Ty, LLVMValueRef ConstantVal,
                                LLVMValueRef *ConstantIndices,
                                unsigned NumIndices);
LLVMValueRef LLVMConstInBoundsGEP2_wrap(LLVMTypeRef Ty,
                                        LLVMValueRef ConstantVal,
                                        LLVMValueRef *ConstantIndices,
                                        unsigned NumIndices);
LLVMBuilderRef LLVMCreateBuilderInContext_wrap(LLVMContextRef C);

#ifdef __cplusplus
}
#endif

#endif // LLVM_BINDINGS_GO_LLVM_DEPRECATED_H
