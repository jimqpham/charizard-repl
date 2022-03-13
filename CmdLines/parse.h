#include <istream>
#include "../shared_ptr.h"

class Expr;

class VarExpr;

PTR(Expr) parse_num(std::istream &in);

PTR(VarExpr) parse_var(std::istream &in);

PTR(Expr) parse_true(std::istream &in);

PTR(Expr) parse_false(std::istream &in);

PTR(Expr) parse_let(std::istream &in);

PTR(Expr) parse_if(std::istream &in);

PTR(Expr) parse_underscore(std::istream &in);

PTR(Expr) parse_multicand(std::istream &in);

PTR(Expr) parse_addend(std::istream &in);

PTR(Expr) parse_comparg(std::istream &in);

PTR(Expr) parse_expr(std::istream &in);

PTR(Expr) parse(std::istream &in);

// HELPERS FROM HERE

PTR(Expr) parse_str(std::string s);

void parse_char_group(std::istream &in, std::string expected);

bool check_and_consume(std::istream &in, char expected);

void consume(std::istream &in, int expect);

void skip_whitespace(std::istream &in);
