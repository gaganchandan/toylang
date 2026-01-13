#include "astvisitor.hh"

void ASTVisitor::visit(TypeExpr *typeExpr) {
  switch (typeExpr->typeExprType) {
  case TypeExprType::INT_TYPE:
    visitIntType(static_cast<IntType *>(typeExpr));
    break;
  case TypeExprType::TYPECONST:
    visitTypeConst(static_cast<TypeConst *>(typeExpr));
    break;
  }
}

void ASTVisitor::visit(Var *var) {
  switch (var->varType) {
  case VarType::TYPED_VAR:
    visitTypedVar(static_cast<TypedVar *>(var));
    break;
  case VarType::UNTYPED_VAR:
    visitUntypedVar(static_cast<UntypedVar *>(var));
    break;
  }
}

void ASTVisitor::visit(Expr *expr) {
  switch (expr->exprType) {
  case ExprType::NUM:
    visitNum(static_cast<Num *>(expr));
    break;
  case ExprType::BINOP:
    visitBinOp(static_cast<BinOp *>(expr));
    break;
  case ExprType::VAR_USE:
    visitVarUse(static_cast<VarUse *>(expr));
    break;
  }
}

void ASTVisitor::visit(Stmt *stmt) {
  switch (stmt->stmtType) {
  case StmtType::SEQ:
    visitSeq(static_cast<Seq *>(stmt));
    break;
  case StmtType::ASSIGN:
    visitAssign(static_cast<Assign *>(stmt));
    break;
  case StmtType::PRINT:
    visitPrint(static_cast<Print *>(stmt));
    break;
  }
}
