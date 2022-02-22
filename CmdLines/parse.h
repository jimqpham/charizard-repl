#include <istream>

class Expr;

class VarExpr;

Expr *parse_num(std::istream &in);

Expr *parse_str(std::string s);

VarExpr *parse_var(std::istream &in);

Expr *parse_let(std::istream &in);

Expr *parse_if(std::istream &in);

Expr *parse_multicand(std::istream &in);

Expr *parse_addend(std::istream &in);

Expr *parse_expr(std::istream &in);

Expr *parse(std::istream &in);

void parse_keyword(std::istream &in, std::string expected, bool spaceAfter);

bool check_and_consume(std::istream &in, char expected);

void consume(std::istream &in, int expect);

void skip_whitespace(std::istream &in);
