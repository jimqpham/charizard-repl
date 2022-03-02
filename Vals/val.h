#pragma once

class Expr;

class Val {
public:
    virtual bool value_equals(Val *o) = 0;

    virtual Val *add_to(Val *o) = 0;

    virtual Val *mult_by(Val *o) = 0;

    virtual std::string to_string() = 0;

    virtual Expr *to_expr() = 0;

    virtual Val *call(Val *actual_arg) = 0;
};
