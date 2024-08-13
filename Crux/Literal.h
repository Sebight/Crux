#pragma once
#include <optional>
#include <variant>
#include <string>

// TODO: Literal could in the end be a struct with 3 types: string, double, bool
// This would ease up the handling of literals in the parser and interpreter, but could introdduce undefined behaviour

typedef std::variant<std::string, double> Literal;
typedef std::optional<Literal> OptLiteral;

std::string getLiteralString(const Literal& value);

OptLiteral makeOptLiteral(const std::string& value);
OptLiteral makeOptLiteral(double value);