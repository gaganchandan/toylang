#include "interpreter.hh"
#include "ast.hh"
#include <iostream>
#include <stdexcept>

Interpreter::Interpreter() = default;

void Interpreter::visitProgram(Program *program) {
  for (auto &stmt : program->statements) {
    visit(stmt.get());
  }
}

void Interpreter::visitNum(Num *num) { currentExpr = num; }

void Interpreter::visitBool(Bool *boolExpr) { currentExpr = boolExpr; }

void Interpreter::visitVariantConstr(VariantConstr *variantConstr) {
  for (auto &arg : variantConstr->args) {
    visit(arg.get());
    arg = currentExpr->clone();
  }
  currentExpr = variantConstr;
}

void Interpreter::visitRecordVal(RecordVal *recordVal) {
  for (auto &field : recordVal->fields) {
    visit(field.second.get());
    field.second = currentExpr->clone();
  }
  currentExpr = recordVal;
}

void Interpreter::visitBinOp(BinOp *binOp) {
  visit(binOp->left.get());
  auto leftExpr = currentExpr;
  visit(binOp->right.get());
  auto rightExpr = currentExpr;

  switch (binOp->op) {
  case BinOpType::ADD:
    currentExpr = new Num(static_cast<Num *>(leftExpr)->value +
                          static_cast<Num *>(rightExpr)->value);
    break;
  case BinOpType::SUB:
    currentExpr = new Num(static_cast<Num *>(leftExpr)->value -
                          static_cast<Num *>(rightExpr)->value);
    break;
  case BinOpType::MUL:
    currentExpr = new Num(static_cast<Num *>(leftExpr)->value *
                          static_cast<Num *>(rightExpr)->value);
    break;
  case BinOpType::DIV:
    if (static_cast<Num *>(rightExpr)->value != 0) {
      currentExpr = new Num(static_cast<Num *>(leftExpr)->value /
                            static_cast<Num *>(rightExpr)->value);
    } else {
      throw std::runtime_error("Runtime error: Division by zero");
    }
    break;
  case BinOpType::AND:
    currentExpr = new Bool(static_cast<Bool *>(leftExpr)->value &&
                           static_cast<Bool *>(rightExpr)->value);
    break;
  case BinOpType::OR:
    currentExpr = new Bool(static_cast<Bool *>(leftExpr)->value ||
                           static_cast<Bool *>(rightExpr)->value);
    break;
  case BinOpType::EQ:
    switch (leftExpr->exprType) {
    case ExprType::NUM:
      currentExpr = new Bool(*static_cast<Num *>(leftExpr) ==
                             *static_cast<Num *>(rightExpr));

      break;
    case ExprType::BOOL:
      currentExpr = new Bool(*static_cast<Bool *>(leftExpr) ==
                             *static_cast<Bool *>(rightExpr));
      break;

    case ExprType::VARIANT_CONSTR:
      currentExpr = new Bool(*static_cast<VariantConstr *>(leftExpr) ==
                             *static_cast<VariantConstr *>(rightExpr));
      break;
    case ExprType::RECORD_VAL:
      currentExpr = new Bool(*static_cast<RecordVal *>(leftExpr) ==
                             *static_cast<RecordVal *>(rightExpr));
      break;
    default:
      throw std::runtime_error(
          "Runtime error: Unsupported type for EQ operation");
    }
    break;

  case BinOpType::LT:
    currentExpr = new Bool(static_cast<Num *>(leftExpr)->value <
                           static_cast<Num *>(rightExpr)->value);
    break;
  case BinOpType::GT:
    currentExpr = new Bool(static_cast<Num *>(leftExpr)->value >
                           static_cast<Num *>(rightExpr)->value);
    break;

  default:
    throw std::runtime_error("Runtime error: Unknown binary operator");
  }
}

void Interpreter::visitUnOp(UnOp *unOp) {
  visit(unOp->expr.get());
  auto expr = currentExpr;

  switch (unOp->op) {
  case UnOpType::NOT:
    currentExpr = new Bool(!static_cast<Bool *>(expr)->value);
    break;
  default:
    throw std::runtime_error("Runtime error: Unknown unary operator");
  }
}

void Interpreter::visitVarUse(VarUse *varUse) {
  currentExpr = valEnv.get(varUse->name);
  if (currentExpr == nullptr) {
    throw std::runtime_error("Runtime error: Undefined variable: " +
                             varUse->name);
  }
}

void Interpreter::visitDecl(Decl *decl) {}

void Interpreter::visitVariant(Variant *variant) {}

void Interpreter::visitRecord(Record *record) {}

void Interpreter::visitAssign(Assign *assign) {
  visit(assign->right.get());
  switch (currentExpr->exprType) {
  case ExprType::VARIANT_CONSTR: {
    auto x = static_cast<VariantConstr *>(currentExpr);
    break;
  }
  default:
    break;
  }
  Expr *rightValue = currentExpr;
  valEnv.put(assign->left, rightValue);
}

void Interpreter::visitIf(If *ifStmt) {
  visit(ifStmt->condition.get());
  Expr *conditionExpr = currentExpr;
  if (conditionExpr->exprType != ExprType::BOOL) {
    throw std::runtime_error("Runtime error: Condition is not boolean");
  }
  if (static_cast<Bool *>(conditionExpr)->value) {
    for (const auto &stmt : ifStmt->thenStmts) {
      visit(stmt.get());
    }
  }
}

void Interpreter::visitIfElse(IfElse *ifElseStmt) {
  visit(ifElseStmt->condition.get());
  Expr *conditionExpr = currentExpr;
  if (conditionExpr->exprType != ExprType::BOOL) {
    throw std::runtime_error("Runtime error: Condition is not boolean");
  }
  if (static_cast<Bool *>(conditionExpr)->value) {
    for (const auto &stmt : ifElseStmt->thenStmts) {
      visit(stmt.get());
    }
  } else {
    for (const auto &stmt : ifElseStmt->elseStmts) {
      visit(stmt.get());
    }
  }
}

// Print using Expr::toString()
void Interpreter::visitPrint(Print *print) {
  visit(print->expr.get());
  Expr *exprToPrint = currentExpr;
  std::cout << exprToPrint->toString() << std::endl;
}
