#include "expr.h"
#include "var_expr.h"

class LetExpr : public Expr {
public:
    VarExpr *variable;
    Expr *rhs;
    Expr *body;

    LetExpr(VarExpr *variable, Expr *rhs, Expr *body);

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
