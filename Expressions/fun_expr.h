#include "expr.h"
#include "../shared_ptr.h"

class FunExpr : public Expr {
private:
    std::string formal_arg;
    Expr *body;

public:
    FunExpr(std::string formal_arg, Expr *body);

    bool equals(Expr *o) override;

    Val *interp() override;

    Expr *subst(std::string stringToMatch, Expr *replcExpr) override;

    void print(std::ostream &out) override;

    void pretty_print_at(std::ostream &out,
                         precedence_t precedence,
                         bool kwrdNeedsPars,
                         std::streampos &newLinePos) override;
};

