#include "visitor.hh"

void Visitor::visit(Expr *expr) {
  switch (expr->exprType) {
  case ExprType::NUM:
    visitNum(static_cast<Num *>(expr));
    break;
  case ExprType::BOOL:
    visitBool(static_cast<Bool *>(expr));
    break;
  case ExprType::VARIANT_CONSTR:
    visitVariantConstr(static_cast<VariantConstr *>(expr));
    break;
  case ExprType::BINOP:
    visitBinOp(static_cast<BinOp *>(expr));
    break;
  case ExprType::UNOP:
    visitUnOp(static_cast<UnOp *>(expr));
    break;
  case ExprType::VAR_USE:
    visitVarUse(static_cast<VarUse *>(expr));
    break;
  }
}

void Visitor::visit(Stmt *stmt) {
  switch (stmt->stmtType) {
  case StmtType::DECL:
    visitDecl(static_cast<Decl *>(stmt));
    break;
  case StmtType::VARIANT:
    visitVariant(static_cast<Variant *>(stmt));
    break;
  case StmtType::ASSIGN:
    visitAssign(static_cast<Assign *>(stmt));
    break;
  case StmtType::IF:
    visitIf(static_cast<If *>(stmt));
    break;
  case StmtType::IF_ELSE:
    visitIfElse(static_cast<IfElse *>(stmt));
    break;
  case StmtType::PRINT:
    visitPrint(static_cast<Print *>(stmt));
    break;
  }
}
