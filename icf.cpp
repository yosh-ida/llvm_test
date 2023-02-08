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
#include <iostream>

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
        printf("Function: %s\n", F.getName().data());
        for (auto &BB : F) {
            const llvm::Instruction *inst = icft->getFirstICFI(&BB);
            if (inst != nullptr) {
                BB.print(*ost); 
                printf("\t%s\n", inst->getOpcodeName());
            }
            // if (icflsi->blockMayThrow(&BB)) {
            //     BB.print(*ost);
            //     printf("\n");
            // }
        }
    }

  return 0;
}