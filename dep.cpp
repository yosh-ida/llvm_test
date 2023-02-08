#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/Analysis/InstructionPrecedenceTracking.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Analysis/MemoryDependenceAnalysis.h>
#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/Analysis/AssumptionCache.h>
#include <llvm/IR/Dominators.h>
#include <llvm/Analysis/PhiValues.h>
#include <llvm/IR/Use.h>
#include <llvm/Support/raw_ostream.h>
#include <iostream>
#include <string>

std::string getNameOrAsOperand(llvm::Value *value) {
  if (!value->getName().empty())
    return std::string(value->getName());
 
  std::string BBName;
  llvm::raw_string_ostream OS(BBName);
  value->printAsOperand(OS, false);
  return OS.str();
}

int main(int argc, char** argv) {
    llvm::LLVMContext context;
    llvm::SMDiagnostic err;
    auto M = llvm::parseIRFile("./sample.ll", err, context);

    if (!M) {
        err.print(argv[0], llvm::errs());
        return 1;
    }

    auto icft = new llvm::ImplicitControlFlowTracking();
    auto ost = new llvm::raw_os_ostream(std::cout);
    for (auto &F : *M) {
        llvm::AssumptionCache AC(F);
        llvm::PhiValues PV(F);
        llvm::DominatorTree DT(F);
        llvm::TargetLibraryInfoImpl TLII;
        llvm::TargetLibraryInfo TLI(TLII, &F);
        llvm::AAResults AA(TLI);
        llvm::MemoryDependenceResults MDR(AA, AC, TLI, DT, PV, 100);

        printf("Function: %s\n", F.getName().data());
        for (auto &BB : F) {
            for (auto &Inst : BB) {
                llvm::MemDepResult dep = MDR.getDependency(&Inst);
                llvm::Instruction *depinst = dep.getInst();
                if (depinst) {
                    printf("%s", Inst.getOpcodeName());
                    for (unsigned int i = 0; i < Inst.getNumOperands(); i++)
                        printf(" %s", getNameOrAsOperand(Inst.getOperand(i)).c_str());
                    printf("\tdepends on\t%s", depinst->getOpcodeName());
                    for (unsigned int i = 0; i < depinst->getNumOperands(); i++)
                        printf(" %s", getNameOrAsOperand(depinst->getOperand(i)).c_str());
                    printf("\n");
                }
            }
        }
    }

  return 0;
}