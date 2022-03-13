#include "expr.h"
#include "../shared_ptr.h"

class BoolExpr : public Expr {
private:
    bool val;

public:
    explicit BoolExpr(bool val);

    bool equals(Expr *o) override;

    Val *interp() override;

    Expr *subst(std::string stringToMatch, Expr *replcExpr) override;

    void print(std::ostream &out) override;

    void pretty_print_at(std::ostream &out,
                         precedence_t precedence,
                         bool kwrdNeedsPars,
                         std::streampos &newLinePos) override;
};
