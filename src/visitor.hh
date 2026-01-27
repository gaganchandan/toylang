#pragma once

#include "ast.hh"
#include <any>

class Visitor {
public:
  virtual ~Visitor() = default;
  void visit(Expr *Expr);
  void visit(Stmt *Stmt);
  virtual void visitProgram(Program *program) = 0;

protected:
  virtual void visitNum(Num *num) = 0;
  virtual void visitBool(Bool *boolExpr) = 0;
  virtual void visitVariantConstr(VariantConstr *variantConstr) = 0;
  virtual void visitRecordVal(RecordVal *recordVal) = 0;
  virtual void visitFieldAccess(FieldAccess *recordAccess) = 0;
  virtual void visitBinOp(BinOp *binOp) = 0;
  virtual void visitUnOp(UnOp *unOp) = 0;
  virtual void visitVarUse(VarUse *varUse) = 0;

  virtual void visitDecl(Decl *decl) = 0;
  virtual void visitVariant(Variant *variant) = 0;
  virtual void visitRecord(Record *record) = 0;
  virtual void visitAssign(Assign *assign) = 0;
  virtual void visitIf(If *ifStmt) = 0;
  virtual void visitIfElse(IfElse *ifElseStmt) = 0;
  virtual void visitPrint(Print *print) = 0;
};
