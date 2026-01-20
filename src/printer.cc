#include "printer.hh"
#include <iostream>

Printer::Printer()
    : binOpStrings({"+", "-", "*", "/", "&&", "||", "==", "<", ">"}),
      unOpStrings({"!"}) {}

void Printer::visitNum(Num *num) { std::cout << num->value; }

void Printer::visitBool(Bool *boolExpr) {
  std::cout << (boolExpr->value ? "true" : "false");
}

void Printer::visitVariantConstr(VariantConstr *variantConstr) {
  std::cout << variantConstr->constr << "(";
  for (size_t i = 0; i < variantConstr->args.size(); ++i) {
    visit(variantConstr->args[i].get());
    if (i != variantConstr->args.size() - 1) {
      std::cout << ", ";
    }
  }
  std::cout << ")";
}

void Printer::visitBinOp(BinOp *binOp) {
  visit(binOp->left.get());
  std::cout << " " << binOpStrings[static_cast<int>(binOp->op)] << " ";
  visit(binOp->right.get());
}

void Printer::visitUnOp(UnOp *unOp) {
  std::cout << unOpStrings[static_cast<int>(unOp->op)];
  visit(unOp->expr.get());
}

void Printer::visitVarUse(VarUse *varUse) { std::cout << varUse->name; }

void Printer::visitDecl(Decl *decl) {
  std::cout << decl->type->toString() << " " << decl->name << ";";
}

void Printer::visitVariant(Variant *variant) {
  std::cout << variant->name << " = " << std::endl;
  for (auto &constr : variant->constructors) {
    std::cout << "| " << constr.first << "(";
    for (auto &typeExpr : constr.second) {
      std::cout << typeExpr->toString();
      if (&typeExpr != &constr.second.back()) {
        std::cout << ", ";
      }
    }
    std::cout << ")";
  }
}

// void Printer::visitVariant(Variant *variant) {}

void Printer::visitAssign(Assign *assign) {
  std::cout << assign->left << " = ";
  visit(assign->right.get());
  std::cout << ";";
}

void Printer::visitIf(If *ifStmt) {
  std::cout << "if (";
  visit(ifStmt->condition.get());
  std::cout << ") {" << std::endl;
  for (const auto &stmt : ifStmt->thenStmts) {
    std::cout << "  ";
    visit(stmt.get());
    std::cout << std::endl;
  }
  std::cout << "}";
}

void Printer::visitIfElse(IfElse *ifElseStmt) {
  std::cout << "if (";
  visit(ifElseStmt->condition.get());
  std::cout << ") {" << std::endl;
  for (const auto &stmt : ifElseStmt->thenStmts) {
    std::cout << "  ";
    visit(stmt.get());
    std::cout << std::endl;
  }
  std::cout << "} else {" << std::endl;
  for (const auto &stmt : ifElseStmt->elseStmts) {
    visit(stmt.get());
    std::cout << std::endl;
  }
  std::cout << "}";
}

void Printer::visitPrint(Print *print) {
  std::cout << "print(";
  visit(print->expr.get());
  std::cout << ");";
}

void Printer::visitProgram(Program *program) {
  for (auto &stmt : program->statements) {
    visit(stmt.get());
    std::cout << std::endl << std::endl;
  }
}
