#include "catch.h"
#include "../Expressions/num_expr.h"
#include "../Expressions/add_expr.h"
#include "../Expressions/mult_expr.h"
#include "../Expressions/var_expr.h"
#include "../Expressions/let_expr.h"

#include <stdexcept>

TEST_CASE("Test NumExpr has_variable") {
    NumExpr numOne = NumExpr(5);
    CHECK(!numOne.has_variable());
}

TEST_CASE("Test VarExpr has_variable") {
    VarExpr varOne = VarExpr("x");
    CHECK(varOne.has_variable());
}

TEST_CASE("Test AddExpr has_variable") {
    NumExpr numOne = NumExpr(5);
    NumExpr numTwo = NumExpr(7);
    VarExpr varOne = VarExpr("x");

    AddExpr addOne = AddExpr(&numOne, &numTwo);
    AddExpr addTwo = AddExpr(&varOne, &numTwo);
    AddExpr addThree = AddExpr(&addOne, &varOne);
    AddExpr addFour = AddExpr(&varOne, &varOne);

    CHECK(!addOne.has_variable());
    CHECK(addTwo.has_variable());
    CHECK(addThree.has_variable());
    CHECK(addFour.has_variable());
}

TEST_CASE("Test MultExpr has_variable") {
    NumExpr numOne = NumExpr(5);
    NumExpr numTwo = NumExpr(7);
    VarExpr varOne = VarExpr("x");

    MultExpr multOne = MultExpr(&numOne, &numTwo);
    MultExpr multTwo = MultExpr(&varOne, &numTwo);
    MultExpr multThree = MultExpr(&multOne, &varOne);
    MultExpr multFour = MultExpr(&varOne, &varOne);

    CHECK(!multOne.has_variable());
    CHECK(multTwo.has_variable());
    CHECK(multThree.has_variable());
    CHECK(multFour.has_variable());
}

TEST_CASE("Test LetExpr has_variable") {
    NumExpr numOne = NumExpr(5);
    VarExpr varOne = VarExpr("x");
    AddExpr addOne = AddExpr(&numOne, &varOne); // Expr with var
    MultExpr multOne = MultExpr(&numOne, &numOne); // Expr without var

    CHECK(!LetExpr(&varOne, &numOne, &multOne).has_variable()); // rhs no var, body no var
    CHECK(LetExpr(&varOne, &varOne, &multOne).has_variable());  // rhs has var, body no var
    CHECK(LetExpr(&varOne, &numOne, &addOne).has_variable());   // rhs no var, body has var
    CHECK(LetExpr(&varOne, &varOne, &addOne).has_variable());   // rhs & body have var
}