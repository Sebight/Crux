#pragma once
#include <optional>
#include <variant>
#include <string>

typedef std::variant<std::string, double> Literal;
typedef std::optional<Literal> OptLiteral;

std::string getLiteralString(const Literal& value);

OptLiteral makeOptLiteral(const std::string& value);
OptLiteral makeOptLiteral(double value);