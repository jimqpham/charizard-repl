#pragma once

class Expr {
public:
    virtual bool equals(Expr *o) = 0;
};
