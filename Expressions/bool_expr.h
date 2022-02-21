#include "expr.h"

class BoolExpr : public Expr {
private:
    bool val;

public:
    explicit BoolExpr(bool val);

    bool equals(Expr *o) override;

    Val *interp() override;

    bool has_variable() override;

    Expr *subst(std::string stringToMatch, Expr *replcExpr) override;

    void print(std::ostream &out) override;

    void pretty_print_at(std::ostream &out,
                         precedence_t precedence,
                         bool needsParenthesesForLet,
                         std::streampos &newLinePos) override;
};
