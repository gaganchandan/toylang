#include "ast.hh"

// Type Expressions
TypeExpr::TypeExpr(TypeExprType type) : typeExprType(type) {}

IntType::IntType() : TypeExpr(TypeExprType::INT_TYPE) {}

std::string IntType::toString() const { return "int"; }

TypeConst::TypeConst(std::string typeName)
    : TypeExpr(TypeExprType::TYPECONST), typeName(std::move(typeName)) {}

std::string TypeConst::toString() const { return typeName; }

// Variables
Var::Var(VarType type, std::string varName)
    : varType(type), name(std::move(varName)) {}

TypedVar::TypedVar(std::string varName, std::unique_ptr<TypeExpr> typeExpr)
    : Var(VarType::TYPED_VAR, std::move(varName)),
      typeExpr(std::move(typeExpr)) {}

std::string TypedVar::toString() const {
  return typeExpr->toString() + " " + name;
}

UntypedVar::UntypedVar(std::string varName)
    : Var(VarType::UNTYPED_VAR, std::move(varName)) {}

std::string UntypedVar::toString() const { return name; }

// Expressions
Expr::Expr(ExprType type) : exprType(type) {}

Num::Num(int val) : Expr(ExprType::NUM), value(val) {}

BinOp::BinOp(std::unique_ptr<Expr> left, BinOpType op,
             std::unique_ptr<Expr> right)
    : Expr(ExprType::BINOP), left(std::move(left)), op(op),
      right(std::move(right)) {}

VarUse::VarUse(std::string varName)
    : Expr(ExprType::VAR_USE), name(std::move(varName)) {}

// Statements
Stmt::Stmt(StmtType type) : stmtType(type) {}

Seq::Seq(std::unique_ptr<Stmt> first, std::unique_ptr<Stmt> second)
    : Stmt(StmtType::SEQ), first(std::move(first)), second(std::move(second)) {}

Assign::Assign(std::unique_ptr<Var> left, std::unique_ptr<Expr> right)
    : Stmt(StmtType::ASSIGN), left(std::move(left)), right(std::move(right)) {}

Print::Print(std::unique_ptr<Expr> expr)
    : Stmt(StmtType::PRINT), expr(std::move(expr)) {}
