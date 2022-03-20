#include "expr.h"
#include "../shared_ptr.h"

class FunExpr : public Expr {
private:
    std::string formal_arg;
    PTR(Expr) body;

public:
    FunExpr(std::string formal_arg, PTR(Expr) body);

    bool equals(PTR(Expr) o) override;

    PTR(Val) interp_env(PTR(Env) env) override;

    void print(std::ostream &out) override;

    void pretty_print_at(std::ostream &out,
                         precedence_t precedence,
                         bool kwrdNeedsPars,
                         std::streampos &newLinePos) override;
};

