#pragma once

#include "ast.hh"

class ASTVisitor {
public:
  virtual ~ASTVisitor() = default;
  void visit(TypeExpr *typeExpr);
  void visit(Var *Var);
  void visit(Expr *Expr);
  void visit(Stmt *Stmt);

protected:
  virtual void visitIntType(IntType *intType) = 0;
  virtual void visitTypeConst(TypeConst *typeConst) = 0;

  virtual void visitTypedVar(TypedVar *typedVar) = 0;
  virtual void visitUntypedVar(UntypedVar *untypedVar) = 0;

  virtual void visitNum(Num *num) = 0;
  virtual void visitBinOp(BinOp *binOp) = 0;
  virtual void visitVarUse(VarUse *varUse) = 0;

  virtual void visitSeq(Seq *seq) = 0;
  virtual void visitAssign(Assign *assign) = 0;
  virtual void visitPrint(Print *print) = 0;
};
