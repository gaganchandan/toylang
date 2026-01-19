#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

enum class TypeExprType {
  INT_TYPE,
  BOOL_TYPE,
  VARIANT_TYPE,
  VARIANT_CONSTR_TYPE
};

enum class ExprType { NUM, BOOL, BINOP, UNOP, VAR_USE, VARIANT_CONSTR };

enum class BinOpType { ADD, SUB, MUL, DIV, AND, OR, EQ, LT, GT };

enum class UnOpType { NOT };

enum class StmtType { DECL, VARIANT, ASSIGN, IF, IF_ELSE, PRINT };

// Type Expressions
class TypeExpr {
public:
  const TypeExprType typeExprType;

public:
  virtual ~TypeExpr() = default;
  virtual std::unique_ptr<TypeExpr> clone() const = 0;
  virtual std::string toString() const = 0;
  virtual bool isEqual(const TypeExpr &other) const = 0;
  bool operator==(const TypeExpr &other) const;

protected:
  TypeExpr(TypeExprType);
};

class IntType : public TypeExpr {
public:
  IntType();
  std::unique_ptr<TypeExpr> clone() const override;
  std::string toString() const override;
  bool isEqual(const TypeExpr &other) const override;
};

class BoolType : public TypeExpr {
public:
  BoolType();
  std::unique_ptr<TypeExpr> clone() const override;
  std::string toString() const override;
  bool isEqual(const TypeExpr &other) const override;
};

// Variant type for typechecker
class VariantType : public TypeExpr {
public:
  const std::string name;

public:
  VariantType(std::string);
  std::unique_ptr<TypeExpr> clone() const override;
  std::string toString() const override;
  bool isEqual(const TypeExpr &other) const override;
};

// Full type of variant constructor with data
class VariantConstrType : public TypeExpr {
public:
  const std::string constr;
  std::vector<std::unique_ptr<TypeExpr>> args;

public:
  VariantConstrType(std::string, std::vector<std::unique_ptr<TypeExpr>>);
  std::unique_ptr<TypeExpr> clone() const override;
  std::string toString() const override;
  bool isEqual(const TypeExpr &other) const override;
  bool operator<(const VariantConstrType &other) const;
};

// Expressions
class Expr {
public:
  const ExprType exprType;

public:
  virtual ~Expr() = default;
  virtual std::unique_ptr<Expr> clone() const = 0;
  virtual bool isEqual(const Expr &other) const = 0;
  bool operator==(const Expr &other) const;

protected:
  Expr(ExprType);
};

class Num : public Expr {
public:
  const int value;

public:
  explicit Num(int);
  std::unique_ptr<Expr> clone() const override;
  bool isEqual(const Expr &other) const override;
};

class Bool : public Expr {
public:
  const bool value;

public:
  explicit Bool(bool);
  std::unique_ptr<Expr> clone() const override;
  bool isEqual(const Expr &other) const override;
};

// Variant Constr with data
class VariantConstr : public Expr {
public:
  const std::string constr;
  std::vector<std::unique_ptr<Expr>> args;

public:
  VariantConstr(std::string, std::vector<std::unique_ptr<Expr>>);
  std::unique_ptr<Expr> clone() const override;
  bool isEqual(const Expr &other) const override;
};

class VarUse : public Expr {
public:
  const std::string name;

public:
  explicit VarUse(std::string);
  std::unique_ptr<Expr> clone() const override;
  bool isEqual(const Expr &other) const override;
};

class BinOp : public Expr {
public:
  std::unique_ptr<Expr> left;
  BinOpType op;
  std::unique_ptr<Expr> right;

public:
  BinOp(std::unique_ptr<Expr>, BinOpType, std::unique_ptr<Expr>);
  std::unique_ptr<Expr> clone() const override;
  bool isEqual(const Expr &other) const override;
};

class UnOp : public Expr {
public:
  UnOpType op;
  std::unique_ptr<Expr> expr;

public:
  UnOp(UnOpType, std::unique_ptr<Expr>);
  std::unique_ptr<Expr> clone() const override;
  bool isEqual(const Expr &other) const override;
};

// Statements
class Stmt {
public:
  const StmtType stmtType;

public:
  virtual ~Stmt() = default;

protected:
  Stmt(StmtType);
};

class Decl : public Stmt {
public:
  const std::string name;
  const std::unique_ptr<TypeExpr> type;

public:
  Decl(std::string, std::unique_ptr<TypeExpr>);
};

// Variant declaration
class Variant : public Stmt {
public:
  const std::string name;
  std::map<std::string, std::vector<std::unique_ptr<TypeExpr>>> constructors;

public:
  Variant(std::string,
          std::map<std::string, std::vector<std::unique_ptr<TypeExpr>>>);
};

class Assign : public Stmt {
public:
  const std::string left;
  const std::unique_ptr<Expr> right;

public:
  Assign(std::string, std::unique_ptr<Expr>);
};

class If : public Stmt {
public:
  const std::unique_ptr<Expr> condition;
  const std::vector<std::unique_ptr<Stmt>> thenStmts;

public:
  If(std::unique_ptr<Expr>, std::vector<std::unique_ptr<Stmt>>);
};

class IfElse : public Stmt {
public:
  const std::unique_ptr<Expr> condition;
  const std::vector<std::unique_ptr<Stmt>> thenStmts;
  const std::vector<std::unique_ptr<Stmt>> elseStmts;

public:
  IfElse(std::unique_ptr<Expr>, std::vector<std::unique_ptr<Stmt>>,
         std::vector<std::unique_ptr<Stmt>>);
};

class Print : public Stmt {
public:
  const std::unique_ptr<Expr> expr;

public:
  Print(std::unique_ptr<Expr>);
};

class Program {
public:
  const std::vector<std::unique_ptr<Stmt>> statements;

public:
  explicit Program(std::vector<std::unique_ptr<Stmt>>);
};
