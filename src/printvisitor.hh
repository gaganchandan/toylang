#pragma once

#include "astvisitor.hh"
#include <vector>

class PrintVisitor : public ASTVisitor {
private:
  const std::vector<std::string> binOpStrings;

protected:
  void visitIntType(IntType *intType) override;
  void visitTypeConst(TypeConst *typeConst) override;

  void visitTypedVar(TypedVar *typedVar) override;
  void visitUntypedVar(UntypedVar *untypedVar) override;

  void visitNum(Num *num) override;
  void visitBinOp(BinOp *binOp) override;
  void visitVarUse(VarUse *varUse) override;

  void visitSeq(Seq *seq) override;
  void visitAssign(Assign *assign) override;
  void visitPrint(Print *print) override;

public:
  PrintVisitor();
};
