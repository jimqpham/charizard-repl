#include "string"
#include "../Expressions/expr.h"

class Var: public Expr {
    public:
        std::string name;

        Var(std::string name);

        bool equals(Expr* o) override;

        int interp() override;

        bool has_variable() override;

        Expr* subst(std::string stringToMatch, Expr* replcExpr);
};