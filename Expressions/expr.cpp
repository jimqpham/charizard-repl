#include <sstream>
#include "expr.h"
#include "../Utils/env.h"

/*
 * Convert to string. Using print by default with an option to use pretty_print
 */
std::string Expr::to_string(bool prettyPrint) {
    std::stringstream out("");
    if (!prettyPrint) {
        this->print(out);
        return out.str();
    } else {
        this->pretty_print(out);
        return out.str();
    }
}

/*
 * This is a wrapper class that automatically set the accumulator so the user don't have to
 */
void Expr::pretty_print(std::ostream &out) {
    std::streampos newLinePos = 0;
    this->pretty_print_at(out, prec_none, false, newLinePos);
}

PTR(Val) Expr::interp() {
    return this->interp_env(Env::empty);
}
