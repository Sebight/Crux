#include "Literal.h"

std::string getLiteralString(const Literal& value)
{
	if (std::holds_alternative<std::string>(value))
	{
		return std::get<std::string>(value);
	}
	else
	{
		return std::to_string(std::get<double>(value));
	}
}

OptLiteral makeOptLiteral(const std::string& value)
{
	if (value.empty())
	{
		return std::nullopt;
	}
	else
	{
		return Literal(value);
	}
}

OptLiteral makeOptLiteral(double value)
{
	return Literal(value);
}