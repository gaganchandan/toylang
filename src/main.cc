#include "ast.hh"
#include "printvisitor.hh"

int main() {
  auto typedVar = std::make_unique<TypedVar>("x", std::make_unique<IntType>());
  auto numExpr = std::make_unique<Num>(42);
  auto assignStmt =
      std::make_unique<Assign>(std::move(typedVar), std::move(numExpr));

  PrintVisitor printVisitor;
  printVisitor.visit(assignStmt.get());
}
