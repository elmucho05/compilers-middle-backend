//===-- LocalOpts.cpp - Example Transformations --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Utils/LocalOpts.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
// L'include seguente va in LocalOpts.h
#include <llvm/IR/Constants.h>
#include "llvm/ADT/APInt.h"

using namespace llvm;

bool runOnBasicBlock(BasicBlock &B) {
  for(auto &I : B){
    if(I.getOpcode() == Instruction::Mul){
      outs() << "Trovata una moltiplicazione" << I << "\n";

      ConstantInt *constPotenzaDue = nullptr;
      bool scambia = false;
      //Controllo subito il secondo operando
      if(ConstantInt *secondConst = dyn_cast<ConstantInt>(I.getOperand(1))){
        if(secondConst->getValue().isPowerOf2()){
          constPotenzaDue = secondConst;
        }
      }
      //se il secondo operando nn è una costante, controllo il primo.
      if(constPotenzaDue==nullptr){
        if(ConstantInt *firstConst = dyn_cast<ConstantInt>(I.getOperand(0))){
          if(firstConst->getValue().isPowerOf2()){
            constPotenzaDue = firstConst;
            scambia=true;
          }
        }
      }
 // Check that at least one of the operands is a constant power of 2
    if (constPotenzaDue != nullptr) {
        ConstantInt *shiftConst = ConstantInt::get(constPotenzaDue->getType(), constPotenzaDue->getValue().exactLogBase2());
        outs() << "Costante : " << constPotenzaDue->getValue() << "\n";

        Instruction *NewInst;

        // If the first operand is the power of 2, we need to swap the operands
        if (scambia) {
          NewInst = BinaryOperator::Create(Instruction::Shl, I.getOperand(1), shiftConst);
        } else {
          NewInst = BinaryOperator::Create(Instruction::Shl, I.getOperand(0), shiftConst);
        }

        NewInst->insertAfter(&I);
        I.replaceAllUsesWith(NewInst);
    } else {
        outs() << "No power of 2 constant found\n";
    }
    }


  }
    // // Preleviamo le prime due istruzioni del BB
    // Instruction &Inst1st = *B.begin(), &Inst2nd = *(++B.begin());

    // // L'indirizzo della prima istruzione deve essere uguale a quello del 
    // // primo operando della seconda istruzione (per costruzione dell'esempio)
    // assert(&Inst1st == Inst2nd.getOperand(0));

    // // Stampa la prima istruzione
    // outs() << "PRIMA ISTRUZIONE: " << Inst1st << "\n";
    // // Stampa la prima istruzione come operando
    // outs() << "COME OPERANDO: ";
    // Inst1st.printAsOperand(outs(), false);
    // outs() << "\n";

    // // User-->Use-->Value
    // outs() << "I MIEI OPERANDI SONO:\n";
    // for (auto *Iter = Inst1st.op_begin(); Iter != Inst1st.op_end(); ++Iter) {
    //   Value *Operand = *Iter;

    //   if (Argument *Arg = dyn_cast<Argument>(Operand)) {
    //     outs() << "\t" << *Arg << ": SONO L'ARGOMENTO N. " << Arg->getArgNo() 
	  //      <<" DELLA FUNZIONE " << Arg->getParent()->getName()
    //            << "\n";
    //   }
    //   if (ConstantInt *C = dyn_cast<ConstantInt>(Operand)) {
    //     outs() << "\t" << *C << ": SONO UNA COSTANTE INTERA DI VALORE " << C->getValue()
    //            << "\n";
    //   }
    // }

    // outs() << "LA LISTA DEI MIEI USERS:\n";
    // for (auto Iter = Inst1st.user_begin(); Iter != Inst1st.user_end(); ++Iter) {
    //   outs() << "\t" << *(dyn_cast<Instruction>(*Iter)) << "\n";
    // }

    // outs() << "E DEI MIEI USI (CHE E' LA STESSA):\n";
    // for (auto Iter = Inst1st.use_begin(); Iter != Inst1st.use_end(); ++Iter) {
    //   outs() << "\t" << *(dyn_cast<Instruction>(Iter->getUser())) << "\n";
    // }

    // // Manipolazione delle istruzioni
    // Instruction *NewInst = BinaryOperator::Create(
    //     Instruction::Add, Inst1st.getOperand(0), Inst1st.getOperand(0));

    // NewInst->insertAfter(&Inst1st);
    // // Si possono aggiornare le singole references separatamente?
    // // Controlla la documentazione e prova a rispondere.
    // Inst1st.replaceAllUsesWith(NewInst);
    // APInt myapint;
    // int mulcounter = 0;
    // for (auto &Inst : B){
    //   if(Inst.getOpcode() == Instruction::Mul){
    //     mulcounter++;
    //     outs() << "I MIEI OPERANDI SONO:\n";
    //     for (auto *Iter = Inst.op_begin(); Iter != Inst.op_end(); ++Iter) {
    //       Value *Operand = *Iter;
    //       if (Instruction *I = dyn_cast<Instruction>(Operand)){
    //         outs() << "\t" << *I << ": SONO L'ISTRUZIONE N. "<< I->getParent()->getName()
    //               << "\n";
    //       }
    //       if (Argument *Arg = dyn_cast<Argument>(Operand)) {
    //         outs() << "\t" << *Arg << ": SONO L'ARGOMENTO N. " << Arg->getArgNo() 
    //         <<" DELLA FUNZIONE " << Arg->getParent()->getName()
    //               << "\n";
    //       }
    //       if (ConstantInt *C = dyn_cast<ConstantInt>(Operand)) {
    //         outs() << "\t" << *C << ": SONO UNA COSTANTE INTERA DI VALORE " << C->getValue()
    //               << "\n";
    //         myapint = C->getValue();
    //         if(myapint.isPowerOf2()){
    //           outs() << "\t" << *C << ": sono una costante intera potenza di due";
    //         // Manipolazione delle istruzioni
    //           Instruction *NewInst = BinaryOperator::Create(
    //           Instruction::Shl, Inst.getOperand(0), C.getValue().exactLogBase2());
    //               NewInst->insertAfter(&Inst1st);
    // // Si possono aggiornare le singole references separatamente?
    // // Controlla la documentazione e prova a rispondere.
    // Inst1st.replaceAllUsesWith(NewInst);
    //         }

    //        }
    //     }
    //   }
        
    // }

    // errs() << "Numero mul : "<< mulcounter << "\n";

    return true;
  }


bool runOnFunction(Function &F) {
  bool Transformed = false;

  for (auto Iter = F.begin(); Iter != F.end(); ++Iter) {
    if (runOnBasicBlock(*Iter)) {
      Transformed = true;
    }
  }

  return Transformed;
}


PreservedAnalyses LocalOpts::run(Module &M,
                                      ModuleAnalysisManager &AM) {
  for (auto Fiter = M.begin(); Fiter != M.end(); ++Fiter)
    if (runOnFunction(*Fiter))
      return PreservedAnalyses::none();
  
  return PreservedAnalyses::all();
}

