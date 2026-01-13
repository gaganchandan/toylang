#include "printvisitor.hh"
#include <iostream>

PrintVisitor::PrintVisitor() : binOpStrings({"+", "-", "*", "/"}) {}

void PrintVisitor::visitIntType(IntType *intType) { std::cout << "int"; }

void PrintVisitor::visitTypeConst(TypeConst *typeConst) {
  std::cout << typeConst->typeName;
}

void PrintVisitor::visitTypedVar(TypedVar *typedVar) {
  visit(typedVar->typeExpr.get());
  std::cout << " " << typedVar->name;
}

void PrintVisitor::visitUntypedVar(UntypedVar *untypedVar) {
  std::cout << untypedVar->name;
}

void PrintVisitor::visitNum(Num *num) { std::cout << num->value; }

void PrintVisitor::visitBinOp(BinOp *binOp) {
  visit(binOp->left.get());
  std::cout << " " << binOpStrings[static_cast<int>(binOp->op)] << " ";
  visit(binOp->right.get());
}

void PrintVisitor::visitVarUse(VarUse *varUse) { std::cout << varUse->name; }

void PrintVisitor::visitSeq(Seq *seq) {
  visit(seq->first.get());
  std::cout << ";\n";
  visit(seq->second.get());
}

void PrintVisitor::visitAssign(Assign *assign) {
  visit(assign->left.get());
  std::cout << " = ";
  visit(assign->right.get());
}

void PrintVisitor::visitPrint(Print *print) {
  std::cout << "print(";
  visit(print->expr.get());
  std::cout << ")";
}
