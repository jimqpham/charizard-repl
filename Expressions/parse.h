#include <istream>

class Expr;

class Var;

Expr *parse_num(std::istream &in);

Expr *parse_str(std::string s);

Var *parse_var(std::istream &in);

Expr *parse_let(std::istream &in);

Expr *parse_multicand(std::istream &in);

Expr *parse_addend(std::istream &in);

Expr *parse_expr(std::istream &in);

Expr *parse(std::istream &in);

void parse_keyword(std::istream &in, std::string expected);

void consume(std::istream &in, int expect);

void skip_whitespace(std::istream &in);
