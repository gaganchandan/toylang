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

TypeExpr *TypeEnvironment::getVariant(VariantConstrType &constr) {
  auto found = variants.find(constr);
  if (found != variants.end()) {
    return &found->second;
  }
  return nullptr;
}

bool TypeEnvironment::checkRecord(RecordType record) {
  for (auto &rec : records) {
    if (rec.fields == record.fields) {
      return true;
    }
  }
  return false;
}

void TypeEnvironment::newRecord(RecordType record) {
  // Record Type has no copy constructor. Rebuild full record type and move it.
  std::map<std::string, std::unique_ptr<TypeExpr>> newFields;
  for (const auto &pair : record.fields) {
    newFields[pair.first] = pair.second->clone();
  }
  RecordType newRecord(record.name, std::move(newFields));
  records.push_back(std::move(newRecord));
}

TypeExpr *TypeEnvironment::getRecord(RecordType &record) {
  for (auto &rec : records) {
    if (rec == record) {
      return &rec;
    }
  }
  return nullptr;
}
