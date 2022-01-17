#include "string"
#include "../Expressions/expr.h"

class Var: public Expr {
    public:
        std::string name;

        Var(std::string name);

        virtual bool equals(Expr* o);
};