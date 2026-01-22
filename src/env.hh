#pragma once
#include "ast.hh"
#include <map>
#include <string>

class ValueEnvironment {
private:
  std::map<std::string, Expr *> table;

public:
  ValueEnvironment();
  void put(const std::string &key, Expr *expr);
  Expr *get(const std::string &key);
};

class TypeEnvironment {
private:
  std::map<std::string, TypeExpr *> table;
  std::map<VariantConstrType, VariantType> variants;
  std::vector<RecordType> records;

public:
  TypeEnvironment();
  void put(const std::string &key, TypeExpr *typeExpr);
  TypeExpr *get(const std::string &key);
  bool checkVariant(VariantType);
  void newVariant(VariantConstrType constr, VariantType variant);
  TypeExpr *getVariant(VariantConstrType &constr);
  bool checkRecord(RecordType record);
  void newRecord(RecordType record);
  TypeExpr *getRecord(RecordType &record);
};
