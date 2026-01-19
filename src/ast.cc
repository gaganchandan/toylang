#include "ast.hh"
#include <iostream>

// Type Expressions
TypeExpr::TypeExpr(TypeExprType type) : typeExprType(type) {}

bool TypeExpr::operator==(const TypeExpr &other) const {
  if (this->typeExprType != other.typeExprType) {
    return false;
  }
  return this->isEqual(other);
}

IntType::IntType() : TypeExpr(TypeExprType::INT_TYPE) {}

std::unique_ptr<TypeExpr> IntType::clone() const {
  return std::make_unique<IntType>();
}

std::string IntType::toString() const { return "int"; }

bool IntType::isEqual(const TypeExpr &other) const { return true; }

BoolType::BoolType() : TypeExpr(TypeExprType::BOOL_TYPE) {}

std::unique_ptr<TypeExpr> BoolType::clone() const {
  return std::make_unique<BoolType>();
}

std::string BoolType::toString() const { return "bool"; }

bool BoolType::isEqual(const TypeExpr &other) const { return true; }

VariantType::VariantType(std::string name)
    : TypeExpr(TypeExprType::VARIANT_TYPE), name(name) {}

std::unique_ptr<TypeExpr> VariantType::clone() const {
  return std::make_unique<VariantType>(name);
}

std::string VariantType::toString() const { return name; }

bool VariantType::isEqual(const TypeExpr &other) const {
  return this->name == static_cast<const VariantType &>(other).name;
}

VariantConstrType::VariantConstrType(
    std::string constr, std::vector<std::unique_ptr<TypeExpr>> args)
    : TypeExpr(TypeExprType::VARIANT_CONSTR_TYPE), constr(constr),
      args(std::move(args)) {}

std::unique_ptr<TypeExpr> VariantConstrType::clone() const {
  std::vector<std::unique_ptr<TypeExpr>> clonedArgs;
  clonedArgs.reserve(args.size());

  for (const auto &arg : args) {
    clonedArgs.push_back(arg->clone());
  }

  return std::make_unique<VariantConstrType>(constr, std::move(clonedArgs));
}

std::string VariantConstrType::toString() const { return constr; }

// bool VariantConstrType::isEqual(const TypeExpr &other) const {
//   return (this->constr ==
//           static_cast<const VariantConstrType &>(other).constr) &&
//          (this->args == static_cast<const VariantConstrType &>(other).args);
// }

bool VariantConstrType::isEqual(const TypeExpr &other) const {
  const auto &o = static_cast<const VariantConstrType &>(other);

  if (constr != o.constr)
    return false;
  if (args.size() != o.args.size())
    return false;

  for (size_t i = 0; i < args.size(); ++i) {
    if (!(*args[i] == *o.args[i]))
      return false;
  }
  return true;
}

bool VariantConstrType::operator<(const VariantConstrType &other) const {
  if (constr != other.constr)
    return constr < other.constr;

  if (args.size() != other.args.size())
    return args.size() < other.args.size();

  for (size_t i = 0; i < args.size(); ++i) {
    if (!args[i]->isEqual(*other.args[i]))
      return args[i]->toString() < other.args[i]->toString();
  }
  return false;
}

// Expressions
Expr::Expr(ExprType type) : exprType(type) {}

bool Expr::operator==(const Expr &other) const {
  if (this->exprType != other.exprType) {
    return false;
  }
  if (!(this->isEqual(other))) {
    switch (this->exprType) {
    case ExprType::NUM:
      std::cout << "Num values not equal: "
                << static_cast<const Num *>(this)->value << " vs "
                << static_cast<const Num *>(&other)->value << std::endl;
      break;
    case ExprType::BOOL:
      std::cout << "Bool values not equal: "
                << static_cast<const Bool *>(this)->value << " vs "
                << static_cast<const Bool *>(&other)->value << std::endl;
      break;
    default:
      break;
    }
  }
  return this->isEqual(other);
}

Num::Num(int val) : Expr(ExprType::NUM), value(val) {}

std::unique_ptr<Expr> Num::clone() const {
  return std::make_unique<Num>(this->value);
}

bool Num::isEqual(const Expr &other) const {
  return this->value == static_cast<const Num *>(&other)->value;
}

Bool::Bool(bool val) : Expr(ExprType::BOOL), value(val) {}

std::unique_ptr<Expr> Bool::clone() const {
  return std::make_unique<Bool>(this->value);
}

