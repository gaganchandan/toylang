#include "typechecker.hh"
#include "ast.hh"
#include "env.hh"
#include <iostream>
#include <stdexcept>

TypeChecker::TypeChecker() = default;

void TypeChecker::visitProgram(Program *program) {
  for (auto &stmt : program->statements) {
    visit(stmt.get());
  }
}

void TypeChecker::visitNum(Num *num) { currentTypeExpr = new IntType(); }

void TypeChecker::visitBool(Bool *boolExpr) {
  currentTypeExpr = new BoolType();
}

void TypeChecker::visitVariantConstr(VariantConstr *variantConstr) {
  std::vector<std::unique_ptr<TypeExpr>> typeExprs;
  for (auto &arg : variantConstr->args) {
    visit(&*arg);
    typeExprs.push_back(currentTypeExpr->clone());
  }
  // VariantConstrType *variantConstrType =
  //     new VariantConstrType(variantConstr->constr, std::move(typeExprs));

  VariantConstrType variantConstrType(variantConstr->constr,
                                      std::move(typeExprs));
  currentTypeExpr = typeEnv.getConstrType(std::move(variantConstrType));
  if (currentTypeExpr == nullptr) {
    throw std::runtime_error("Undefined variant constructor: " +
                             variantConstr->constr);
  }
}

void TypeChecker::visitBinOp(BinOp *binOp) {
  visit(binOp->left.get());
  auto leftType = currentTypeExpr;
  visit(binOp->right.get());
  auto rightType = currentTypeExpr;
  switch (binOp->op) {
  case BinOpType::ADD:
  case BinOpType::SUB:
  case BinOpType::MUL:
  case BinOpType::DIV:
    if (leftType->typeExprType != TypeExprType::INT_TYPE ||
        rightType->typeExprType != TypeExprType::INT_TYPE) {
      throw std::runtime_error("Type error in arithmetic operation");
    }
    currentTypeExpr = leftType;
    break;
  case BinOpType::LT:
  case BinOpType::GT:
    if (leftType->typeExprType != TypeExprType::INT_TYPE ||
        rightType->typeExprType != TypeExprType::INT_TYPE) {
      throw std::runtime_error("Type error in comparison operation");
    }
    currentTypeExpr = new BoolType();
    break;
  case BinOpType::AND:
  case BinOpType::OR:
    if (leftType->typeExprType != TypeExprType::BOOL_TYPE ||
        rightType->typeExprType != TypeExprType::BOOL_TYPE) {
      throw std::runtime_error("Type error in logical operation");
    }
    currentTypeExpr = leftType;
    break;
  case BinOpType::EQ:
    if (!(*leftType == *rightType)) {
      throw std::runtime_error("Type error in equality operation");
    }
    currentTypeExpr = new BoolType();
    break;
  }
}

void TypeChecker::visitUnOp(UnOp *unOp) {
  visit(unOp->expr.get());
  auto exprType = currentTypeExpr;
  switch (unOp->op) {
  case UnOpType::NOT:
    if (exprType->typeExprType != TypeExprType::BOOL_TYPE) {
      throw std::runtime_error("Type error in NOT operation");
    }
    currentTypeExpr = exprType;
    break;
  }
}

void TypeChecker::visitVarUse(VarUse *varUse) {
  currentTypeExpr = typeEnv.get(varUse->name);
  if (currentTypeExpr == nullptr) {
    throw std::runtime_error("Undefined variable: " + varUse->name);
  }
}

void TypeChecker::visitDecl(Decl *decl) {
  currentTypeExpr = typeEnv.get(decl->name);
  if (currentTypeExpr == nullptr) {
    typeEnv.put(decl->name, decl->type->clone().release());
    currentTypeExpr = decl->type->clone().release();
  } else {
    throw std::runtime_error("Cannot redefine variable: " + decl->name);
  }
}

void TypeChecker::visitVariant(Variant *variant) {
  if (typeEnv.checkVariant(variant->name)) {
    throw std::runtime_error("Cannot redefine variant: " + variant->name);
  }
  for (auto &constr : variant->constructors) {
    auto variantConstrType = std::make_unique<VariantConstrType>(
        std::move(constr.first), std::move(constr.second));
    auto variantType = std::make_unique<VariantType>(variant->name);
    typeEnv.newVariant(std::move(*variantConstrType), std::move(*variantType));
  }
}

void TypeChecker::visitAssign(Assign *assign) {
  visit(assign->right.get());
  auto rightType = currentTypeExpr;
  auto leftType = typeEnv.get(assign->left);
  if (leftType == nullptr) {
    throw std::runtime_error("Undefined variable: " + assign->left);
  }
  if (!(*leftType == *rightType)) {
    throw std::runtime_error("Type error in assignment to variable: " +
                             assign->left);
  }
}

void TypeChecker::visitIf(If *ifStmt) {
  visit(ifStmt->condition.get());
  auto condType = currentTypeExpr;
  if (condType->typeExprType != TypeExprType::BOOL_TYPE) {
    throw std::runtime_error("Condition in if statement must be of type bool");
  }
  for (const auto &stmt : ifStmt->thenStmts) {
    visit(stmt.get());
  }
}

void TypeChecker::visitIfElse(IfElse *ifElseStmt) {
  visit(ifElseStmt->condition.get());
  auto condType = currentTypeExpr;
  if (condType->typeExprType != TypeExprType::BOOL_TYPE) {
    throw std::runtime_error(
        "Condition in if-else statement must be of type bool");
  }
  for (const auto &stmt : ifElseStmt->thenStmts) {
    visit(stmt.get());
  }
  for (const auto &stmt : ifElseStmt->elseStmts) {
    visit(stmt.get());
  }
}

void TypeChecker::visitMatch(Match *match) {
  visit(match->expr.get());
  auto exprType = currentTypeExpr;
  if (exprType->typeExprType != TypeExprType::VARIANT_TYPE) {
    throw std::runtime_error("Match expression must be of variant type");
  }
  for (const auto &casePair : match->cases) {
    auto localTypeEnv = new TypeEnvironment();
    for (auto &decl : casePair.second) {
    }
    // Check if key with ->constr == casePair.first->constr exists in typeEnv
    for (const auto &stmt : casePair.second) {
      visit(stmt.get());
    }
  }
}

void TypeChecker::visitPrint(Print *print) { visit(print->expr.get()); }
