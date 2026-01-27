#include "printer.hh"
#include <iostream>

Printer::Printer() = default;

void Printer::visitNum(Num *num) { std::cout << num->toString(); }

void Printer::visitBool(Bool *boolExpr) { std::cout << boolExpr->toString(); }

void Printer::visitVariantConstr(VariantConstr *variantConstr) {
  std::cout << variantConstr->toString();
}

void Printer::visitRecordVal(RecordVal *recordVal) {
  std::cout << recordVal->toString();
}

void Printer::visitFieldAccess(FieldAccess *recordAccess) {
  std::cout << recordAccess->toString();
}

void Printer::visitBinOp(BinOp *binOp) { std::cout << binOp->toString(); }

void Printer::visitUnOp(UnOp *unOp) { std::cout << unOp->toString(); }

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

void Printer::visitRecord(Record *record) {
  std::cout << "record " << record->name << " {" << std::endl;
  for (auto &field : record->fields) {
    std::cout << "  " << field.second->toString() << " " << field.first << ";"
              << std::endl;
  }
  std::cout << "};";
}

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