bool Bool::isEqual(const Expr &other) const {
  return this->value == static_cast<const Bool *>(&other)->value;
}

VariantConstr::VariantConstr(std::string constr,
                             std::vector<std::unique_ptr<Expr>> args)
    : Expr(ExprType::VARIANT_CONSTR), constr(std::move(constr)),
      args(std::move(args)) {}

std::unique_ptr<Expr> VariantConstr::clone() const {
  std::vector<std::unique_ptr<Expr>> clonedArgs;
  clonedArgs.reserve(args.size());
  for (const auto &arg : args) {
    clonedArgs.push_back(arg->clone());
  }
  return std::make_unique<VariantConstr>(constr, std::move(clonedArgs));
}

bool VariantConstr::isEqual(const Expr &other) const {
  const auto o = static_cast<const VariantConstr *>(&other);

  if (constr != o->constr) {
    std::cout << "Constr not equal: " << constr << " vs " << o->constr
              << std::endl;
    return false;
  }
  if (args.size() != o->args.size()) {
    std::cout << "Args size not equal: " << args.size() << " vs "
              << o->args.size() << std::endl;
    return false;
  }

  for (size_t i = 0; i < args.size(); ++i) {
    if (!(*args[i] == *o->args[i])) {
      switch (args[i]->exprType) {
      case ExprType::NUM:
        std::cout << "Arg " << i << " Num values not equal: "
                  << static_cast<const Num *>(args[i].get())->value << " vs "
                  << static_cast<const Num *>(o->args[i].get())->value
                  << std::endl;
        break;
      default:
        break;
      }
      std::cout << "Arg " << i << " not equal." << std::endl;
      return false;
    }
  }
  std::cout << "VariantConstrs are equal." << std::endl;
  return true;
}

BinOp::BinOp(std::unique_ptr<Expr> left, BinOpType op,
             std::unique_ptr<Expr> right)
    : Expr(ExprType::BINOP), left(std::move(left)), op(op),
      right(std::move(right)) {}

std::unique_ptr<Expr> BinOp::clone() const {
  return std::make_unique<BinOp>(this->left->clone(), this->op,
                                 this->right->clone());
}

bool BinOp::isEqual(const Expr &other) const { return false; }

UnOp::UnOp(UnOpType op, std::unique_ptr<Expr> expr)
    : Expr(ExprType::UNOP), op(op), expr(std::move(expr)) {}

std::unique_ptr<Expr> UnOp::clone() const {
  return std::make_unique<UnOp>(this->op, this->expr->clone());
}

bool UnOp::isEqual(const Expr &other) const { return false; }

VarUse::VarUse(std::string varName)
    : Expr(ExprType::VAR_USE), name(std::move(varName)) {}

std::unique_ptr<Expr> VarUse::clone() const {
  return std::make_unique<VarUse>(this->name);
}

bool VarUse::isEqual(const Expr &other) const { return false; }

// Statements
Stmt::Stmt(StmtType type) : stmtType(type) {}

Decl::Decl(std::string name, std::unique_ptr<TypeExpr> type)
    : Stmt(StmtType::DECL), name(std::move(name)), type(std::move(type)) {}

Variant::Variant(
    std::string variantName,
    std::map<std::string, std::vector<std::unique_ptr<TypeExpr>>> constrs)
    : Stmt(StmtType::VARIANT), name(std::move(variantName)),
      constructors(std::move(constrs)) {}

Assign::Assign(std::string left, std::unique_ptr<Expr> right)
    : Stmt(StmtType::ASSIGN), left(std::move(left)), right(std::move(right)) {}

If::If(std::unique_ptr<Expr> condition,
       std::vector<std::unique_ptr<Stmt>> thenStmts)
    : Stmt(StmtType::IF), condition(std::move(condition)),
      thenStmts(std::move(thenStmts)) {}

IfElse::IfElse(std::unique_ptr<Expr> condition,
               std::vector<std::unique_ptr<Stmt>> thenStmts,
               std::vector<std::unique_ptr<Stmt>> elseStmts)
    : Stmt(StmtType::IF_ELSE), condition(std::move(condition)),
      thenStmts(std::move(thenStmts)), elseStmts(std::move(elseStmts)) {}

Print::Print(std::unique_ptr<Expr> expr)
    : Stmt(StmtType::PRINT), expr(std::move(expr)) {}

Program::Program(std::vector<std::unique_ptr<Stmt>> stmts)
    : statements(std::move(stmts)) {}
