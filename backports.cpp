
#include "backports.h"
#include "llvm/IR/Instructions.h"
#if LLVM_VERSION_MAJOR >= 16
#include "llvm/IR/PassManager.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Transforms/IPO/ThinLTOBitcodeWriter.h"
#if LLVM_VERSION_MAJOR >= 23
#include "llvm/Transforms/Utils/AssignGUID.h"
#endif
#else
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#endif
#include "llvm/IR/Module.h"
#include "llvm/Linker/Linker.h"
#include "llvm/Pass.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Transforms/IPO.h"
#include "llvm-c/Core.h"
#include "llvm-c/DebugInfo.h"

void LLVMGlobalObjectAddMetadata(LLVMValueRef Global, unsigned KindID, LLVMMetadataRef MD) {
  llvm::MDNode *N = MD ? llvm::unwrap<llvm::MDNode>(MD) : nullptr;
  llvm::GlobalObject *O = llvm::unwrap<llvm::GlobalObject>(Global);
  O->addMetadata(KindID, *N);
}

// See https://reviews.llvm.org/D119431
LLVMMemoryBufferRef LLVMGoWriteThinLTOBitcodeToMemoryBuffer(LLVMModuleRef M) {
  std::string Data;
  llvm::raw_string_ostream OS(Data);
#if LLVM_VERSION_MAJOR >= 16
  llvm::LoopAnalysisManager LAM;
  llvm::FunctionAnalysisManager FAM;
  llvm::CGSCCAnalysisManager CGAM;
  llvm::ModuleAnalysisManager MAM;
  llvm::PassBuilder PB;
  PB.registerModuleAnalyses(MAM);
  PB.registerCGSCCAnalyses(CGAM);
  PB.registerFunctionAnalyses(FAM);
  PB.registerLoopAnalyses(LAM);
  PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);
  llvm::ModulePassManager MPM;
#if LLVM_VERSION_MAJOR >= 23
  // LLVM 23 requires AssignGUIDPass before any GlobalValue::getGUID() call.
  // ThinLTOBitcodeWriterPass needs it internally for the module summary.
  MPM.addPass(llvm::AssignGUIDPass());
#endif
  MPM.addPass(llvm::ThinLTOBitcodeWriterPass(OS, nullptr));
  MPM.run(*llvm::unwrap(M), MAM);
#else
  llvm::legacy::PassManager PM;
  PM.add(createWriteThinLTOBitcodePass(OS));
  PM.run(*llvm::unwrap(M));
#endif
  return llvm::wrap(llvm::MemoryBuffer::getMemBufferCopy(OS.str()).release());
}

void LLVMGoDIBuilderInsertDbgValueRecordAtEnd(
    LLVMDIBuilderRef Builder, LLVMValueRef Val, LLVMMetadataRef VarInfo,
    LLVMMetadataRef Expr, LLVMMetadataRef DebugLoc, LLVMBasicBlockRef Block) {
#if LLVM_VERSION_MAJOR >= 19
  // Note: this returns a LLVMDbgRecordRef. Previously, InsertValueAtEnd would
  // return a Value. But since the type changed, and I'd like to keep the API
  // consistent across LLVM versions, I decided to drop the return value.
  LLVMDIBuilderInsertDbgValueRecordAtEnd(Builder, Val, VarInfo, Expr, DebugLoc, Block);
#else
  // Old llvm.dbg.* API.
  LLVMDIBuilderInsertDbgValueAtEnd(Builder, Val, VarInfo, Expr, DebugLoc, Block);
#endif
}

LLVMGoLinkerRef LLVMGoCreateLinker(LLVMModuleRef Dest) {
  return reinterpret_cast<LLVMGoLinkerRef>(new llvm::Linker(*llvm::unwrap(Dest)));
}

LLVMBool LLVMGoLinkerLinkModule(LLVMGoLinkerRef L, LLVMModuleRef Src) {
  std::unique_ptr<llvm::Module> M(llvm::unwrap(Src));
  return reinterpret_cast<llvm::Linker *>(L)->linkInModule(std::move(M));
}

void LLVMGoDisposeLinker(LLVMGoLinkerRef L) {
  delete reinterpret_cast<llvm::Linker *>(L);
}

// LLVM 24 deprecates LLVMConstGEP2 and LLVMConstInBoundsGEP2. LLVM 19 adds
// LLVMConstGEPWithNoWrapFlags.
LLVMValueRef LLVMGoConstPtrAdd(LLVMValueRef Ptr, LLVMValueRef Offset,
                               unsigned NoWrapFlags) {
#if LLVM_VERSION_MAJOR >= 24
  return LLVMConstPtrAdd(Ptr, Offset, NoWrapFlags);
#else
  LLVMTypeRef I8 = LLVMInt8TypeInContext(LLVMGetTypeContext(LLVMTypeOf(Ptr)));
#if LLVM_VERSION_MAJOR >= 19
  return LLVMConstGEPWithNoWrapFlags(I8, Ptr, &Offset, 1, NoWrapFlags);
#else
  if (NoWrapFlags & 1)
    return LLVMConstInBoundsGEP2(I8, Ptr, &Offset, 1);
  return LLVMConstGEP2(I8, Ptr, &Offset, 1);
#endif
#endif
}
