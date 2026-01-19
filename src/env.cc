#include "env.hh"

ValueEnvironment::ValueEnvironment() = default;

void ValueEnvironment::put(const std::string &key, Expr *expr) {
  table[key] = expr;
}

Expr *ValueEnvironment::get(const std::string &key) {
  if (table.find(key) != table.end()) {
    return table[key];
  }
  return nullptr;
}

TypeEnvironment::TypeEnvironment() = default;

void TypeEnvironment::put(const std::string &key, TypeExpr *typeExpr) {
  table[key] = typeExpr;
}

TypeExpr *TypeEnvironment::get(const std::string &key) {
  if (table.find(key) != table.end()) {
    return table[key];
  }
  return nullptr;
}

bool TypeEnvironment::checkVariant(VariantType variant) {
  for (auto &constr : variants) {
    if (constr.second == variant) {
      return true;
    }
  }
  return false;
}

void TypeEnvironment::newVariant(VariantConstrType constr,
                                 VariantType variant) {
  variants.emplace(std::move(constr), std::move(variant));
}

TypeExpr *TypeEnvironment::getConstrType(VariantConstrType constr) {
  auto found = variants.find(constr);
  if (found != variants.end()) {
    return &found->second;
  }
  return nullptr;
}
