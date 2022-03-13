#pragma once

#include <string>
#include <iostream>
#include "../shared_ptr.h"

typedef enum {
    prec_none,
    prec_equal,
    prec_add,
    prec_mult
} precedence_t;

class Val;

CLASS(Expr) {
public:
    virtual bool equals(PTR(Expr) o) = 0;

    virtual PTR(Val) interp() = 0;

    virtual PTR(Expr) subst(std::string stringToMatch, PTR(Expr) replcExpr) = 0;

    virtual void print(std::ostream &out) = 0;

    virtual void pretty_print_at(std::ostream &out,
                                 precedence_t precedence,
                                 bool kwrdNeedsPars,
                                 std::streampos &newLinePos) = 0;

    ////// HELPER METHOD
    /*
     * Convert to string. Using print by default with an option to use pretty_print
     */
    std::string to_string(bool prettyPrint = false);

    /*
    * This is a wrapper method that automatically set the accumulator so the user don't have to
    */
    void pretty_print(std::ostream &out);
};
