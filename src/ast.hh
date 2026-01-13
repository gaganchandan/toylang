#pragma once

#include <memory>
#include <string>

enum class TypeExprType { INT_TYPE, TYPECONST };

enum class VarType { TYPED_VAR, UNTYPED_VAR };

enum class ExprType { NUM, BINOP, VAR_USE };

enum class BinOpType { ADD, SUB, MUL, DIV };

enum class StmtType { SEQ, ASSIGN, PRINT };

// Type Expressions
class TypeExpr {
public:
  const TypeExprType typeExprType;

public:
  virtual ~TypeExpr() = default;
  virtual std::string toString() const = 0;

protected:
  TypeExpr(TypeExprType);
};

class IntType : public TypeExpr {
public:
  IntType();
  virtual std::string toString() const;
};

class TypeConst : public TypeExpr {
public:
  const std::string typeName;

public:
  explicit TypeConst(std::string);
  virtual std::string toString() const;
};

// Variables
class Var {
public:
  const VarType varType;
  const std::string name;

public:
  virtual ~Var() = default;
  virtual std::string toString() const = 0;

protected:
  Var(VarType, std::string);
};

class TypedVar : public Var {
public:
  std::unique_ptr<TypeExpr> typeExpr;

public:
  TypedVar(std::string, std::unique_ptr<TypeExpr>);
  virtual std::string toString() const;
};

class UntypedVar : public Var {
public:
  UntypedVar(std::string);
  virtual std::string toString() const;
};

// Expressions
class Expr {
public:
  const ExprType exprType;

public:
  virtual ~Expr() = default;

protected:
  Expr(ExprType);
};

class Num : public Expr {
public:
  const int value;

public:
  explicit Num(int);
};

class VarUse : public Expr {
public:
  const std::string name;

public:
  explicit VarUse(std::string);
};

class BinOp : public Expr {
public:
  std::unique_ptr<Expr> left;
  BinOpType op;
  std::unique_ptr<Expr> right;

public:
  BinOp(std::unique_ptr<Expr>, BinOpType, std::unique_ptr<Expr>);
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

class Seq : public Stmt {
public:
  const std::unique_ptr<Stmt> first;
  const std::unique_ptr<Stmt> second;

public:
  Seq(std::unique_ptr<Stmt>, std::unique_ptr<Stmt>);
};

class Assign : public Stmt {
public:
  const std::unique_ptr<Var> left;
  const std::unique_ptr<Expr> right;

public:
  Assign(std::unique_ptr<Var>, std::unique_ptr<Expr>);
};

class Print : public Stmt {
public:
  const std::unique_ptr<Expr> expr;

public:
  Print(std::unique_ptr<Expr>);
};
