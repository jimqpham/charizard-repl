#include "expr.h"

class Var;

class Let : public Expr {
public:
    Var *variable;
    Expr *rhs;
    Expr *body;

    Let(Var *variable, Expr *rhs, Expr *body);

    bool equals(Expr *o) override;

    int interp() override;

    bool has_variable() override;

    Expr *subst(std::string stringToMatch, Expr *replcExpr) override;

    void print(std::ostream &out) override;

    void pretty_print_at(
            std::ostream &out,
            precedence_t precedence,
            bool needsParenthesesForLet,
            std::streampos &newLinePos) override;
};
