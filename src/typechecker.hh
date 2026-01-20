#pragma once

#include "env.hh"
#include "visitor.hh"

class TypeChecker : public Visitor {
private:
  TypeEnvironment typeEnv;
  TypeExpr *currentTypeExpr;

protected:
  void visitNum(Num *num) override;
  void visitBool(Bool *boolExpr) override;
  void visitVariantConstr(VariantConstr *variantConstr) override;
  void visitBinOp(BinOp *binOp) override;
  void visitUnOp(UnOp *unOp) override;
  void visitVarUse(VarUse *varUse) override;

  void visitDecl(Decl *decl) override;
  void visitVariant(Variant *variant) override;
  void visitAssign(Assign *assign) override;
  void visitIf(If *ifStmt) override;
  void visitIfElse(IfElse *ifElseStmt) override;
  void visitMatch(Match *match) override;
  void visitPrint(Print *print) override;

public:
  TypeChecker();
  void visitProgram(Program *program) override;
};
