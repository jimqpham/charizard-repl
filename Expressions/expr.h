#pragma once

#include <string>
#include <iostream>

typedef enum {
    prec_none,
    prec_add,
    prec_mult
} precedence_t;

class Expr {
public:
    virtual bool equals(Expr *o) = 0;

    virtual int interp() = 0;

    virtual bool has_variable() = 0;

    virtual Expr* subst(std::string stringToMatch, Expr* replcExpr) = 0;

    virtual void print(std::ostream &out) = 0;

    /*
     * Convert to string. Using print by default with an option to use pretty_print
     */
    std::string to_string(bool prettyPrint = false);

    virtual void pretty_print_at(std::ostream &out, precedence_t precedence) = 0;

    /*
    * This is a wrapper class that automatically set the accumulator so the user don't have to
    */
    void pretty_print(std::ostream &out);
};
