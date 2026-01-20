#include "ast.hh"
#include "interpreter.hh"
#include "printer.hh"
#include "typechecker.hh"
#include <iostream>
#include <map>

int main() {

  std::map<std::string, std::vector<std::unique_ptr<TypeExpr>>> varMap;
  std::vector<std::unique_ptr<TypeExpr>> args1;
  std::vector<std::unique_ptr<TypeExpr>> args2;
  args2.push_back(std::make_unique<IntType>());
  varMap.emplace("Zero", std::move(args1));
  varMap.emplace("NonZero", std::move(args2));
  auto variant1 = std::make_unique<Variant>("Num", std::move(varMap));
  auto declV1 = std::make_unique<Decl>(std::move("v1"),
                                       std::make_unique<VariantType>("Num"));
  auto declV2 = std::make_unique<Decl>(std::move("v2"),
                                       std::make_unique<VariantType>("Num"));
  auto decl1 =
      std::make_unique<Decl>(std::move("x"), std::make_unique<IntType>());
  auto decl2 =
      std::make_unique<Decl>(std::move("y"), std::make_unique<IntType>());
  auto numExpr = std::make_unique<Num>(42);
  auto stmt1 = std::make_unique<Assign>(std::move("x"), std::move(numExpr));
  auto numExprY = std::make_unique<Num>(10);
  auto stmt2 = std::make_unique<Assign>(std::move("y"), std::move(numExprY));
  // v1 = Num(x + y); v2 = Num(42);
  std::vector<std::unique_ptr<Expr>> variantConstrArgs1 =
      std::vector<std::unique_ptr<Expr>>();
  variantConstrArgs1.push_back(
      std::make_unique<BinOp>(std::make_unique<VarUse>("x"), BinOpType::ADD,
                              std::make_unique<VarUse>("y")));
  auto variantConstr1 =
      std::make_unique<VariantConstr>("NonZero", std::move(variantConstrArgs1));
  auto stmt3 =
      std::make_unique<Assign>(std::move("v1"), std::move(variantConstr1));
  // create int x = 52 and assign to v2
  auto decl3 =
      std::make_unique<Decl>(std::move("z"), std::make_unique<IntType>());
  auto stmtZ =
      std::make_unique<Assign>(std::move("z"), std::make_unique<Num>(52));
  std::vector<std::unique_ptr<Expr>> variantConstrArgs2 =
      std::vector<std::unique_ptr<Expr>>();
  // variantConstrArgs2.push_back(std::move(std::make_unique<VarUse>("z")));
  variantConstrArgs2.push_back(std::move(std::make_unique<Num>(52)));
  auto variantConstr2 =
      std::make_unique<VariantConstr>("NonZero", std::move(variantConstrArgs2));
  auto stmt4 =
      std::make_unique<Assign>(std::move("v2"), std::move(variantConstr2));
  auto eqExpr =
      std::make_unique<BinOp>(std::make_unique<VarUse>("v1"), BinOpType::EQ,
                              std::make_unique<VarUse>("v2"));
  auto printStmt = std::make_unique<Print>(std::move(eqExpr));

  auto thenStmts = std::vector<std::unique_ptr<Stmt>>();
  std::vector<std::unique_ptr<Stmt>> statements;
  statements.push_back(std::move(decl1));
  statements.push_back(std::move(decl2));
  statements.push_back(std::move(decl3));
  statements.push_back(std::move(variant1));
  statements.push_back(std::move(declV1));
  statements.push_back(std::move(declV2));
  statements.push_back(std::move(stmtZ));
  statements.push_back(std::move(stmt1));
  statements.push_back(std::move(stmt2));
  statements.push_back(std::move(stmt3));
  statements.push_back(std::move(stmt4));
  statements.push_back(std::move(printStmt));
  Program program(std::move(statements));
  std::cout << "---- Program ----" << std::endl;
  Printer printer;
  printer.visitProgram(&program);
  std::cout << "-----------------" << std::endl;
  std::cout << "---- Type Checking ----" << std::endl;
  TypeChecker typeChecker;
  typeChecker.visitProgram(&program);
  std::cout << "Type checking completed successfully." << std::endl;
  std::cout << "------------------------" << std::endl;
  std::cout << "---- Interpretation ----" << std::endl;
  Interpreter interpreter;
  interpreter.visitProgram(&program);
  std::cout << "------------------------" << std::endl;
}
