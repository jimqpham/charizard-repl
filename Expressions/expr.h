#pragma once

#include <string>
#include <iostream>

typedef enum {
    prec_none,      // = 0
    prec_add,       // = 1
    prec_mult     // = 2
} precedence_t;

class Expr {
public:
    virtual bool equals(Expr *o) = 0;

    virtual int interp() = 0;

    virtual bool has_variable() = 0;

    virtual Expr* subst(std::string stringToMatch, Expr* replcExpr) = 0;

    virtual void print(std::ostream &out) = 0;

    std::string to_string(bool prettyPrint = false);

    virtual void pretty_print_at(std::ostream &out, precedence_t precedence) = 0;

    void pretty_print(std::ostream &out);
};
