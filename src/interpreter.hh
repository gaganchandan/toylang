#pragma once

#include "env.hh"
#include "visitor.hh"

class Interpreter : public Visitor {
private:
  ValueEnvironment valEnv;
  Expr *currentExpr;

protected:
  void visitNum(Num *num) override;
  void visitBool(Bool *boolExpr) override;
  void visitVariantConstr(VariantConstr *variantConstr) override;
  void visitRecordVal(RecordVal *recordVal) override;
  void visitBinOp(BinOp *binOp) override;
  void visitUnOp(UnOp *unOp) override;
  void visitVarUse(VarUse *varUse) override;

  void visitDecl(Decl *decl) override;
  void visitVariant(Variant *variant) override;
  void visitRecord(Record *record) override;
  void visitAssign(Assign *assign) override;
  void visitIf(If *ifStmt) override;
  void visitIfElse(IfElse *ifElseStmt) override;
  void visitPrint(Print *print) override;

public:
  Interpreter();
  void visitProgram(Program *program) override;
};
